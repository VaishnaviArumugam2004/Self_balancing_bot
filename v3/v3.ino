#include <Wire.h>

const int MPU_ADDR = 0x68;  // MPU6050 I2C address


int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;
float ax, ay, az, gx, gy, gz;


float angleAcc, angleGyro, angleFiltered;


float alpha = 0.98;  

unsigned long prevTime;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  
  Wire.write(0);     
  Wire.endTransmission(true);

  prevTime = millis();
}

void loop() {
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  
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

  
  angleAcc = atan2(ax, sqrt(ay * ay + az * az)) * 180 / PI;

  
  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0;
  prevTime = currentTime;

  
  angleGyro += gy * dt;

  
  angleFiltered = alpha * (angleFiltered + gy * dt) + (1 - alpha) * angleAcc;

  
  Serial.print("Acc_Angle: "); Serial.print(angleAcc); Serial.print(" ");
  Serial.print("Gyro_Angle: "); Serial.print(angleGyro); Serial.print(" ");
  Serial.print("Filtered_Angle: "); Serial.println(angleFiltered);

  delay(100);  
}
