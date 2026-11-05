#define stepXPin 3
#define dirXn 5
#define enXPin 8       // A4988 Enable pin
#define joyX A0       // Joystick X-axis
#define joyY A1

#include <Servo.h>

Servo myServo;

const int servoPin = 9;
const int buttonPin = 2;

const int homeAngle = 50;
const int rotatedAngle = 100;

bool rotated = false;

bool lastButtonState = HIGH;

// Define constants for joystick control
const float DEADZONE = 0.3f;

// Boolean to detect motor running
bool motorsRunning = false;
bool motorsRunningDown = false;


void setup() {
  pinMode(steXpPin, OUTPUT);
  pinMode(dirXPin, OUTPUT);
  pinMode(enXPin, OUTPUT);
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);

  digitalWrite(enXPin, LOW); // Enable motor outputs at startup

  // Servo setup
  myServo.attach(servoPin);

  pinMode(buttonPin, INPUT_PULLUP);

  // Home servo at startup
  myServo.write(homeAngle);

  delay(500);
}

void loop() {
  int xValue = analogRead(joyX);
  int yValue = analogRead(joyY);

  if (xValue < -DEADZONE) {
    // Move LEFT
    digitalWrite(enXPin, LOW);        // Enable motor
    digitalWrite(dirPin, LOW);       // Set direction
    stepMotor(map(512 - xValue, 0, 512, 1, 10));  // More deflection = more steps

  } else if (xValue > DEADZONE) {
    // Move RIGHT
    digitalWrite(enXPin, LOW);        // Enable motor
    digitalWrite(dirPin, HIGH);      // Set direction
    stepMotor(map(xValue - 512, 0, 511, 1, 10));

  } else {
    // Joystick centered — fully stop and disable motor
    digitalWrite(enXPin, HIGH);       // Disable motor outputs
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