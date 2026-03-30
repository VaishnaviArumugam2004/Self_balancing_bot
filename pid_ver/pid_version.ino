#include <Wire.h>
#include <MPU6050.h>

#define STEP_PIN_X 2   
#define DIR_PIN_X 5   
#define STEP_PIN_Y 3   
#define DIR_PIN_Y 6    
#define ENABLE_PIN 8   


double setpointX = 0, inputX, outputX; 
double setpointY = 0, inputY, outputY;


double Kp = 1.0, Ki = 0.1, Kd = 0.5;
double previousErrorX = 0, previousErrorY = 0;
double integralX = 0, integralY = 0;
unsigned long lastTimeX = 0, lastTimeY = 0;


MPU6050 mpu;

int16_t ax, ay, az, gx, gy, gz;

void setup() {
  
  Wire.begin();
  mpu.initialize();
  
  
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);  
  }

  
  pinMode(STEP_PIN_X, OUTPUT);
  pinMode(DIR_PIN_X, OUTPUT);
  pinMode(STEP_PIN_Y, OUTPUT);
  pinMode(DIR_PIN_Y, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  
  digitalWrite(ENABLE_PIN, HIGH);
  
  
  Serial.begin(9600);
}

void loop() {
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  
  float tiltX = atan2(ay, az) * 180.0 / PI;  
  float tiltY = atan2(ax, az) * 180.0 / PI;  

  
  inputX = tiltX;
  inputY = tiltY;

  
  computePID(inputX, outputX, setpointX, Kp, Ki, Kd, previousErrorX, integralX, lastTimeX);
  computePID(inputY, outputY, setpointY, Kp, Ki, Kd, previousErrorY, integralY, lastTimeY);

  
  controlMotor(STEP_PIN_X, DIR_PIN_X, outputX);
  controlMotor(STEP_PIN_Y, DIR_PIN_Y, outputY);

  
  delay(10);
}

void computePID(double input, double &output, double setpoint, double Kp, double Ki, double Kd, double &previousError, double &integral, unsigned long &lastTime) {
  unsigned long currentTime = millis();
  double elapsedTime = (currentTime - lastTime) / 1000.0;  
  double error = setpoint - input;  
  integral += error * elapsedTime;  
  double derivative = (error - previousError) / elapsedTime;  

  
  output = Kp * error + Ki * integral + Kd * derivative;

  previousError = error;
  lastTime = currentTime;
}

void controlMotor(int stepPin, int dirPin, double pidOutput) {
  
  if (pidOutput > 0) {
    digitalWrite(dirPin, HIGH);  
  } else 
    digitalWrite(dirPin, LOW);   
  }

  
  int delayTime = map(abs(pidOutput), 0, 255, 1000, 100);
  digitalWrite(ENABLE_PIN, LOW);  
  
  for (int i = 0; i < 200; i++) { 
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayTime);  
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayTime);  
  }
  digitalWrite(ENABLE_PIN, HIGH);  
}
