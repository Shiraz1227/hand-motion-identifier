#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
float Ax;
float Ay;
float Az;

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

  Serial.print("Accx:"); //Only one word before the colon allowed in serial plotter
  Serial.print(Ax);
  Serial.print(",");
  Serial.print("Accy:");
  Serial.print(Ay);
  Serial.print(",");
  Serial.print("Accz:");
  Serial.print(Az);
  Serial.print(",");
  Serial.print("UL:");
  Serial.print(1);
  Serial.print(",");
  Serial.print("LL:");
  Serial.println("-1");
  delay(100);
}
