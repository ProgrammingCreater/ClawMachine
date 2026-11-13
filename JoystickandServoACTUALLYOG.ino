#define stepXPin 3
#define stepYPin 4

#define dirXPin 5
#define dirYPin 6

#define enXPin 8       // A4988 Enable pin
#define enYPin 9

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


void setup() {
  pinMode(stepXPin, OUTPUT);
  pinMode(dirXPin, OUTPUT);
  pinMode(enXPin, OUTPUT);

  pinMode(stepYPin, OUTPUT);
  pinMode(dirYPin, OUTPUT);
  pinMode(enYPin, OUTPUT);

  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);

  digitalWrite(enXPin, LOW); // Enable motor outputs at startup
  digitalWrite(enYPin, LOW);

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

  if (xValue < 400) {
    // Move LEFT
    digitalWrite(enXPin, LOW);        // Enable motor
    digitalWrite(dirXPin, LOW);       // Set direction
    stepXMotor(map(512 - xValue, 0, 512, 1, 10));  // More deflection = more steps

  } else if (xValue > 624) {
    // Move RIGHT
    digitalWrite(enXPin, LOW);        // Enable motor
    digitalWrite(dirXPin, HIGH);      // Set direction
    stepXMotor(map(xValue - 512, 0, 511, 1, 10));
  
  } else if (yValue < 400) {
    // Move UP
    digitalWrite(enYPin, LOW);        // Enable motor
    digitalWrite(dirYPin, LOW);      // Set direction
    stepYMotor(map(512 - yValue, 0, 512, 1, 10));

  } else if (yValue > 624) {
    // Move DOWN
    digitalWrite(enYPin, LOW);        // Enable motor
    digitalWrite(dirYPin, HIGH);      // Set direction
    stepYMotor(map(yValue - 512, 0, 511, 1, 10));

  } else {
    // Joystick centered — fully stop and disable motor
    digitalWrite(enXPin, HIGH);
    digitalWrite(enYPin, HIGH);       // Disable motor outputs
    delay(100);                       
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

void stepXMotor(int stepsX) {
  for (int i = 0; i < stepsX; i++) {
    digitalWrite(stepXPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepXPin, LOW);
    delayMicroseconds(800);
  }
}

void stepYMotor(int stepsY) {
  for (int i = 0; i < stepsY; i++) {
    digitalWrite(stepYPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepYPin, LOW);
    delayMicroseconds(800);
  }
}