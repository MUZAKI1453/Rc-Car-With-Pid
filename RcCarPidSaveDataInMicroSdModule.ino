#include <AFMotor.h>
#include <Servo.h>
#include <SD.h>

// Motor dan sensor setup
AF_DCMotor motor1(2); // motor pada channel 2 (motor kanan)
AF_DCMotor motor2(3); // motor pada channel 3 (motor kiri)

#define Echo1 A0 // sensor tengah
#define Trig1 A1
#define Echo2 A2 // sensor kiri
#define Trig2 A3
#define Echo3 A4 // sensor kanan
#define Trig3 A5

Servo myservo;
int pos = 90; // Posisi lurus servo
int currentSpeed = 0;
const int maxSpeed = 255;
const int minSpeed = 150;
const int speedAdjustment = 50;

// PID constants
double Kp = 2.0; // Konstanta Proportional
double Ki = 0.0; // Konstanta Integral
double Kd = 1.0; // Konstanta Derivative

double previous_error = 0;
double integral = 0;

const int chipSelect = 4; // Pin CS untuk SD card

void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);
  pinMode(Trig3, OUTPUT);
  pinMode(Echo3, INPUT);
  myservo.attach(10);

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");
}

long readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration / 29 / 2; // Menghitung jarak dalam cm
}

void loop() {
  long distanceLeft = readUltrasonic(Trig2, Echo2);
  long distanceRight = readUltrasonic(Trig3, Echo3);

  double error = distanceLeft - distanceRight;
  integral += error;
  double derivative = error - previous_error;
  double output = Kp * error + Ki * integral + Kd * derivative;

  pos = 90 + output; // Menyesuaikan posisi servo berdasarkan output PID
  pos = constrain(pos, 45, 135); // Batasan gerakan servo
  myservo.write(pos);

  // Set kecepatan berdasarkan error
  int baseSpeed = map(abs(error), 0, 80, maxSpeed, minSpeed);
  int speedRight = baseSpeed;
  int speedLeft = baseSpeed;

  // Adjust kecepatan untuk belok
  if (error > 0) {
    speedLeft -= speedAdjustment; // Belok ke kiri
  } else if (error < 0) {
    speedRight -= speedAdjustment; // Belok ke kanan
  }

  speedLeft = constrain(speedLeft, minSpeed, maxSpeed);
  speedRight = constrain(speedRight, minSpeed, maxSpeed);

  motor1.setSpeed(speedRight);
  motor2.setSpeed(speedLeft);

  motor1.run(FORWARD);
  motor2.run(FORWARD);

  // Simpan data ke SD card
  logDataToSD(distanceLeft, distanceRight, error, output, speedLeft, speedRight);

  previous_error = error;

  delay(100); // Delay untuk stabilitas
}

void logDataToSD(long distanceLeft, long distanceRight, double error, double output, int speedLeft, int speedRight) {
  File dataFile = SD.open("data.txt", FILE_WRITE);
  
  if (dataFile) {
    dataFile.print("Distance Left: ");
    dataFile.print(distanceLeft);
    dataFile.print(" cm, Distance Right: ");
    dataFile.print(distanceRight);
    dataFile.print(" cm, Error: ");
    dataFile.print(error);
    dataFile.print(", PID Output: ");
    dataFile.print(output);
    dataFile.print(", Speed Left: ");
    dataFile.print(speedLeft);
    dataFile.print(", Speed Right: ");
    dataFile.println(speedRight);
    dataFile.close();
  } else {
    Serial.println("Error opening data file!");
  }
}
