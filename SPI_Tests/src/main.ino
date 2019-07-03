#include <SparkFunAutoDriver.h>
#include <SPI.h>

// Create our AutoDriver instance. The parameters are the position in the chain of
//  boards (with board 0 being located at the end of the chain, farthest from the
//  controlling processor), CS pin, and reset pin.
// Arduino pinout (to Board):
// DIGITAL 13 (to SCK), MISO 12 (to SD to prevBD), MOSI 11 (to SD from prevBD),
// DIGTIAL 10 (to CS), DIGITAL 8 (to BUSY), DIGITAL 7 (to RESET)
AutoDriver boardA(0, 10, 7);

void setup()
{
  Serial.begin(9600);
  // Start by setting up the SPI port and pins. The
  //  Autodriver library does not do this for you!
  pinMode(7, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  //digitalWrite(10, HIGH);
  digitalWrite(7, LOW);       // This low/high is a reset of the L6470 chip on the
  digitalWrite(7, HIGH);      //  Autodriver board, and is a good thing to do at
                              //  the start of any Autodriver sketch, to be sure
                              //  you're starting the Autodriver from a known state.
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  Serial.println("dSPINConfig start:");
  //dSPINConfig();
  //Serial.print("dSPINConfig end.");
  delay(2000);
  boardA.move(FWD,200);
  delay(1000);
  Serial.println("Setup done.");

}

// loop() waits for a character- any character- and then plays the song.
void loop()
{

  //boardA.goTo(100);
  //Serial.println();
  //Serial.println();
  //delay(5000);


}
