#include <iostream>
#include <vector>
#include "SparkFunAutoDriver.h"
#include "jetsonGPIO.c"
#include "linux_spi.h"

using namespace std;

AutoDriver boardA(0, gpio19, gpio200);
Linux_SPI spi_dev("/dev/spidev0.0");

void spiConfig(){

  if( _SPI->dev_open() != 0 )
    {
        printf("Error: %s\n", _SPI->strerror(_SPI->get_errno()));
        exit(-1);
    }


    if( _SPI->set_mode(SPI_MODE_3) != 0 )
    {
        printf("Error: %s\n", _SPI->strerror(_SPI->get_errno()));
        exit(-1);
    }


    if( _SPI->set_bits_per_word(8) != 0 )//How Many Bits Per Word?
    {
        printf("Error: %s\n", _SPI->strerror(_SPI->get_errno()));
        exit(-1);
    }


    if( _SPI->set_max_speed_hz(3900000) != 0 )// MaxSpeed: 4000000?
    {
        printf("Error: %s\n", _SPI->strerror(_SPI->get_errno()));
        exit(-1);
    }

}

void dSPINConfig()
{
  boardA.SPIPortConnect(&spi_dev);      // Before doing anything else, we need to
  //boardB.SPIPortConnect(&SPI);

  boardA.configSyncPin(BUSY_PIN, 0);// BUSY pin low during operations;
                                    //  second paramter ignored.
  boardA.configStepMode(STEP_FS);   // 0 microsteps per step
  boardA.setMaxSpeed(1000);        // 10000 steps/s max
  boardA.setFullSpeed(10000);       // microstep below 10000 steps/s
  boardA.setAcc(1);             // accelerate at 10000 steps/s/s
  boardA.setDec(1);
  boardA.setSlewRate(SR_530V_us);   // Upping the edge speed increases torque.
  boardA.setOCThreshold(OC_750mA);  // OC threshold 750mA
  boardA.setPWMFreq(PWM_DIV_2, PWM_MUL_2); // 31.25kHz PWM freq
  boardA.setOCShutdown(OC_SD_ENABLE); // don't shutdown on OC
  boardA.setVoltageComp(VS_COMP_DISABLE); // don't compensate for motor V
  boardA.setSwitchMode(SW_HARD_STOP);    // Switch is not hard stop
  //boardA.setOscMode(EXT_16MHZ_OSCOUT_INVERT); // for boardA, we want 16MHz
  boardA.setOscMode(INT_16MHZ_OSCOUT_16MHZ);


  boardA.setAccKVAL(200);
  boardA.setDecKVAL(200);
  boardA.setRunKVAL(200);
  boardA.setHoldKVAL(32);


  // boardB.configSyncPin(BUSY_PIN, 0);// BUSY pin low during operations;
  //                                   //  second paramter ignored.
  // boardB.configStepMode(STEP_FS_64);   // 0 microsteps per step
  // boardB.setMaxSpeed(1000);        // 10000 steps/s max
  // boardB.setFullSpeed(10000);       // microstep below 10000 steps/s
  // boardB.setAcc(1);             // accelerate at 10000 steps/s/s
  // boardB.setDec(1);
  // boardB.setSlewRate(SR_530V_us);   // Upping the edge speed increases torque.
  // boardB.setOCThreshold(OC_750mA);  // OC threshold 750mA
  // boardB.setPWMFreq(PWM_DIV_2, PWM_MUL_2); // 31.25kHz PWM freq
  // boardB.setOCShutdown(OC_SD_ENABLE); // don't shutdown on OC
  // boardB.setVoltageComp(VS_COMP_DISABLE); // don't compensate for motor V
  // boardB.setSwitchMode(SW_HARD_STOP);    // Switch is not hard stop
  // //boardA.setOscMode(EXT_16MHZ_OSCOUT_INVERT); // for boardA, we want 16MHz
  // boardB.setOscMode(INT_16MHZ_OSCOUT_16MHZ);
  //
  //
  // boardB.setAccKVAL(200);
  // boardB.setDecKVAL(200);
  // boardB.setRunKVAL(200);
  // boardB.setHoldKVAL(32);

}

int main()
{
    cout << "Configuring GPIO Pins" << endl;

    jetsonGPIO resetPin = gpio200 ;
    gpioExport(resetPin) ;
    gpioSetDirection(resetPin, outputPin) ;
    gpioSetValue(resetPin, off);
    gpioSetValue(resetPin, on);

    cout << "Configure Autodriver Board" << endl;
    dSPINConfig();

    cout << "Execute Move Command" << endl;
    boardA.move(FWD, 200);

    cout << "Clean Up" << endl;

    gpioUnexport(resetPin);
    _SPI->dev_close();

    return 0;
}
