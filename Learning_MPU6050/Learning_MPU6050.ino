#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
float Ax;
float Ay;
float Az;
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

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Ax = a.acceleration.x; 
  Ay = a.acceleration.y;
  Az = a.acceleration.z;
  float cAx = calibration(Ax,10.2,-9.36);
  float cAy = calibration(Ay,9.50,-10.07);
  float cAz = calibration(Az,10.0,-10.0);
  float pitch = asin(cAy / 9.81) * (180.0 / PI);
  float roll = asin(cAx / 9.81) * (180.0 / PI);
  AccCurrentTime = millis();
  if (AccCurrentTime - AccPreviousTime > AccDelayTime) {
    Serial.print(cAx);
    Serial.print(",");
    Serial.print(cAy);
    Serial.print(",");
    Serial.print(cAz);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.println(roll);
    AccPreviousTime = AccCurrentTime;
  }
}
