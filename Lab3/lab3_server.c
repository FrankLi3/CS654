/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: template file for serial          */
/*                communication server              */
/*                                                  */
/****************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "pc_crc16.h"
#include "lab3.h"
#include "lab3_troll.h"

#define GREETING_STR						\
    "CS454/654 - Lab 3 Server\n"				\
    "Author: Renato Mancuso (BU)\n"				\
    "(Enter a message to send.  Type \"quit\" to exit)\n"

#define USAGE_STR							\
	"\nUSAGE: %s [-v] [-t percentage] <dev>\n"			\
	"   -v \t\t Verbose output\n"					\
	"   -t \t\t Invoke troll with specified bit flipping percentage\n" \
	"   <dev> \t Path to serial terminal device to use, e.g. /dev/ttyUSB0\n\n"

#define TROLL_PATH "./lab3_troll"

int main(int argc, char* argv[])
{
	double troll_pct=0.3;		// Perturbation % for the troll (if needed)
	int ifd,ofd,i,N,troll=0;	// Input and Output file descriptors (serial/troll)
	char str[MSG_BYTES_MSG],opt;	// String input
	struct termios oldtio, tio;	// Serial configuration parameters
	int VERBOSE = 0;		// Verbose output - can be overriden with -v
	int dev_name_len;
	char * dev_name = NULL;
	
	/* Parse command line options */
	while ((opt = getopt(argc, argv, "-t:v")) != -1) {
		switch (opt) {
		case 1:
			dev_name_len = strlen(optarg);
			dev_name = (char *)malloc(dev_name_len);
			strncpy(dev_name, optarg, dev_name_len);
			break;
		case 't':
			troll = 1; 
			troll_pct = atof(optarg);                    
			break;
		case 'v':
			VERBOSE = 1;
			break;
		default:
			break;
		}
	}

	/* Check if a device name has been passed */
	if (!dev_name) {
		fprintf(stderr, USAGE_STR, argv[0]);
		exit(EXIT_FAILURE);
	}
	
	// Open the serial port (/dev/ttyS1) read-write
	ifd = open(dev_name, O_RDWR | O_NOCTTY);
	if (ifd < 0) {
		perror(dev_name);
		exit(EXIT_FAILURE);
	}

	printf(GREETING_STR);

	// Start the troll if necessary
	if (troll)
	{
		// Open troll process (lab5_troll) for output only
		FILE * pfile;		// Process FILE for troll (used locally only)
		char cmd[128];		// Shell command

		snprintf(cmd, 128, TROLL_PATH " -p%f %s %s", troll_pct,
			 (VERBOSE) ? "-v" : "", dev_name);

		pfile = popen(cmd, "w");
		if (!pfile) { perror(TROLL_PATH); exit(-1); }
		ofd = fileno(pfile);
	}
	else ofd = ifd;		// Use the serial port for both input and output

	//
 	// WRITE ME: Set up the serial port parameters and data format
	//
	
	tcgetattr(ifd, &oldtio); // oldtio stores old serial settings
	tio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	tio.c_iflag = 0;
	tio.c_oflag = 0;
	tio.c_lflag = 0;
	
	tcflush(ifd, TCIFLUSH); // flush pending transfers
	
	tcsetattr(ifd, TCSANOW, &tio); // enable new serial port settings

	

	while(1)
	{

		//
		// WRITE ME: Read a line of input (Hint: use fgetc(stdin) to read each character)
		//
		char bigbuf[300];
		
		
		//while(getline((char**)&str, NULL, stdin) == -1);
		int str_len = 0;
		while ((bigbuf[4+str_len] = fgetc(stdin)) != '\n') {
			str_len++;
			
		}
		bigbuf[4+str_len] = '\0';

		if (strcmp(bigbuf+4, "quit") == 0) break;

		//
		// WRITE ME: Compute crc (only lowest 16 bits are returned)
		//
		char ack = MSG_NACK;
		char crc1 = 0x00FF & pc_crc16(bigbuf+4, str_len);
		char crc2 = 0x00FF & (pc_crc16(bigbuf+4, str_len) >> 8);
		int attempts = 0;
		
		bigbuf[0] = MSG_START;
		bigbuf[1] = crc2;
		bigbuf[2] = crc1;
		bigbuf[3] = (char)str_len;
	
		while (!ack)
		{
			printf("Sending (attempt %d)...\n", ++attempts);

			
			// 
			// WRITE ME: Send message
			//
			write(ofd, bigbuf, 4 + str_len);
			
		
			printf("Message sent, waiting for ack... ");
			usleep(100000);
			
			//
			// WRITE ME: Wait for MSG_ACK or MSG_NACK
			//
			while(read(ifd, &ack, 1) == 0);
			
			if (ack == 1){
				ack = MSG_ACK;
			}else{
				ack = MSG_NACK;
				printf("%d\n",ack);
			}

			printf("%s\n", ack ? "ACK" : "NACK, resending");
		}
		printf("\n");
	}


	//
	// WRITE ME: Reset the serial port parameters
	//
	tcsetattr(ifd, TCSANOW, &oldtio); // enable old serial port settings

	
	// Close the serial port
	close(ifd);
	
	return EXIT_SUCCESS;
}

