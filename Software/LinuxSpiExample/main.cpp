#include <iostream>
#include <algorithm>
#include "jetsonGPIO.c"
#include "linux_spi.h"

using namespace std;

void init_spi_dev(Linux_SPI *spi_dev)
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


    if( spi_dev->set_bits_per_word(8) != 0 )
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

void test_read_write(Linux_SPI *spi_dev, uint8_t data)
{

    int ret;


    ret = spi_dev->write(&data, sizeof(data));

    printf("send %.02x ret == %d\n", ret);


    ret = spi_dev->read(&data, sizeof(data));

    printf("read ret == %d  val == %d\n", ret, data);

}


int main()
{
    cout << "Configuring GPIO Pins" << endl;

    jetsonGPIO resetPin = gpio200 ;
    gpioExport(resetPin) ;
    gpioSetDirection(resetPin, outputPin) ;
    gpioSetValue(resetPin, off);
    gpioSetValue(resetPin, on);


    cout << "Configuring SPI" << endl;

    Linux_SPI spi_dev("/dev/spidev0.0");
    init_spi_dev(&spi_dev);

    cout << "Send Autodriver Config Buffer" << endl;

    //dSPINConfig command:
    vector<uint8_t> configBuf {0x36,0x00,0x16,0x00,0x36,0x00,0x16,0x00,0x07,0x00,0x21,0x15,0x01,0x47,0x28,0x00,0x00,0x08,0x04,0xEC, 0x05,0x00,0x36,0x06,0x00,0x36,0x38,0x00,0x00,0x18,0x3,0x00,0x13,0x01,0x38,0x00,0x00,0x18,0x3C,0x00,0x38,0x00,0x00,0x18,0x00,0x80,0x38,0x00,0x00,0x18,0x00,0x00,0x38,0x00,0x00,0x18,0x00,0x00,0x38,0x00,0x00,0x18,0x00,0x0B,0x0B,0xC8,0x0C,0xC8,0x0A,0xC8,0x09,0x20};
    for (auto elem : configBuf){
        test_read_write(&spi_dev, elem);
    }

    cout << "Send Move Command" << endl;

    vector<uint8_t> moveCom {0x41, 0x00, 0x00, 0xC8};
    for (auto elem : moveCom){
        test_read_write(&spi_dev, elem);
    }

    cout << "SPI/GPIO Clean Up" << endl;
    spi_dev.dev_close();
    gpioUnexport(resetPin);

    return 0;
}
