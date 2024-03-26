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

/* This function should initialize the DAQ and return a device
 * handle. The function takes as a parameter the calibratrion info to
 * be filled up with what obtained from the device. */
HANDLE init_DAQ(u3CalibrationInfo * caliInfo)
{
	HANDLE hDevice;
	int localID;
	
	/* Invoke openUSBConnection function here */
	hDevice = openUSBConnection(localID);
	/* Invoke getCalibrationInfo function here */
	getCalibrationInfo(hDevice, caliInfo);

	return hDevice;
}

int main(int argc, char **argv)
{
	u3CalibrationInfo caliInfo;
	/* Invoke init_DAQ and handle errors if needed */
    HANDLE hDevice = init_DAQ(&caliInfo);

	/* Provide prompt to the user for a voltage range between 0
	 * and 5 V. Require a new set of inputs if an invalid range
	 * has been entered. */
    double minVoltage, maxVoltage;

    printf("Enter a min voltage between 0 and 5 V: ");
    scanf("%lf", &minVoltage);

    printf("Enter a max voltage between 0 and 5 V: ");
    scanf("%lf", &maxVoltage);

    if(minVoltage < 0 || minVoltage > 5 || maxVoltage < 0 || maxVoltage > 5 || minVoltage > maxVoltage)
    {
        printf("Invalid voltage range. Please enter a valid range.\n");
        return EXIT_FAILURE;
    }
	
	/* Compute the maximum resolutiuon of the CLOCK_REALTIME
	 * system clock and output the theoretical maximum frequency
	 * for a square wave */
    struct timespec res;
    clock_getres(CLOCK_REALTIME, &res);
    double maxFrequency = 1 / (2 * res.tv_sec + 2 * res.tv_nsec * 1e-9);
    printf("Theoretical maximum frequency for a square wave: %f Hz\n", maxFrequency);

	/* Provide prompt to the user to input a desired square wave
	 * frequency in Hz. */
    double frequency;
    printf("Enter a desired square wave frequency in Hz: ");
    scanf("%lf", &frequency);

    if(frequency > maxFrequency)
    {
        printf("Frequency too high. Please enter a lower frequency.\n");
        return EXIT_FAILURE;
    }

	/* Program a timer to deliver a SIGRTMIN signal, and the
	 * corresponding signal handler to output a square wave on
	 * BOTH digital output pin FIO2 and analog pin DAC0. */
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;
    long long freq_nanosecs;
    sigset_t mask;
    struct sigaction sa;

    // Establish handler for timer signal
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    // Block timer signal temporarily
    sigemptyset(&mask);
    sigaddset(&mask, SIG);
    if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }

    // Create the timer
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCKID, &sev, &timerid) == -1) {
        perror("timer_create");
        return 1;
    }

    // Start the timer
    freq_nanosecs = 500000000; // 0.5 seconds
    its.it_value.tv_sec = freq_nanosecs / 1000000000;
    its.it_value.tv_nsec = freq_nanosecs % 1000000000;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        return 1;
    }

    // Unblock the timer signal
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }

    // Main loop
    while (1) {
        pause(); // Wait for signals
    }


	/* The square wave generated on the DAC0 analog pin should
	 * have the voltage range specified by the user in the step
	 * above. */
    static int state = 0;

    if(state == 0)
    {
        eDAC(hDevice, caliInfo, 0, 0, 0, 0);
        state = 1;
    }
    else
    {
        eDAC(hDevice, caliInfo, 0, 0, 0, 5);
        state = 0;
    }

	/* Display a prompt to the user such that if the "exit"
	 * command is typed, the USB DAQ is released and the program
	 * is terminated. */
    char command[256];

    do {
        printf("Enter a minimum voltage range between 0 and 5 V: ");
        scanf("%lf", &min_voltage);

        printf("Enter a maximum voltage range between 0 and 5 V: ");
        scanf("%lf", &max_voltage);

        // Rest of your code...

        printf("Do you want to continue? (y/n): ");
        scanf("%s", command);

    } while (strcmp(command, "y") == 0);

    // Release the USB DAQ
    closeUSBConnection(hDevice);
    printf("USB DAQ released. Exiting program.\n");

	return EXIT_SUCCESS;
}

