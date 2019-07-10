gcc -c -std=c99 -o jetsonGPIO.o jetsonGPIO.c
g++ -c -std=c++0x -o linux_spi.o linux_spi.cpp
g++ -c -std=c++0x -o main.o main.cpp
g++ -o spiTest jetsonGPIO.o linux_spi.o main.o

