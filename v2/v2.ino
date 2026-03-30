#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <AccelStepper.h>

MPU6050 mpu;

#define INTERRUPT_PIN 2
volatile bool mpuInterrupt = false;
void dmpDataReady() { mpuInterrupt = true; }

AccelStepper motorX(1, 2, 5);  // X-axis stepper
AccelStepper motorY(1, 3, 6);  // Y-axis stepper

float Kp = 30.0;  // PID constants
float Ki = 0.0;
float Kd = 1.0;

float setpoint = 0.0;
float input, error, output;
float lastError = 0.0;
float integral = 0.0;

Quaternion q;
VectorFloat gravity;
float ypr[3];

void setup() {
  Serial.begin(115200);
  Wire.begin();

  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788);

  mpu.setDMPEnabled(true);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);

  motorX.setMaxSpeed(1000);
  motorX.setAcceleration(500);

  motorY.setMaxSpeed(1000);
  motorY.setAcceleration(500);

  Serial.println("Self-balancing bot initialized.");
}

void loop() {
  if (!mpu.dmpGetCurrentFIFOPacket((uint8_t*)mpu.dmpPacketBuffer)) return;

  mpu.dmpGetQuaternion(&q, (uint8_t*)mpu.dmpPacketBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  input = ypr[1] * 180 / M_PI;  // Pitch angle

  error = setpoint - input;
  integral += error;
  float derivative = error - lastError;

  output = Kp * error + Ki * integral + Kd * derivative;
  lastError = error;

  // Adjust motor speeds based on output
  motorX.setSpeed(output);
  motorY.setSpeed(output);

  motorX.runSpeed();
  motorY.runSpeed();

  Serial.print("Pitch: ");
  Serial.print(input);
  Serial.print(" | Output: ");
  Serial.println(output);
}
