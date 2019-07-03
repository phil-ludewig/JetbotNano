// exampleApp.c

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

/*
int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    // set the terminal to raw mode
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    // read a character from the stdin stream without blocking
    //   returns EOF (-1) if no character is available
    character = fgetc(stdin);

    // restore the original terminal attributes
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}
*/


int main(int argc, char *argv[]){

    cout << "Testing the GPIO Pins" << endl;

    jetsonNanoGPIONumber redLED = gpio79 ;     // Ouput
    // Make led available in user space
    gpioExport(redLED) ;
    gpioSetDirection(redLED,outputPin) ;

    // Flash the LED
    for(int i=0; i<100; i++){
        cout << "Setting the LED on" << endl;
        gpioSetValue(redLED, on);
        usleep(1000000);         // on for 1000ms
        cout << "Setting the LED off" << endl;
        gpioSetValue(redLED, off);
        usleep(1000000);         // off for 1000ms
    }

    cout << "GPIO example finished." << endl;
    gpioUnexport(redLED);     // unexport the LED
    return 0;
}
