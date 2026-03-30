#include <Wire.h>
const int MPU_ADDR = 0x68;  


int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;
float ax, ay, az, gx, gy, gz;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  
  Wire.write(0);     /
  Wire.endTransmission(true);
}

void loop() {
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  /
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);


  accX = Wire.read() << 8 | Wire.read();
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();

  
  ax = accX / 16384.0;
  ay = accY / 16384.0;
  az = accZ / 16384.0;
  gx = gyroX / 131.0;
  gy = gyroY / 131.0;
  gz = gyroZ / 131.0;


  Serial.print("AccX: "); Serial.print(ax); Serial.print(" ");
  Serial.print("AccY: "); Serial.print(ay); Serial.print(" ");
  Serial.print("AccZ: "); Serial.print(az); Serial.print(" ");
  Serial.print("GyroX: "); Serial.print(gx); Serial.print(" ");
  Serial.print("GyroY: "); Serial.print(gy); Serial.print(" ");
  Serial.print("GyroZ: "); Serial.println(gz);

  delay(100);  
}
