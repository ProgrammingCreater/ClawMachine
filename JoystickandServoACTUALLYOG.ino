#define stepPin 3
#define dirPin 5
#define enPin 8       // A4988 Enable pin
#define joyX A0       // Joystick X-axis

#include <Servo.h>

Servo myServo;

const int servoPin = 9;
const int buttonPin = 2;

const int homeAngle = 50;
const int rotatedAngle = 100;

bool rotated = false;

bool lastButtonState = HIGH;


void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(joyX, INPUT);

  digitalWrite(enPin, LOW); // Enable motor outputs at startup

  // Servo setup
  myServo.attach(servoPin);

  pinMode(buttonPin, INPUT_PULLUP);

  // Home servo at startup
  myServo.write(homeAngle);

  delay(500);
}

void loop() {
  int xValue = analogRead(joyX);

  if (xValue < 400) {
    // Move LEFT
    digitalWrite(enPin, LOW);        // Enable motor
    digitalWrite(dirPin, LOW);       // Set direction
    stepMotor(map(512 - xValue, 0, 512, 1, 10));  // More deflection = more steps

  } else if (xValue > 624) {
    // Move RIGHT
    digitalWrite(enPin, LOW);        // Enable motor
    digitalWrite(dirPin, HIGH);      // Set direction
    stepMotor(map(xValue - 512, 0, 511, 1, 10));

  } else {
    // Joystick centered — fully stop and disable motor
    digitalWrite(enPin, HIGH);       // Disable motor outputs
    delay(10);                       
  }

  //SERVO
  bool currentButtonState = digitalRead(buttonPin);

  // Detect new button press
  if (lastButtonState == HIGH && currentButtonState == LOW) {

    // Toggle state
    rotated = !rotated;

    if (rotated) {
      myServo.write(rotatedAngle);
    }
    else {
      myServo.write(homeAngle);
    }

    // Debounce delay
    delay(200);
  }

  lastButtonState = currentButtonState;
}

void stepMotor(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
  }
}