// Blink example for LED on J41 Pin 12 (=gpio79)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include "jetsonGPIO.h"
using namespace std;


int main(int argc, char *argv[]){

    cout << "Blink started." << endl;

    jetsonNanoGPIONumber ledPin = gpio79;
    gpioExport(ledPin); // make available in user space
    gpioSetDirection(ledPin,outputPin);

    for(int i=0; i<100; i++){
        cout << "Setting the LED on" << endl;
        gpioSetValue(ledPin, on);
        usleep(1000000); // 1000ms
        cout << "Setting the LED off" << endl;
        gpioSetValue(ledPin, off);
        usleep(1000000);
    }

    cout << "GPIO example finished." << endl;
    gpioUnexport(ledPin);     // unexport the LED
    return 0;
}
