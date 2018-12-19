/*
  "LaserMice" by So KANNO.
*/
// libraries
#include <IRremote.h>
#include <EEPROM.h>

#include "pinAssign.h"

//tab
// IRrecv
IRrecv irrecv(RECV_PIN);
decode_results results;

int myId;
/*
  ID will be defined by external ESP-01.
  When ESP get the IP Address, it send its own IP
  Then 4th digit become robot's ID.
*/
int identifier; 
int id; // 8bit [0~255]
int op; // 2bit [0~4] (==, !=, <, >)
int address; // 3bit [0~7]
unsigned int value; // 16bit [0~65535]
boolean correspond = false;
const int sonyRemote = 0;
const int irCommander = 1;
boolean irFunction; // sonyRemote or irCommander
unsigned long timeStamp_MTR;
boolean reaction_MTR = false;
int reactionLength_MTR = 500;
boolean irCommandFlag = false;
boolean irNotification = false;
int blinkCount;
int countFlag = true;
unsigned long ignoreThis = 4294967295;

// EEPROM
boolean readFromEEPROM = false;
int cycleLength_add_0 = 0;
int cycleLength_add_1 = 1;
int laser_reactionLength_ratio_add = 2;
int motorSpeed_add = 3;
int threshold_add = 4;
int reactionLength_SLND_add = 5;

//////parameters/////
unsigned int cycleLength = 2000; //
float laser_reactionLength_ratio = 0.1;
int motorSpeed = 150;
int threshold = 25; // used to be 3
/*
  memo:
  3rd board prefer higher like 15-20
  4th board prefer lower like 7
*/

boolean playFlag = false;
boolean lastPlayFlag = false;
boolean bangFlag = false;
boolean bangFlag_LSR = false;
boolean bangFlag_SLND = false;
unsigned long shiftAmount = 0;
boolean turnDirection = false; // true is left, false is left
boolean turnFlag = false;
// boolean turnRightFlag = false;
// boolean turnLeftFlag = false;
const int debounce = 4;
boolean rotateReactFlag = false;
boolean tailLED = true;
boolean irReceive = true;
boolean keepLaser = true;

// LASER DETECTION
// buffer for mean filter
const int BUFFER_LENGTH = 60;
int buffer0[BUFFER_LENGTH];
int buffer1[BUFFER_LENGTH];
int buffer2[BUFFER_LENGTH];
int index = 0;
boolean threeSensorsMode = false;

// for bangFlag
unsigned long timeStamp_LSR;
// boolean bangFlag= false;
unsigned int reactionLength_LSR = cycleLength * laser_reactionLength_ratio;
unsigned int reactionLength_SLND = 20;

// bumper function
boolean bumpLstate;
boolean bumpRstate;
boolean lastBumpLstate;
boolean lastBumpRstate;
boolean bumpLreactFlag = false;
boolean bumpRreactFlag = false;
unsigned long timeStamp_BMP;
long BMPconstant = 180000;
//id 1 ~ 5 has various gear ratio. should have some different BMPconstant
long reactionLength_BMP = BMPconstant / motorSpeed;
long retreatConstant = 76500;
int retreatLength = retreatConstant / motorSpeed;

// sequence functions
boolean sequenceMode = false;
unsigned int sequence;
boolean beat[16];
int loopDigit = 16;
int seqCount = 0;
boolean seqCountFlag = true;

boolean followLightFlag = false;

//for database
int strikeCount = 0;
int beShotCount = 0;
boolean detectReport = false;
boolean commandReaction = true;

void setup() {
//  if (id == 5) BMPconstant = 100000;
  Serial.begin(115200); // for debug
  Serial1.begin(115200); // for communication with ESP-01
  Serial1.write(3);
  pinMode(MTR_A_F, OUTPUT);
  pinMode(MTR_A_B, OUTPUT);
  pinMode(MTR_A_PWM, OUTPUT);
  pinMode(MTR_B_F, OUTPUT);
  pinMode(MTR_B_B, OUTPUT);
  pinMode(MTR_B_PWM, OUTPUT);
  pinMode(SOLENOID, OUTPUT);
  pinMode(LASER, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(L_BUMPER, INPUT_PULLUP);
  pinMode(R_BUMPER, INPUT_PULLUP);
  pinMode(CDS, INPUT);
  pinMode(CDS_L, INPUT);
  pinMode(CDS_R, INPUT);
  // pinMode(JUMP_1, INPUT_PULLUP);
  pinMode(BAT_CHK, INPUT);
  pinMode(JUMP, INPUT_PULLUP);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(JUMP, INPUT_PULLUP);
  // prepare mean filter
  for (int i = 0; i < BUFFER_LENGTH; i++) {
    buffer0[i] = analogRead(CDS);
    buffer1[i] = analogRead(CDS_L);
    buffer2[i] = analogRead(CDS_R);
  }

  // EEPROM reading
  if (readFromEEPROM) {
    byte a = EEPROM.read(cycleLength_add_0);
    byte b = EEPROM.read(cycleLength_add_1);
    cycleLength = (a << 8) + b;
    laser_reactionLength_ratio = EEPROM.read(laser_reactionLength_ratio_add) / 100.0;
    motorSpeed = EEPROM.read(motorSpeed_add);
    threshold = EEPROM.read(threshold_add);
    reactionLength_SLND = EEPROM.read(reactionLength_SLND_add);
  }
}

void loop() {
  /*
    note:
    "laserDetect_React()" turns "bangFlag" Booleans on and off.
    and does digitalWrite for Laser and Solenoid.
    "play()" only turns booleans on and off.
  */
  laserDetect_React();
  if (irReceive) irUpdate();
  espUpdate();
  batteryCheck();
  bumperFunction();
  play();
  if (followLightFlag) followLight();
  if (keepLaser) digitalWrite(LASER, HIGH);
}
