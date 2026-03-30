#include <Wire.h>

#define MPU_ADDR 0x68
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H 0x43
#define GYRO_YOUT_H 0x45
#define GYRO_ZOUT_H 0x47

void setup() {
  Wire.begin();
  Serial.begin(9600);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  
  Wire.write(0);     
  Wire.endTransmission(true);
}

int16_t readRawData(uint8_t reg) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 2, true);
  return Wire.read() << 8 | Wire.read();
}

void loop() {
  int16_t accelX = readRawData(ACCEL_XOUT_H);
  int16_t accelY = readRawData(ACCEL_YOUT_H);
  int16_t accelZ = readRawData(ACCEL_ZOUT_H);
  int16_t gyroX = readRawData(GYRO_XOUT_H);
  int16_t gyroY = readRawData(GYRO_YOUT_H);
  int16_t gyroZ = readRawData(GYRO_ZOUT_H);

  Serial.print("Accel X: "); Serial.print(accelX);
  Serial.print(" | Accel Y: "); Serial.print(accelY);
  Serial.print(" | Accel Z: "); Serial.print(accelZ);
  Serial.print(" | Gyro X: "); Serial.print(gyroX);
  Serial.print(" | Gyro Y: "); Serial.print(gyroY);
  Serial.print(" | Gyro Z: "); Serial.println(gyroZ);
  delay(500);
}
