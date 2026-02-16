---
title: How to publish Obsidian notes with Quartz on GitHub Pages
draft: true
tags:
  -
---
Code draft
```c++
//pins to address flip dot (via hbridge)
int hBridge1 = 5;
int hBridge2 = 4;
int hBridge3 = 3;
int hBridge4 = 2;

//pins to read joystick
int stickPressed = 8;
int stickX = A0;
int stickY = A1;

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


void setup() {
  pinMode(hBridge1, OUTPUT);
  pinMode(hBridge2, OUTPUT);
  pinMode(hBridge3, OUTPUT);
  pinMode(hBridge4, OUTPUT);
  pinMode(8, INPUT);

  resetHBridge();

  Serial.begin(9600);
}

void loop() {
  if(buttonPressed()) {
    Serial.println("pressed");
  }

  handleJoystick();
}

void resetHBridge() {
  digitalWrite(hBridge1, LOW);
  digitalWrite(hBridge2, HIGH);
  digitalWrite(hBridge3, LOW);
  digitalWrite(hBridge4, HIGH);
}

bool buttonPressed() {
  int buttonReading = digitalRead(stickPressed);
  if (buttonReading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if((millis() - lastDebounceTime) > debounceDelay) {
    if(buttonReading != buttonState) {
      buttonState = buttonReading;
      if (buttonState == HIGH){
        lastButtonState = buttonReading;
        return true;
      }
    }
  }

  lastButtonState = buttonReading;
  return false;
}

void handleJoystick() {
  int joystickXRead = analogRead(A0);
  int joystickYRead = analogRead(A1);
  int x = ceil(3.0 * joystickXRead / 1023.0) - 2;
  int y = ceil(3.0 * joystickYRead / 1023.0) - 2;

  if(x != 0) {
    if(x > 0 && millis() - lastStickInputR > joystickTimeout) {
      Serial.println("right");
      resetLastStickInputs();
      lastStickInputR = millis();
    } else if(x < 0 && millis() - lastStickInputL > joystickTimeout) {
      Serial.println("left");
      resetLastStickInputs();
      lastStickInputL = millis();
    }
  } else if (y != 0) {
    if(y > 0 && millis() - lastStickInputD > joystickTimeout) {
      Serial.println("down");
      resetLastStickInputs();
      lastStickInputD = millis();
    } else if(y < 0 && millis() - lastStickInputU > joystickTimeout) {
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
``` 