// -*- c++ -*-

#define SDA_PORT PORTC
#define SDA_PIN 5
#define SCL_PORT PORTC
#define SCL_PIN 4
#define I2C_TIMEOUT 100
#define I2C_FASTMODE 1

#include <SoftWire.h>

//pins to address flip dot (via hbridge)
int hBridge1 = 5;
int hBridge2 = 4;
int hBridge3 = 3;
int hBridge4 = 2;

//pins to read joystick
int stickPressed = 8;
int stickX = A1;
int stickY = A0;

//stick variables
int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long lastStickInputR = 0;
unsigned long lastStickInputL = 0;
unsigned long lastStickInputU = 0;
unsigned long lastStickInputD = 0;
int joystickTimeout = 1000;

//flipdot
bool yellowDots[2] = {true, true};
int selectedDot = 0;
bool isFlipping = false;
unsigned long flipStart = 0;
int flipDuration = 1000;
int col4[2] = {7, 6}; //0 - set; 1 - reset;
int cols[3][2] = {{B00000100, B00001000}, {B00010000, B00100000}, {B00000001, B00000010}}; //13, 14;  15, 16;  11, 12

void setup() {
  pinMode(hBridge1, OUTPUT);
  pinMode(hBridge2, OUTPUT);
  pinMode(hBridge3, OUTPUT);
  pinMode(hBridge4, OUTPUT);
  pinMode(8, INPUT);

  Wire.begin();

  Wire.beginTransmission(0x20);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // set all of port A to outputs
  Wire.endTransmission();

  Wire.beginTransmission(0x20);
  Wire.write(0x01); // IODIRB register
  Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();

  resetFlipLogic();

  Serial.begin(9600);
}



void loop() {
  if(isFlipping && millis() - flipStart > flipDuration) {
    isFlipping = false;
    Serial.println("stop Flip");
    resetFlipLogic();
  }

  if (buttonPressed()) {
    Serial.println("start Flip");
    flipADot();
  }

  //handleJoystick();
}

void flipADot() {
  //get dot state first
  int dot = selectedDot;
  bool isYellow = yellowDots[dot];

  int set = dot == 1 ? hBridge3 : hBridge1;
  int reset = dot == 1  ? hBridge4 : hBridge2;
  int toWrite = B00000000;

  if(isYellow) {
    Serial.println("toBlack");
    digitalWrite(reset, LOW);
    digitalWrite(7, HIGH);
    yellowDots[dot] = false;
  } else {
    Serial.println("toYellow");
    digitalWrite(set, HIGH);
    digitalWrite(6, HIGH);
    yellowDots[dot] = true;
  }

  flipStart = millis();
  isFlipping = true;
}

void resetFlipLogic() {
  digitalWrite(hBridge1, LOW);
  digitalWrite(hBridge2, HIGH);
  digitalWrite(hBridge3, LOW);
  digitalWrite(hBridge4, HIGH);
  digitalWrite(col4[0], HIGH);
  digitalWrite(col4[1], LOW);

  Wire.beginTransmission(0x20);
  Wire.write(0x13); // address port B
  Wire.write(B00000000);  // value to send
  Wire.endTransmission();
}



bool buttonPressed() {
  int buttonReading = digitalRead(stickPressed);

  if (buttonReading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonReading != buttonState) {
      buttonState = buttonReading;
      if (buttonState == HIGH) {
        lastButtonState = buttonReading;
        return true;
      }
    }
  }

  lastButtonState = buttonReading;
  return false;
}

void handleJoystick() {
  int joystickXRead = analogRead(stickX);
  int joystickYRead = analogRead(stickY);
  int x = ceil(3.0 * joystickXRead / 1023.0) - 2;
  int y = ceil(3.0 * joystickYRead / 1023.0) - 2;

  if (x != 0) {
    if (x < 0 && millis() - lastStickInputR > joystickTimeout) {
      Serial.println("right");
      resetLastStickInputs();
      lastStickInputR = millis();
    } else if (x > 0 && millis() - lastStickInputL > joystickTimeout) {
      Serial.println("left");
      resetLastStickInputs();
      lastStickInputL = millis();
    }
  } else if (y != 0) {
    if (y < 0 && millis() - lastStickInputD > joystickTimeout) {
      Serial.println("down");
      resetLastStickInputs();
      lastStickInputD = millis();
    } else if (y > 0 && millis() - lastStickInputU > joystickTimeout) {
      Serial.println("up");
      resetLastStickInputs();
      lastStickInputU = millis();
    }
  }

  else {
    resetLastStickInputs();
  }
}

void resetLastStickInputs() {
  lastStickInputR = 0;
  lastStickInputL = 0;
  lastStickInputU = 0;
  lastStickInputD = 0;
}