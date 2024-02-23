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
	// Set up the serial port parameters and data format
	tcgetattr(ifd, &oldtio); // Save current serial port settings
	memset(&tio, 0, sizeof(tio)); // Clear struct for new port settings

	tio.c_cflag = CS8 | CLOCAL | CREAD; // Control mode flags
	tio.c_iflag = IGNPAR; // Input mode flags to ignore framing and parity errors
	tio.c_oflag = 0; // Output mode flags
	tio.c_lflag = 0; // Local mode flags

	cfsetospeed(&tio, B9600); // Set output speed
	cfsetispeed(&tio, B9600); // Set input speed

	tcflush(ifd, TCIFLUSH); // Flush the port
	tcsetattr(ifd, TCSANOW, &tio); // Apply the settings to the port

	while(1)
	{

		//
		// WRITE ME: Read a line of input (Hint: use fgetc(stdin) to read each character)
		//
		// Read a line of input
		i = 0; // Initialize index for input string
		while ((opt = fgetc(stdin)) != '\n' && i < MSG_BYTES_MSG - 1) {
			str[i++] = opt;
		}
		str[i] = '\0'; // Null-terminate the string


		if (strcmp(str, "quit") == 0) break;

		//
		// WRITE ME: Compute crc (only lowest 16 bits are returned)
		//
		// Compute CRC (only lowest 16 bits are returned)
		unsigned short crc;
		crc = pc_crc16(str, strlen(str));

	
	    int ack = MSG_NACK;
		int attempts = 0;
		while (!ack)
		{
			printf("Sending (attempt %d)...\n", ++attempts);

			
			// 
			// WRITE ME: Send message
			//
			// Format the message according to the specified protocol
			unsigned char msg[MSG_BYTES_MSG + 4]; // Plus 4 for start byte, CRC (2 bytes), and length
			int msg_len = strlen(str);
			msg[0] = 0x0; // Start byte
			msg[1] = crc >> 8; // CRC high byte
			msg[2] = crc & 0xFF; // CRC low byte
			msg[3] = msg_len; // Length of message body

			// Copy the message body
			memcpy(msg + 4, str, msg_len);

			// Send message
			int bytes_sent = 0;
			int to_send = msg_len + 4;
			while (bytes_sent < to_send) {
				int n = write(ofd, msg + bytes_sent, to_send - bytes_sent);
				if (n < 0) {
					perror("Write failed");
					exit(EXIT_FAILURE);
				}
				bytes_sent += n;
			}


		
			printf("Message sent, waiting for ack... ");

			
			//
			// WRITE ME: Wait for MSG_ACK or MSG_NACK
			//
			// Wait for MSG_ACK or MSG_NACK
			unsigned char ack_nack;
			int ack = 0;
			read(ifd, &ack_nack, 1); // Assuming blocking read for simplicity
			ack = (ack_nack == MSG_ACK);



			printf("%s\n", ack ? "ACK" : "NACK, resending");
		}
		printf("\n");
	}


	//
	// WRITE ME: Reset the serial port parameters
	//
	// Reset the serial port parameters
	tcsetattr(ifd, TCSANOW, &oldtio);

	
	// Close the serial port
	close(ifd);
	
	return EXIT_SUCCESS;
}

