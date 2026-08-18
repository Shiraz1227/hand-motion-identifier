#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
float Ax;
float Ay;
float Az;
float Gx;
float Gy;
float Gz;
float cAx;
float cAy;
float cAz;
float accPitch = 0.0;
float accRoll = 0.0;
float pitchRAW;
float rollRAW;
float gyroPitch = 0.0;
float gyroRoll = 0.0;
float gyroYaw = 0.0;
float confidenceFactor = 0.05;
unsigned long AccCurrentTime;
unsigned long AccPreviousTime = 0;
unsigned long PRYCurrentTime = 0;
unsigned long PRYPreviousTime = 0;
int AccDelayTime = 100;

Adafruit_MPU6050 mpu;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G); // plus or minus 2g
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
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
    Serial.print(accPitch);
    Serial.print(",");
    Serial.print("Roll:");
    Serial.print(accRoll);
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

void boundaryPrinter(int upperLimit, int lowerLimit) {
    Serial.print("UL:");
    Serial.print(upperLimit);
    Serial.print(",");
    Serial.print("LL:");
    Serial.println(lowerLimit);
}

void pySerialPrinter() {
  Serial.print("Stuff still needs to be added to this function!!!");
}

void gyroPitchRollYaw(String rotation, float someG) {
  if (rotation == "Roll") { //So then Gy must be given. 

  }
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Ax = a.acceleration.x; 
  Ay = a.acceleration.y;
  Az = a.acceleration.z;
  Gx = g.gyro.x;
  Gy = g.gyro.y;
  Gz = g.gyro.z;
  PRYCurrentTime = millis();
  float dt = (PRYCurrentTime - PRYPreviousTime) / 1000.0;
  PRYPreviousTime = PRYCurrentTime;
  gyroPitch = gyroPitch + (dt * Gx * (180.0 / PI));
  gyroRoll += dt * Gy * (180.0 / PI);
  gyroYaw += dt * Gz * (180.0 / PI);
  cAx = calibration(Ax,10.2,-9.36) / 9.81;
  cAy = calibration(Ay,9.50,-10.07) / 9.81;
  cAz = calibration(Az,10.0,-10.0) / 9.81;
  pitchRAW = atan2(cAy,sqrt((cAz * cAz) + (cAx * cAx))) * (180.0 / PI);
  rollRAW = atan2(cAx,sqrt((cAz * cAz) + (cAy * cAy))) * (180.0 / PI);
  accPitch = ((1.0 - confidenceFactor) * accPitch) + (confidenceFactor * pitchRAW);
  accRoll = ((1.0 - confidenceFactor) * accRoll) + (confidenceFactor * rollRAW);
  AccCurrentTime = millis();
  if (AccCurrentTime - AccPreviousTime > AccDelayTime) {
    //accelerationPrinter("c");
    //pitchAndRollPrinter("f");
    //pitchAndRollPrinter("r");
    Serial.print("Roll:");
    Serial.print(gyroRoll);
    Serial.print(",");
    Serial.print("Pitch:");
    Serial.print(gyroPitch);
    Serial.print(",");
    Serial.print("Yaw:");
    Serial.print(gyroYaw);
    Serial.print(",");
    boundaryPrinter(180,-180);
    AccPreviousTime = AccCurrentTime;
  }
}
