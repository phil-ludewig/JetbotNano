// Support functions.

#define NOTE_DIVISOR 2  // My cheesy way of reducing the note frequencies to a range
                        //  that doesn't cause the motor to slip. I *could* rewrite
                        //  the wantYouGone() function to change the notes, but that
                        //  would be a lot of work.

int stepDir = 1;        // Direction flipping bit. Rather than all going one way,
                        //  they change directions. It looks cooler.

// To play a note, we start the motor spinning at the note's frequency in steps/s.
//  The run() function automagically calculates the appropriate value to feed to the
//  dSPIN part based on the desired steps/s.
void playNote(int note, int duration)
{
  if (stepDir == 1)  boardA.run(FWD, note/NOTE_DIVISOR);
  else               boardA.run(REV, note/NOTE_DIVISOR);
  delay(duration);
  stepDir*=-1;
  boardA.softStop();
  while (boardA.busyCheck());
}

// This is the configuration function for the two dSPIN parts. Read the inline
//  comments for more info.
void dSPINConfig(void)
{
  boardA.SPIPortConnect(&SPI);      // Before doing anything else, we need to
                                    //  tell the object which SPI port to use.
                                    //  Some devices may have more than one.
  // My setup
  boardA.configSyncPin(BUSY_PIN, 0);
  boardA.configStepMode(STEP_FS);
  boardA.setMaxSpeed(500);
  boardA.setFullSpeed(5000);
  boardA.setMinSpeed(300);
  boardA.setAcc(400);
  boardA.setDec(400);
  boardA.setSlewRate(SR_530V_us);
  boardA.setOCThreshold(OC_750mA);
  boardA.setPWMFreq(PWM_DIV_2, PWM_MUL_2);
  boardA.setOCShutdown(OC_SD_ENABLE);
  boardA.setVoltageComp(VS_COMP_DISABLE);
  boardA.setSwitchMode(SW_HARD_STOP);
  boardA.setOscMode(INT_16MHZ_OSCOUT_16MHZ);
  boardA.setAccKVAL(200);
  boardA.setDecKVAL(200);
  boardA.setRunKVAL(200);
  boardA.setHoldKVAL(32);
}
