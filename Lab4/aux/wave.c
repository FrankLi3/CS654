/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: template file for digital and     */
/*                analog square wave generation     */
/*                via the LabJack U3-LV USB DAQ     */
/*                                                  */
/****************************************************/

#include "u3.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#define MAX_TRIES 15
HANDLE hDevice;
u3CalibrationInfo caliInfo;
double vMax, vMin;
bool d_state = 0;

long channel = 2;
long configIO = 1;
long state = 1;


void SignalHandler(int sig, siginfo_t *si, void *uc){
	
	if(sig==SIGRTMIN){
	    long dig_wave = eDO(hDevice, configIO, channel, state);
	    d_state = !d_state;
	    state = d_state? 1 : 0;
	    //printf("digital ");
	    fflush(stdout);
	}
	else if(sig==SIGRTMAX){
	
	    if(d_state){
	        long ana_wave = eDAC(hDevice, &caliInfo, configIO, 0, vMax,0,0,0);
	    }
	    else if(!d_state){
	        long ana_wave = eDAC(hDevice, &caliInfo, configIO, 0, vMin,0,0,0);
	    }
	    
	    d_state = !d_state;
	    //printf("analog ");
	    fflush(stdout);
	}

	
}

void SignalHandlerA(int sig, siginfo_t *si, void *uc){
	long wave = eDO(hDevice, configIO, channel, state);
	d_state = !d_state;
	state = d_state? 1 : 0;
	fflush(stdout);
}


/* This function should initialize the DAQ and return a device
 * handle. The function takes as a parameter the calibratrion info to
 * be filled up with what obtained from the device. */
HANDLE init_DAQ(u3CalibrationInfo * caliInfo)
{
	HANDLE hDevice;
	int localID = -1;
	
	/* Invoke openUSBConnection function here */
	hDevice = openUSBConnection(localID);
	/* Invoke getCalibrationInfo function here */
	getCalibrationInfo(hDevice, caliInfo);
	
	return hDevice;
}

int main(int argc, char **argv)
{
	double vRange;
	int i, ret;
	long long freq_hzn;
	double freq_hz;

	char genAnalogWave, genDigitalWave;

	
	/* Invoke init_DAQ and handle errors if needed */
	
	hDevice = init_DAQ(&caliInfo);
	if (hDevice == 0) {
		printf("Failed to intialized DAQ\n");
		return EXIT_FAILURE;
	}

	/* Provide prompt to the user for a voltage range between 0
	 * and 5 V. Require a new set of inputs if an invalid range
	 * has been entered. */
	i = 0;
	while (i < MAX_TRIES) {
    		printf("Provide min voltage between 0 and 5V:");
    		ret = scanf("%lf", &vMin);
    		if (ret != 1 || vMin < 0 || vMin >= 5) {
        		printf("Invalid minimum voltage, please try again.\n");
        		i++;
   		 } else {
        		break;
    		}
	}

	if (i == MAX_TRIES) {
    		printf("Too many invalid inputs for minimum voltage, exiting...\n");
    		return EXIT_FAILURE;
	}

	i = 0;
	while (i < MAX_TRIES) {
	  	printf("Provide max voltage between 0 and 5V:");
	    	ret = scanf("%lf", &vMax);
	    	if (ret != 1 || vMax <= vMin || vMax > 5) {
			printf("Invalid maximum voltage, please try again.\n");
			i++;
	    	} else {
			vRange = vMax - vMin;
			break;
	    	}
	}

	if (i == MAX_TRIES) {
	    	printf("Too many invalid inputs for maximum voltage, exiting...\n");
	    	return EXIT_FAILURE;
	}
	

	/* Compute the maximum resolutiuon of the CLOCK_REALTIME
	 * system clock and output the theoretical maximum frequency
	 * for a square wave */
	struct timespec resolution;
	
	if(clock_getres(CLOCK_REALTIME,&resolution)==-1){
		perror("clock_getres");
		return 1;
	};
	
	float seconds = (float)resolution.tv_sec + (float)resolution.tv_nsec / 1e9;
    	float max_frequency = 1.0/ seconds;
	printf("Clock realtime resolution is: %f Hz\n", max_frequency);
	
	/* Provide prompt to the user to input a desired square wave
	 * frequency in Hz. */
	while (1) {
	      printf("Enter desired square wave frequency (Hz): ");
	      ret = scanf("%lf", &freq_hz);
	      if (ret != 1 || freq_hz <= 0) {
		  printf("Invalid frequency, please try again.\n");
	      } else {
		  break;
	      }
	}

	    do {
		printf("Do you want to generate an analog wave? [y/n]: ");
		scanf(" %c", &genAnalogWave);
	    } while (genAnalogWave != 'y' && genAnalogWave != 'n');

	    do {
		printf("Do you want to generate a digital wave? [y/n]: ");
		scanf(" %c", &genDigitalWave);
	    } while (genDigitalWave != 'y' && genDigitalWave != 'n');

	    /* Setup timer and signal handler for digital square wave */
	    
	    
	    struct sigaction sa;
	    //sigset_t mask, wait_mask;
	    struct sigevent ev1, ev2;
	    struct itimerspec it;
	    struct itimerspec it2;
	    timer_t timer1, timer2;
	    
	    
	    /* Clear all the masks, reset all structures */
	    sa.sa_flags = SA_SIGINFO;
	    sa.sa_sigaction = SignalHandler;
	    
	    sigaddset(&sa.sa_mask, SIGRTMIN);
	    sigaddset(&sa.sa_mask, SIGRTMAX);
	
	    
	    sigaction(SIGRTMIN, &sa, NULL);
	    sigaction(SIGRTMAX, &sa, NULL);
	    
	    ev2.sigev_notify = ev1.sigev_notify = SIGEV_SIGNAL;
	    ev1.sigev_signo = SIGRTMIN;
	    ev2.sigev_signo = SIGRTMAX;
	    
	    ev1.sigev_value.sival_ptr = &timer1;
	    ev2.sigev_value.sival_ptr = &timer2;
	    
	    if(genAnalogWave == 'n' && genDigitalWave == 'y'){
	    	timer_create(CLOCK_REALTIME, &ev1, &timer1);
	    	
	    	//long long period = 1 / freq_hz * 1000000000LL;
	    	//printf(period);
	    	
	    	//freq_hzn = freq_hz * ;
	    	//it.it_value.tv_sec = 0;
		//it.it_value.tv_nsec = 100;
		//it.it_interval.tv_sec = 0;
		//it.it_interval.tv_nsec = 500000000;
		
		
		//timer_settime(timer1, 0, &it, NULL);
		
		    // Assuming frequency is stored in freq_hz
    		//double freq_hz = 0.5; // Example frequency
    		long long halfPeriodNsec = (1 / freq_hz) * 500000000LL; 

    		struct itimerspec it;
    		memset(&it, 0, sizeof(it));
    		it.it_value.tv_sec = halfPeriodNsec / 1000000000LL; 
    		it.it_value.tv_nsec = halfPeriodNsec % 1000000000LL; 
    		it.it_interval.tv_sec = it.it_value.tv_sec; 
    		it.it_interval.tv_nsec = it.it_value.tv_nsec; 

    		// Assuming timer1 is for the digital output
   		timer_settime(timer1, 0, &it, NULL);

		
	    }
	    else if(genAnalogWave == 'y' && genDigitalWave == 'n'){
	    	timer_create(CLOCK_REALTIME, &ev2, &timer2);
	       
	        //freq_hzn = freq_hz * 1000000000LL;
	    	//it.it_value.tv_sec = freq_hz;
		//it.it_value.tv_nsec = freq_hzn % 1000000000LL;
		//it.it_interval.tv_sec = (1/it.it_value.tv_sec) / 2;
		//it.it_interval.tv_nsec = (1/it.it_value.tv_nsec) / 2 ;
	    
	        
	        long long halfPeriodNsec = (1 / freq_hz) * 500000000LL; 

    		struct itimerspec it;
    		memset(&it, 0, sizeof(it));
    		it.it_value.tv_sec = halfPeriodNsec / 1000000000LL; 
    		it.it_value.tv_nsec = halfPeriodNsec % 1000000000LL; 
    		it.it_interval.tv_sec = it.it_value.tv_sec; 
    		it.it_interval.tv_nsec = it.it_value.tv_nsec; 

    		// Assuming timer1 is for the digital output
   		timer_settime(timer2, 0, &it, NULL);
	        

	        
	    }
	    
	    //while(1);
	    
	    
	    char cmd[10];
	    printf("Type 'exit' to quit: ");
		
			
	    while(1){
	    	if (scanf("%9s", cmd) == 1){
		  if (strcmp(cmd,"exit") == 0) {
	    	    return EXIT_SUCCESS;
	    	  }
	    	}
	    }
		

	    /* Display a prompt to the user such that if the "exit"
	     * command is typed, the USB DAQ is released and the program
	     * is terminated. */
	    
	
	//return EXIT_SUCCESS;
		
	
}



