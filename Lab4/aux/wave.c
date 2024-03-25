/*****************************************************************************/
/* */
/* CS-454/654 Embedded Systems Development */
/* Instructor: Renato Mancuso <rmancuso@bu.edu> */
/* Boston University */
/* */
/* Description: template file for digital and */
/* analog square wave generation */
/* via the LabJack U3-LV USB DAQ */
/* */
/*****************************************************************************/

#include "u3.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#define MAX_TRIES 5

HANDLE hDevice;
u3CalibrationInfo caliInfo;
double vMax, vMin;  // 用戶指定的電壓範圍

// Timer handler函數原型
void analogHandler(int sig, siginfo_t *si, void *uc);
void digitalHandler(int sig, siginfo_t *si, void *uc);

/* This function should initialize the DAQ and return a device
 * handle. The function takes as a parameter the calibratrion info to
 * be filled up with what obtained from the device. */
HANDLE init_DAQ(u3CalibrationInfo * caliInfo)
{
    HANDLE hDevice;
    int localID;

    /* Invoke openUSBConnection function here */
    hDevice = openUSBConnection(-1);
    if (hDevice == 0) {
        printf("Failed to open USB connection\n");
        return 0;
    }

    /* Invoke getCalibrationInfo function here */
    getCalibrationInfo(hDevice, caliInfo);

    return hDevice;
}

int main(int argc, char **argv)
{
    double vRange;
    int i, ret;
    struct sigevent analogSev, digitalSev;
    timer_t analogTimerid, digitalTimerid;
    struct itimerspec its;
    long long freq_ns;
    double freq_hz;
    struct sigaction sa;
    char genAnalogWave, genDigitalWave;

    /* Invoke init_DAQ and handle errors if needed */
    hDevice = init_DAQ(&caliInfo);
    if (hDevice == 0) {
        printf("Failed to initialize DAQ\n");
        return EXIT_FAILURE;
    }

    /* Provide prompt to the user for a voltage range between 0
     * and 5 V. Require a new set of inputs if an invalid range
     * has been entered. */
    i = 0;
    while (i < MAX_TRIES) {
        printf("Enter minimum voltage (between 0 and 5 V): ");
        ret = scanf("%lf", &vMin);
        if (ret != 1 || vMin < 0 || vMin > 5) {
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
        printf("Enter maximum voltage (between %lf and 5 V): ", vMin);
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
    struct timespec res;
    clock_getres(CLOCK_REALTIME, &res);
    freq_ns = 1000000000LL / res.tv_nsec;  // 將紳秒轉換為納秒
    printf("Maximum theoretical square wave frequency: %.3f Hz\n", (double)freq_ns);

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

    /* Setup timer and signal handler for analog square wave */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = analogHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        printf("Failed to set up signal handler for analog wave\n");
        return EXIT_FAILURE;
    }

    analogSev.sigev_notify = SIGEV_SIGNAL;
    analogSev.sigev_signo = SIGRTMIN;
    analogSev.sigev_value.sival_ptr = &analogTimerid;
    timer_create(CLOCK_REALTIME, &analogSev, &analogTimerid);

    /* Setup timer and signal handler for digital square wave */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = digitalHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMAX, &sa, NULL) == -1) {
        printf("Failed to set up signal handler for digital wave\n");
        return EXIT_FAILURE;
    }

    digitalSev.sigev_notify = SIGEV_SIGNAL;
    digitalSev.sigev_signo = SIGRTMAX;
    digitalSev.sigev_value.sival_ptr = &digitalTimerid;
    timer_create(CLOCK_REALTIME, &digitalSev, &digitalTimerid);

    /* Configure timers based on frequency */
    freq_ns = freq_hz * 1000000000LL;
    its.it_value.tv_sec = freq_ns / 1000000000LL;
    its.it_value.tv_nsec = freq_ns % 1000000000LL;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if (genAnalogWave == 'y') {
        timer_settime(analogTimerid, 0, &its, NULL);
    }

    if (genDigitalWave == 'y') {
        timer_settime(digitalTimerid, 0, &its, NULL);
    }

    /* Display a prompt to the user such that if the "exit"
     * command is typed, the USB DAQ is released and the program
     * is terminated. */
    while (1) {
        char cmd[10];
        printf("Type 'exit' to quit: ");
        scanf("%s", cmd);
        if (strcmp(cmd, "exit") == 0) {
            if (genAnalogWave == 'y') {
                timer_delete(analogTimerid);
            }
            if (genDigitalWave == 'y') {
                timer_delete(digitalTimerid);
            }
            releaseUSBConnection(hDevice);
            break;
        }
    }

    return EXIT_SUCCESS;
}

void analogHandler(int sig, siginfo_t *si, void *uc)
{
    static int state = 0;
    double voltage;

    voltage = state ? vMax : vMin;
    binaryToCalibratedAnalogSample(hDevice, 0, caliInfo, voltage);
    state = !state;
}

void digitalHandler(int sig, siginfo_t *si, void *uc)
{
    static int state = 0;

    state = !state;
    putDigitalBitD(hDevice, 2, caliInfo, state);
}
