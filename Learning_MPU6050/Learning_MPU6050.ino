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

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Ax = a.acceleration.x;
  Ay = a.acceleration.y;
  Az = a.acceleration.z;
  AccCurrentTime = millis();
  if (AccCurrentTime - AccPreviousTime > AccDelayTime) {
    Serial.print(Ax);
    Serial.print(",");
    Serial.print(Ay);
    Serial.print(",");
    Serial.println(Az);
    AccPreviousTime = AccCurrentTime;
  }
}
