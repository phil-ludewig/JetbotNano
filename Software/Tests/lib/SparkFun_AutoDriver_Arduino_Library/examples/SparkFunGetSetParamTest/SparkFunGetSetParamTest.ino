#include <SparkFunAutoDriver.h>
#include <SPI.h>

/* Test sketch that just gets and sets values on a L6470 AutoDriver
 *
 * This is mainly a test to make sure that getting a value after setting returns
 * the same result. This is a useful test that should be run whenever the library is changed.
 */

AutoDriver board(0, 10, 6);
String name = "";
unsigned long temp;
boolean tempBool;
byte tempByte;
float tempFloat;
int tempInt;
int tempInt2;
boolean pass = true;

void setup()
{
  Serial.begin(9600);
  // Start by setting up the pins and the SPI peripheral.
  //  The library doesn't do this for you!
  pinMode(6, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(6, HIGH);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);

  // Tell the Autodriver object which SPI port you want to use. Some
  //  devices may have more than one.
  board.SPIPortConnect(&SPI);

  // first check  board config register, should be 0x2E88 on bootup
  temp = board.getParam(CONFIG);
  Serial.print("Config reg value: ");
  Serial.println(temp, HEX);

  // Now check the status of the board. Should be 0x7c03
  temp = board.getStatus();
  Serial.print("Status reg value: ");
  Serial.println(temp, HEX);

  // set and get all configuration values here to make sure the
  //  conversions are working and comms are up properly.
  name = "LoSpdOpt";
  tempBool = board.getLoSpdOpt();
  pv(tempBool);
  tempBool = ~tempBool;
  board.setLoSpdOpt(tempBool);
  test(tempBool, board.getLoSpdOpt());

  name = "MinSpeed";
  tempFloat = board.getMinSpeed();
  pv(tempFloat);
  // be careful about rounding
  tempFloat = (tempFloat == 23.8418788909) ? 47.6837577818 : 23.8418788909;
  board.setMinSpeed(tempFloat);
  test(tempFloat, board.getMinSpeed());

  name = "StepMode";
  tempByte = board.getStepMode();
  pv(tempByte);
  tempByte = (tempByte == 0) ? 1 : 0;
  board.configStepMode(tempByte);
  test(tempByte, board.getStepMode());

  name = "MaxSpeed";
  tempFloat = board.getMaxSpeed();
  pv(tempFloat);
  // be careful about rounding
  tempFloat = (tempFloat == 152.587890625) ? 305.17578125 : 152.587890625;
  board.setMaxSpeed(tempFloat);
  test(tempFloat, board.getMaxSpeed());

  name = "FullSpeed";
  tempFloat = board.getFullSpeed();
  pv(tempFloat);
  // be careful about rounding
  tempFloat = (tempFloat == 160.21728515625) ? 312.80517578125 : 160.21728515625;
  board.setFullSpeed(tempFloat);
  test(tempFloat, board.getFullSpeed());

  name = "Acc";
  tempFloat = board.getAcc();
  pv(tempFloat);
  // be careful about rounding
  tempFloat = (tempFloat == 72.76008090920998) ? 145.52016181841995 : 72.76008090920998;
  board.setAcc(tempFloat);
  test(tempFloat, board.getAcc());

  name = "Dec";
  tempFloat = board.getDec();
  pv(tempFloat);
  // be careful about rounding
  tempFloat = (tempFloat == 72.76008090920998) ? 145.52016181841995 : 72.76008090920998;
  board.setDec(tempFloat);
  test(tempFloat, board.getDec());

  name = "OCThreshold";
  tempByte = board.getOCThreshold();
  pv(tempByte);
  tempByte = (tempByte == OC_375mA) ? OC_750mA : OC_375mA;
  board.setOCThreshold(tempByte);
  test(tempByte, board.getOCThreshold());

  name = "PWMFreqDivisor";
  tempInt = board.getPWMFreqDivisor();
  tempInt2 = board.getPWMFreqMultiplier();
  pv(tempInt);
  tempInt = (tempInt == PWM_DIV_1) ? PWM_DIV_2 : PWM_DIV_1;
  board.setPWMFreq(tempInt, tempInt2);
  test(tempInt, board.getPWMFreqDivisor());

  name = "PWMFreqMultiplier";
  pv(tempInt2);
  tempInt2 = (tempInt2 == PWM_MUL_1) ? PWM_MUL_2 : PWM_MUL_1;
  board.setPWMFreq(tempInt, tempInt2);
  test(tempInt2, board.getPWMFreqMultiplier());

  name = "SlewRate";
  tempInt = board.getSlewRate();
  pv(tempInt);
  tempInt = (tempInt == SR_180V_us) ? SR_290V_us : SR_180V_us;
  board.setSlewRate(tempInt);
  test(tempInt, board.getSlewRate());

  name = "OCShutdown";
  tempInt = board.getOCShutdown();
  pv(tempInt);
  tempInt = (tempInt == OC_SD_ENABLE) ? OC_SD_DISABLE : OC_SD_ENABLE;
  board.setOCShutdown(tempInt);
  test(tempInt, board.getOCShutdown());

  name = "VoltageComp";
  tempInt = board.getVoltageComp();
  pv(tempInt);
  tempInt = (tempInt == VS_COMP_ENABLE) ? VS_COMP_DISABLE : VS_COMP_ENABLE;
  board.setVoltageComp(tempInt);
  test(tempInt, board.getVoltageComp());

  name = "SwitchMode";
  tempInt = board.getSwitchMode();
  pv(tempInt);
  tempInt = (tempInt == SW_USER) ? VS_COMP_DISABLE : SW_HARD_STOP;
  board.setSwitchMode(tempInt);
  test(tempInt, board.getSwitchMode());

  name = "OscMode";
  tempInt = board.getOscMode();
  pv(tempInt);
  tempInt = (tempInt == INT_16MHZ) ? INT_16MHZ_OSCOUT_2MHZ : INT_16MHZ;
  board.setOscMode(tempInt);
  test(tempInt, board.getOscMode());

  name = "AccK";
  tempByte = board.getAccKVAL();
  pv(tempByte);
  tempByte = (tempByte == 0) ? 1 : 0;
  board.setAccKVAL(tempByte);
  test(tempByte, board.getAccKVAL());

  name = "DecK";
  tempByte = board.getDecKVAL();
  pv(tempByte);
  tempByte = (tempByte == 0) ? 1 : 0;
  board.setDecKVAL(tempByte);
  test(tempByte, board.getDecKVAL());

  name = "RunK";
  tempByte = board.getRunKVAL();
  pv(tempByte);
  tempByte = (tempByte == 0) ? 1 : 0;
  board.setRunKVAL(tempByte);
  test(tempByte, board.getRunKVAL());

  name = "HoldK";
  tempByte = board.getHoldKVAL();
  pv(tempByte);
  tempByte = (tempByte == 0) ? 1 : 0;
  board.setHoldKVAL(tempByte);
  test(tempByte, board.getHoldKVAL());

  Serial.print("Passed? ");
  Serial.println(pass);
}

void loop()
{
  // do nothing
}

void pv(float v)
{
  Serial.print(name + " ");
  Serial.println(v, DEC);
}

void pv(bool v)
{
  Serial.print(name + " ");
  Serial.println(v);
}

void pv(byte v)
{
  Serial.print(name + " ");
  Serial.println(v, DEC);
}

void pv(unsigned long v)
{
  Serial.print(name + " ");
  Serial.println(v, DEC);
}

void pv(int v)
{
  Serial.print(name + " ");
  Serial.println(v, DEC);
}

void test(float v1, float v2)
{
  if (abs(v1-v2) > 0.1)
  {
    Serial.println("!!! " + name + " failed");
    Serial.print("Expected ");
    Serial.println(v1, DEC);
    Serial.print("Got ");
    Serial.println(v2, DEC);
    pass = false;
  }
  else
  {
    Serial.println(name + " passed r/w test!");
  }
}

void test(int v1, int v2)
{
  if (v1 != v2)
  {
    Serial.println("!!! " + name + " failed");
    Serial.print("Expected ");
    Serial.println(v1, DEC);
    Serial.print("Got ");
    Serial.println(v2, DEC);
    pass = false;
  }
  else
  {
    Serial.println(name + " passed r/w test!");
  }
}

void test(bool v1, bool v2)
{
  if (v1 != v2)
  {
    Serial.println("!!! " + name + " failed");
    Serial.print("Expected ");
    Serial.println(v1);
    Serial.print("Got ");
    Serial.println(v2);
    pass = false;
  }
  else
  {
    Serial.println(name + " passed r/w test!");
  }
}
