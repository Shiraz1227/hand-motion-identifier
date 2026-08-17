#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
float Ax;
float Ay;
float Az;
float cAx;
float cAy;
float cAz;
float pitchRAW;
float rollRAW;
float pitch = 0.0;
float roll = 0.0;
float confidenceFactor = 0.05;
unsigned long AccCurrentTime;
unsigned long AccPreviousTime = 0;
int AccDelayTime = 100;

Adafruit_MPU6050 mpu;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G); // plus or minus 2g
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

float calibration(float someAcc,float maxReading, float minReading) { //Arduino Tutorial 79
  float offsetError = (maxReading + minReading) / 2;
  float scaleError = 19.62 / (maxReading - minReading);
  return scaleError * (someAcc - offsetError);
}

void accelerationPrinter(String someLetter) {
  if (someLetter == "c") {
    Serial.print("cAx:");
    Serial.print(cAx);
    Serial.print(",");
    Serial.print("cAy:");
    Serial.print(cAy);
    Serial.print(",");
    Serial.print("cAz:");
    Serial.print(cAz);
    Serial.print(",");
  }
  else if (someLetter == "r") {
    Serial.print("Ax:");
    Serial.print(Ax);
    Serial.print(",");
    Serial.print("Ay:");
    Serial.print(Ay);
    Serial.print(",");
    Serial.print("Az:");
    Serial.print(Az);
    Serial.print(",");
  }
}

void pitchAndRollPrinter(String someLetter) {
  if (someLetter == "f") {
    Serial.print("Pitch:");
    Serial.print(pitch);
    Serial.print(",");
    Serial.print("Roll:");
    Serial.print(roll);
    Serial.print(",");
  }
  else if (someLetter == "r") {
    Serial.print("PitchRAW:");
    Serial.print(pitchRAW);
    Serial.print(",");
    Serial.print("RollRAW:");
    Serial.print(rollRAW);
    Serial.print(",");
  }
}

void boundaryPrinter() {
    Serial.print("UL:");
    Serial.print(90);
    Serial.print(",");
    Serial.print("LL:");
    Serial.print(-90);
    Serial.print(",");
    Serial.println(".");
}

void pySerialPrinter() {
  Serial.print("Stuff still needs to be added to this function!!!");
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Ax = a.acceleration.x; 
  Ay = a.acceleration.y;
  Az = a.acceleration.z;
  cAx = calibration(Ax,10.2,-9.36) / 9.81;
  cAy = calibration(Ay,9.50,-10.07) / 9.81;
  cAz = calibration(Az,10.0,-10.0) / 9.81;
  pitchRAW = atan2(cAy,sqrt((cAz * cAz) + (cAx * cAx))) * (180.0 / PI);
  rollRAW = atan2(cAx,sqrt((cAz * cAz) + (cAy * cAy))) * (180.0 / PI);
  pitch = ((1.0 - confidenceFactor) * pitch) + (confidenceFactor * pitchRAW);
  roll = ((1.0 - confidenceFactor) * roll) + (confidenceFactor * rollRAW);
  AccCurrentTime = millis();
  if (AccCurrentTime - AccPreviousTime > AccDelayTime) {
    accelerationPrinter("c");
    pitchAndRollPrinter("f");
    pitchAndRollPrinter("r");
    boundaryPrinter();
    AccPreviousTime = AccCurrentTime;
  }
}
