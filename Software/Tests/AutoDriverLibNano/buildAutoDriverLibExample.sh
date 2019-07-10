gcc -c -std=c99 -o obj/jetsonGPIO.o jetsonGPIO.c
g++ -c -std=c++0x -o obj/linux_spi.o linux_spi.cpp
g++ -c -std=c++0x -o obj/SparkFunAutoDriver.o SparkFunAutoDriver.cpp
g++ -c -std=c++0x -o obj/SparkFunAutoDriverCommands.o SparkFunAutoDriverCommands.cpp
g++ -c -std=c++0x -o obj/SparkFunAutoDriverConfig.o SparkFunAutoDriverConfig.cpp
g++ -c -std=c++0x -o obj/SparkFunAutoDriverSupport.o SparkFunAutoDriverSupport.cpp
g++ -c -std=c++0x -o obj/main.o main.cpp
g++ -o autoDriverLibTest obj/jetsonGPIO.o obj/linux_spi.o obj/SparkFunAutoDriver.o obj/SparkFunAutoDriverCommands.o obj/SparkFunAutoDriverConfig.o obj/SparkFunAutoDriverSupport.o obj/main.o
