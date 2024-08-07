#include <AFMotor.h>
#include <Servo.h>

// Motor dan sensor setup
AF_DCMotor motor1(2); // motor pada channel 2
AF_DCMotor motor2(3); // motor pada channel 3

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
const int speedIncrement = 5;
const int speedDecrement = 5;

// PID constants
double Kp = 2.0; // Konstanta Proportional
double Ki = 0.0; // Konstanta Integral
double Kd = 1.0; // Konstanta Derivative

double previous_error = 0;
double integral = 0;

void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);
  pinMode(Trig3, OUTPUT);
  pinMode(Echo3, INPUT);
  myservo.attach(10);
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

  currentSpeed = map(abs(error), 0, 80, maxSpeed, minSpeed);
  motor1.setSpeed(currentSpeed);
  motor2.setSpeed(currentSpeed);

  motor1.run(FORWARD);
  motor2.run(FORWARD);

  previous_error = error;

  delay(100); // Delay untuk stabilitas
}
