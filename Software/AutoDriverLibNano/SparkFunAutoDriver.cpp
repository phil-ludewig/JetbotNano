#include "linux_spi.h"
#include "jetsonGPIO.h"
#include "SparkFunAutoDriver.h"

int AutoDriver::_numBoards;

// Constructors
AutoDriver::AutoDriver(int position, int CSPin, int resetPin, int busyPin)
{
  _CSPin = CSPin;
  _position = position;
  _resetPin = resetPin;
  _busyPin = busyPin;
  _numBoards++;
  //_SPI = &SPI;
}

AutoDriver::AutoDriver(int position, int CSPin, int resetPin)
{
  _CSPin = CSPin;
  _position = position;
  _resetPin = resetPin;
  _busyPin = -1;
  _numBoards++;
  //_SPI = &SPI;
}

void AutoDriver::SPIPortConnect(Linux_SPI *SPIPort)
{
  _SPI = SPIPort;
}

int AutoDriver::busyCheck(void)
{
  unsigned int* digitalPinVal = 0;
  if (_busyPin == -1)
  {
    if (getParam(STATUS) & 0x0002) return 0;
    else                           return 1;
  }
  else
  {
    gpioGetValue(_busyPin, digitalPinVal);
    if (*digitalPinVal == HIGH) return 0;
    else                               return 1;
  }
}
