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
float AccConfidenceFactor = 0.9;
float gyroPitch = 0.0;
float gyroRoll = 0.0;
float gyroYaw = 0.0;
float compRoll = 0.0;
float compPitch = 0.0;
unsigned long currentTime;
unsigned long previousTime = 0;
unsigned long PRYCurrentTime = 0;
unsigned long PRYPreviousTime = 0;
int delayTime = 100;

Adafruit_MPU6050 mpu;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G); // plus or minus 2g
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

float AccCalibration(float someAcc,float maxReading, float minReading) { //Arduino Tutorial 79
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

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  //Acceleration section
  Ax = a.acceleration.x; 
  Ay = a.acceleration.y;
  Az = a.acceleration.z;
  cAx = AccCalibration(Ax,10.2,-9.36) / 9.81;
  cAy = AccCalibration(Ay,9.50,-10.07) / 9.81;
  cAz = AccCalibration(Az,10.0,-10.0) / 9.81;
  pitchRAW = atan2(cAy,sqrt((cAz * cAz) + (cAx * cAx))) * (180.0 / PI) * -1;
  rollRAW = atan2(cAx,sqrt((cAz * cAz) + (cAy * cAy))) * (180.0 / PI) * -1;
  // Acceleration pitch and roll. Uses low pass filter. 
  accPitch = (AccConfidenceFactor * accPitch) + ((1.0 - AccConfidenceFactor) * pitchRAW);
  accRoll = (AccConfidenceFactor * accRoll) + ((1.0 - AccConfidenceFactor) * rollRAW);
  //Gyroscope section
  Gx = g.gyro.x + 0.04; 
  Gy = g.gyro.y;
  Gz = g.gyro.z + 0.02;
  PRYCurrentTime = millis();
  float dt = (PRYCurrentTime - PRYPreviousTime) / 1000.0;
  PRYPreviousTime = PRYCurrentTime;
  gyroPitch += dt * Gx * (180.0 / PI);
  gyroRoll += dt * Gy * (180.0 / PI);
  gyroYaw += dt * Gz * (180.0 / PI);
  //Complementary filter section
  alpha = 0.98
  compRoll = ((1 - alpha) * accRoll) + alpha * (compRoll + dt * Gy * (180.0 / PI));
  compPitch = ((1 - alpha) * accPitch) + alpha * (compPitch + dt * Gx * (180.0 / PI));
  currentTime = millis();
  if (currentTime - previousTime > delayTime) {
    //accelerationPrinter("c");
    //pitchAndRollPrinter("f");
    //pitchAndRollPrinter("r");
    Serial.print("GyroRoll:");
    Serial.print(gyroRoll);
    Serial.print(",");
    Serial.print("AccRoll:");
    Serial.print(accRoll);
    Serial.print(",");
    Serial.print("RawRoll:");
    Serial.print(rollRAW);
    Serial.print(",");
    boundaryPrinter(90,-90);
    previousTime = currentTime;
  }
}
