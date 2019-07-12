#include <iostream>
#include <vector>
#include "SparkFunAutoDriver.h"
#include "jetsonGPIO.c"
#include "linux_spi.h"
#include <unistd.h> // for time

using namespace std;

AutoDriver rightMotor(1, gpio19, gpio200); // right wheel (REV = forward movement)
AutoDriver leftMotor(0, gpio19, gpio200); // left wheel (FWD = forward movement)
Linux_SPI spi_dev("/dev/spidev0.0");

#define WHEELBASE 23.44   // [cm]
#define WHEELDIAM 9.60    // [cm]
#define STEPSPERREV 200
#define PI 3.14159265

void spiInit(Linux_SPI *spi_dev)
{

  if( spi_dev->dev_open() != 0 )
    {
        printf("Error: %s\n", spi_dev->strerror(spi_dev->get_errno()));
        exit(-1);
    }


    if( spi_dev->set_mode(SPI_MODE_3) != 0 )
    {
        printf("Error: %s\n", spi_dev->strerror(spi_dev->get_errno()));
        exit(-1);
    }


    if( spi_dev->set_bits_per_word(8) != 0 )//How Many Bits Per Word?
    {
        printf("Error: %s\n", spi_dev->strerror(spi_dev->get_errno()));
        exit(-1);
    }


    if( spi_dev->set_max_speed_hz(3900000) != 0 )
    {
        printf("Error: %s\n", spi_dev->strerror(spi_dev->get_errno()));
        exit(-1);
    }

}

void dSPINConfig()
{
  // Config driver parameters
  // For more info on parameters:
  // https://learn.sparkfun.com/tutorials/getting-started-with-the-autodriver---v13?_ga=2.186722024.1189833582.1562587519-1543984250.1560589149

  rightMotor.SPIPortConnect(&spi_dev);

  rightMotor.configSyncPin(BUSY_PIN, 0);        // Busy pin low during operation
  rightMotor.configStepMode(STEP_FS_128);       // Step mode; FS disables microstepping for any microstepping functions!
  rightMotor.setMaxSpeed(380);                  // [steps/s] (stalling above 380 with current motor)
  rightMotor.setMinSpeed(1);                    // [steps/s] autom. 0, if setFullSpeed() is used
  rightMotor.setFullSpeed(5000);                // [steps/s] use smooth microsteps below this value (disabled if step mode = FS)
  rightMotor.setAcc(200);                       // [steps/s/s] no acc. curve above max acc. (=29802)
  rightMotor.setDec(200);
  rightMotor.setSlewRate(SR_530V_us);           // 180/290/530; higher value increases torque at higher speeds -> more elm-interference
  rightMotor.setOCThreshold(OC_750mA);          // OC threshold 750mA
  rightMotor.setOCShutdown(OC_SD_ENABLE);       // Shutdown driver and motor on overcurrent
  rightMotor.setPWMFreq(PWM_DIV_2, PWM_MUL_2);  // 31.25kHz PWM freq
  rightMotor.setVoltageComp(VS_COMP_DISABLE);   // Compensates fluctuations in input voltage for more consistent motor behavior
  rightMotor.setSwitchMode(SW_HARD_STOP);       // Reaction to optional switch input
  rightMotor.setOscMode(INT_16MHZ_OSCOUT_16MHZ);// Pass internal clock from board1 to board2 for synchronized movements (for board2: EXT_16MHZ_OSCOUT_INVERT)
  rightMotor.setAccKVAL(255);                   // Current scaling [0-255] = (0-100%)
  rightMotor.setDecKVAL(255);
  rightMotor.setRunKVAL(255);
  rightMotor.setHoldKVAL(30);                    // Motor holding current (keep low)


  leftMotor.SPIPortConnect(&spi_dev);
  leftMotor.configSyncPin(BUSY_PIN, 0);        // Busy pin low during operation
  leftMotor.configStepMode(STEP_FS_128);       // Step mode; FS disables microstepping for any microstepping functions!
  leftMotor.setMaxSpeed(380);                  // [steps/s] (stalling above 380 with current motor)
  leftMotor.setMinSpeed(1);                    // [steps/s] autom. 0, if setFullSpeed() is used
  leftMotor.setFullSpeed(5000);                // [steps/s] use smooth microsteps below this value (disabled if step mode = FS)
  leftMotor.setAcc(200);                       // [steps/s/s] no acc. curve above max acc. (=29802)
  leftMotor.setDec(200);
  leftMotor.setSlewRate(SR_530V_us);           // 180/290/530; higher value increases torque at higher speeds -> more elm-interference
  leftMotor.setOCThreshold(OC_750mA);          // OC threshold 750mA
  leftMotor.setOCShutdown(OC_SD_ENABLE);       // Shutdown driver and motor on overcurrent
  leftMotor.setPWMFreq(PWM_DIV_2, PWM_MUL_2);  // 31.25kHz PWM freq
  leftMotor.setVoltageComp(VS_COMP_DISABLE);   // Compensates fluctuations in input voltage for more consistent motor behavior
  leftMotor.setSwitchMode(SW_HARD_STOP);       // Reaction to optional switch input
  leftMotor.setOscMode(EXT_16MHZ_OSCOUT_INVERT);// Pass internal clock from board1 to board2 for synchronized movements (for board2: EXT_16MHZ_OSCOUT_INVERT)
  leftMotor.setAccKVAL(255);                   // Current scaling [0-255] = (0-100%)
  leftMotor.setDecKVAL(255);
  leftMotor.setRunKVAL(255);
  leftMotor.setHoldKVAL(30);

}

void cleanup(Linux_SPI *spi_dev)
{
    spi_dev->dev_close();
}

void turn(float degrees)
{
    // positive stepcount is clockwise rotation
    float distPerStep = WHEELDIAM*PI / (STEPSPERREV*128);
    float distToDrive = (degrees / 360) * (WHEELBASE*PI);
    int steps = distToDrive / distPerStep;

    if(steps >= 0)  // clockwise rotation
    {
      rightMotor.move(FWD, steps);
      leftMotor.move(FWD, steps);
    }
    else
    {
      rightMotor.move(REV, steps);
      leftMotor.move(REV, steps);
    }

}

int main()
{
    //cout << "Configuring GPIO Pins" << endl;

    jetsonGPIO resetPin = gpio200 ;
    gpioExport(resetPin) ;
    gpioSetDirection(resetPin, outputPin) ;

    gpioSetValue(resetPin, off); // Reset autodriver
    gpioSetValue(resetPin, on);

    //cout << "Configure Autodriver Board" << endl;
    spiInit(&spi_dev);
    dSPINConfig();

    //cout << "Execute Move Command" << endl;
    //rightMotor.move(REV, 200*128);
    //leftMotor.move(FWD, 200*128);

    turn(90);
    usleep(4000*1000); // [microseconds]

    turn(-90);
    //cout << "Clean Up" << endl;

    cleanup(&spi_dev); // close gpio and spi
    gpioUnexport(resetPin);

    cout << "Done." << endl;

    return 0;
}
