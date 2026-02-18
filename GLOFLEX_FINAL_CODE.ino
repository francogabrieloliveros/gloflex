#include "Wire.h"

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Speaker------------------------------------------------------------
static const uint8_t PIN_MP3_TX = 10;  // Connects to module's RX
static const uint8_t PIN_MP3_RX = 11;  // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

DFRobotDFPlayerMini player;

//Flex Sensors---------------------------------------------------------
int flex_1 = A0;  //Thumb
int flex_2 = A1;  //Index
int flex_3 = A2;  //Middle
int flex_4 = A3;  //Ring
int flex_5 = A6;  //Pinky

int flex_1_v = 0;
int flex_2_v = 0;
int flex_3_v = 0;
int flex_4_v = 0;
int flex_5_v = 0;

int flex_values[5] = { 0, 0, 0, 0, 0 };

//Accelerometer---------------------------------------------------------
const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;

int handPos = 0;

void setup() {

  Serial.begin(9600);

  //Speaker------------------------------------------------------------
  softwareSerial.begin(9600);

  if (player.begin(softwareSerial)) {
    // Set volume to maximum (0 to 30).
    player.volume(30);
    player.playFolder(1, 43);
  }

  else {
    Serial.println("Connection Failed");
  }

  //Flex Sensors---------------------------------------------------------
  pinMode(flex_1, INPUT);  // thumb
  pinMode(flex_2, INPUT);  // index
  pinMode(flex_3, INPUT);  // middle
  pinMode(flex_4, INPUT);  // ring
  pinMode(flex_5, INPUT);  // pinky

  //Accelerometer---------------------------------------------------------
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {

  //Flex Sensors---------------------------------------------------------
  flex_1_v = analogRead(flex_1);
  flex_2_v = analogRead(flex_2);
  flex_3_v = analogRead(flex_3);
  flex_4_v = analogRead(flex_4);
  flex_5_v = analogRead(flex_5);

  //Array---------------------------------------------------------------
  //done: Thumb
  if (flex_1_v > 24) {
    flex_values[0] = 0;
  } else if (flex_1_v < 21) {
    flex_values[0] = 2;
  } else {
    flex_values[0] = 1;
  }

  //done: Index
  if (flex_2_v > 30) {
    flex_values[1] = 0;
  } else if (flex_2_v < 23) {
    flex_values[1] = 2;
  } else {
    flex_values[1] = 1;
  }

  //done: Middle
  if (flex_3_v > 30) {
    flex_values[2] = 0;
  } else if (flex_3_v < 22) {
    flex_values[2] = 2;
  } else {
    flex_values[2] = 1;
  }

  //done: Ring
  if (flex_4_v > 29) {
    flex_values[3] = 0;
  } else if (flex_4_v < 26) {
    flex_values[3] = 2;
  } else {
    flex_values[3] = 1;
  }

  //done: Pinky
  if (flex_5_v > 26) {
    flex_values[4] = 0;
  } else if (flex_5_v < 22) {
    flex_values[4] = 2;
  } else {
    flex_values[4] = 1;
  }

  //Gyro------------------------------------------------------------
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  if (y < 30) {
    handPos = 0;
  }
  if (y > 30 && y < 300) {
    handPos = 1;
  }
  if (x < 300 && z > 220) {
    handPos = 2;
  }

  //Serials---------------------------------------------------------------
  Serial.print("\tT\tI\tM\tR\tP\tPos\n");
  Serial.print("\t");
  Serial.print(flex_values[0]);
  Serial.print("\t");
  Serial.print(flex_values[1]);
  Serial.print("\t");
  Serial.print(flex_values[2]);
  Serial.print("\t");
  Serial.print(flex_values[3]);
  Serial.print("\t");
  Serial.print(flex_values[4]);
  Serial.print("\t");
  Serial.print(" ");
  Serial.println(handPos);

  //Audio Playing---------------------------------------------------------
  //Relaxed
  if (flex_values[0] == 0 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 0 && flex_values[4] == 0 && handPos == 0) {
  }

  //A
  else if (flex_values[0] == 0 && flex_values[1] == 2 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 1);
  }

  //B
  else if (flex_values[0] == 1 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 0 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 2);
  }

  //C
  else if (flex_values[0] == 0 && flex_values[1] == 1 && flex_values[2] == 1 && flex_values[3] == 1 && flex_values[4] == 1 && handPos == 1) {
    player.playFolder(1, 3);
  }

  //D
  else if (flex_values[0] == 2 && flex_values[1] == 0 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 4);
  }

  //E
  else if (flex_values[0] == 2 && flex_values[1] == 2 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 5);
  }

  //F
  else if (flex_values[0] == 1 && flex_values[1] == 2 && flex_values[2] == 0 && flex_values[3] == 0 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 6);
  }

  //G
  else if (flex_values[0] == 0 && flex_values[1] == 0 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 2) {
    player.playFolder(1, 7);
  }

  //H
  else if (flex_values[0] == 0 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 2 && flex_values[4] == 1 && handPos == 2) {
    player.playFolder(1, 8);
  }

  //I
  else if (flex_values[0] == 2 && flex_values[1] == 2 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 9);
  }

  //J
  else if (flex_values[0] == 2 && flex_values[1] == 2 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 10);
  }

  //K
  else if (flex_values[0] == 0 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 11);
  }

  //L
  else if (flex_values[0] == 0 && flex_values[1] == 0 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 12);
  }

  //M
  else if (flex_values[0] == 0 && flex_values[1] == 1 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 0) {
    player.playFolder(1, 13);
  }

  //N
  else if (flex_values[0] == 1 && flex_values[1] == 2 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 0) {
    player.playFolder(1, 14);
  }

  //O
  else if (flex_values[0] == 1 && flex_values[1] == 1 && flex_values[2] == 1 && flex_values[3] == 1 && flex_values[4] == 1 && handPos == 1) {
    player.playFolder(1, 15);
  }

  //P
  else if (flex_values[0] == 0 && flex_values[1] == 0 && flex_values[2] == 1 && flex_values[3] == 1 && flex_values[4] == 1 && handPos == 0) {
    player.playFolder(1, 16);
  }

  //Q
  else if (flex_values[0] == 0 && flex_values[1] == 0 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 0) {
    player.playFolder(1, 17);
  }

  //R
  else if (flex_values[0] == 1 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 18);
  }

  //S
  else if (flex_values[0] == 1 && flex_values[1] == 2 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 19);
  }

  //T
  else if (flex_values[0] == 0 && flex_values[1] == 1 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 20);
  }

  //U
  else if (flex_values[0] == 1 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 21);
  }

  //V
  else if (flex_values[0] == 1 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 22);
  }

  //W
  else if (flex_values[0] == 1 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 0 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 23);
  }

  //X
  else if (flex_values[0] == 2 && flex_values[1] == 1 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 24);
  }

  //Y
  else if (flex_values[0] == 0 && flex_values[1] == 1 && flex_values[2] == 1 && flex_values[3] == 2 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 25);
  }

  //Z
  else if (flex_values[0] == 2 && flex_values[1] == 0 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 26);
  }
  //Numbers ---------------------------------
  //1
  else if (flex_values[0] == 2 && flex_values[1] == 0 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 27);
  }

  //2
  else if (flex_values[0] == 2 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 28);
  }

  //3
  else if (flex_values[0] == 0 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 29);
  }

  //4
  else if (flex_values[0] == 1 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 0 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 30);
  }

  //5
  else if (flex_values[0] == 0 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 0 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 31);
  }

  //6
  else if (flex_values[0] == 2 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 0 && flex_values[4] == 2 && handPos == 1) {
    player.playFolder(1, 32);
  }

  //7
  else if (flex_values[0] == 2 && flex_values[1] == 0 && flex_values[2] == 0 && flex_values[3] == 2 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 33);
  }

  //8
  else if (flex_values[0] == 2 && flex_values[1] == 0 && flex_values[2] == 2 && flex_values[3] == 0 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 34);
  }

  //9
  else if (flex_values[0] == 2 && flex_values[1] == 1 && flex_values[2] == 0 && flex_values[3] == 0 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 35);
  }

  //I love You
  else if (flex_values[0] == 0 && flex_values[1] == 0 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 0 && handPos == 1) {
    player.playFolder(1, 41);
  }

  else if (flex_values[0] == 0 && flex_values[1] == 2 && flex_values[2] == 0 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 2) {
    player.playFolder(1, 44);
  }

  else if (flex_values[0] == 0 && flex_values[1] == 2 && flex_values[2] == 2 && flex_values[3] == 2 && flex_values[4] == 2 && handPos == 2) {
    player.playFolder(1, 38);
  }

  delay(1500);
}
