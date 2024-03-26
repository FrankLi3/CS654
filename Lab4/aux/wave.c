#include "u3.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

/* Signal handler for SIGRTMIN */
void handle_sigrtmin(int sig) {
    // Code to toggle digital output on FIO2
}

/* Additional functions you might need to declare */
// ...

HANDLE init_DAQ(u3CalibrationInfo *caliInfo) {
    HANDLE hDevice;
    int localID = -1; // typically default for first found device

    if ((hDevice = openUSBConnection(localID)) == NULL) {
        fprintf(stderr, "Could not open USB connection\n");
        exit(EXIT_FAILURE);
    }

    if (getCalibrationInfo(hDevice, caliInfo) < 0) {
        fprintf(stderr, "Could not get calibration information\n");
        closeUSBConnection(hDevice);
        exit(EXIT_FAILURE);
    }

    return hDevice;
}

int main(int argc, char **argv) {
    u3CalibrationInfo caliInfo;
    HANDLE hDevice;

    // Setup signal handler for SIGRTMIN
    struct sigaction sa;
    sa.sa_handler = &handle_sigrtmin;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Initialize the DAQ
    hDevice = init_DAQ(&caliInfo);

    float voltage_low, voltage_high;
    // Provide prompt to the user for a voltage range and check validity
    do {
        printf("Enter voltage range (0 to 5V), format low high: ");
        scanf("%f %f", &voltage_low, &voltage_high);
    } while (voltage_low < 0 || voltage_high > 5 || voltage_low >= voltage_high);

    // Compute and display the maximum square wave frequency
    // ...

    float desired_frequency;
    // Prompt user for square wave frequency
    printf("Enter desired square wave frequency in Hz: ");
    scanf("%f", &desired_frequency);

    // Program a timer to deliver SIGRTMIN for digital square wave
    // ...

    // Program a second timer for analog square wave
    // ...

    char command[10];
    // Command prompt loop
    do {
        printf("Enter command: ");
        scanf("%9s", command);
        if (strcmp(command, "exit") == 0) {
            // Release the USB device
            closeUSBConnection(hDevice);
            // Break from loop to allow program to exit
            break;
        }
        // Other command handling
        // ...
    } while (1);

    return EXIT_SUCCESS;
}

