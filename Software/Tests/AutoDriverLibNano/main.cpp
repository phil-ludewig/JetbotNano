#include <iostream>
#include <vector>
#include "SparkFunAutoDriver.h"
#include "jetsonGPIO.c"
#include "linux_spi.h"

using namespace std;

AutoDriver boardA(0, gpio19, gpio200);
Linux_SPI spi_dev("/dev/spidev0.0");

void spiInit()
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

  boardA.SPIPortConnect(&spi_dev);
  //boardB.SPIPortConnect(&spi_dev);

  boardA.configSyncPin(BUSY_PIN, 0);        // Busy pin low during operation
  boardA.configStepMode(STEP_FS);           // Step mode; FS disables microstepping for any microstepping functions!
  boardA.setMaxSpeed(1000);                 // [steps/s]
  boardA.setMinSpeed(100);                  // [steps/s] autom. 0, if setFullSpeed() is used
  boardA.setFullSpeed(10000);               // [steps/s] use smooth microsteps below this value (disabled if step mode = FS)
  boardA.setAcc(1);                         // [steps/s/s] no acc. curve above max acc. (=29802)
  boardA.setDec(1);
  boardA.setSlewRate(SR_530V_us);           // 180/290/530; higher value increases torque at higher speeds -> more elm-interference
  boardA.setOCThreshold(OC_750mA);          // OC threshold 750mA
  boardA.setOCShutdown(OC_SD_ENABLE);       // Shutdown driver and motor on overcurrent
  boardA.setPWMFreq(PWM_DIV_2, PWM_MUL_2);  // 31.25kHz PWM freq
  boardA.setVoltageComp(VS_COMP_DISABLE);   // Compensates fluctuations in input voltage for more consistent motor behavior
  boardA.setSwitchMode(SW_HARD_STOP);       // Reaction to optional switch input
  boardA.setOscMode(INT_16MHZ_OSCOUT_16MHZ);// Pass internal clock from board1 to board2 for synchronized movements (for board2: EXT_16MHZ_OSCOUT_INVERT)
  boardA.setAccKVAL(200);                   // Current scaling [0-255] = (0-100%)
  boardA.setDecKVAL(200);
  boardA.setRunKVAL(200);
  boardA.setHoldKVAL(1);                    // Motor holding current (keep low)


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
    spiInit();
    dSPINConfig();

    //cout << "Execute Move Command" << endl;
    boardA.move(FWD, 200);

    //cout << "Clean Up" << endl;

    gpioUnexport(resetPin);
    spi_dev->dev_close();

    cout << "Done." << endl;

    return 0;
}
