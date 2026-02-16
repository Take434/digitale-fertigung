---
title: Embedded Programming
draft: false
tags:
  -
---
Disclaimer: I did not use any AI tools to create any of the code here. All of it was written by me.
# LED Control
https://www.tinkercad.com/things/5jIxxLDL1OE-led-address?sharecode=ZzfQ8HE0A_rFtyqUWMIS7ii4qkZixvuxi3DH0F1pdDk
![[Pasted image 20251213140103.png]]
This project works as follows:
Pressing the right dot turns on the currently selected LED (if you just started thats the left most one). The left dot turns of all LEDs and selects the next one in line, the order moves to the left. So when starting the sketch, first the left most is selected, then the right most, then the 3rd and so on.

Most logic here is needed to debounce the buttons using debounce time and a debounce delay. To do this i have followed the [official Arduino documentation](https://docs.arduino.cc/built-in-examples/digital/Debounce/).
```c++
const int leds[] = {13, 8, 12, 4};
const int leftBtn = 7;
const int rightBtn = 2;

int selector = 0;
int leftButtonState;
int leftLastButtonState = LOW;
int rightButtonState;
int rightLastButtonState = LOW;

unsigned long leftLastDebounceTime = 0;
unsigned long rightLastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup()
{
  pinMode(leds[0], OUTPUT);  
  pinMode(leds[1], OUTPUT);
  pinMode(leds[2], OUTPUT);
  pinMode(leds[3], OUTPUT);

  pinMode(leftBtn, INPUT);
  pinMode(rightBtn, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  int leftReading = digitalRead(leftBtn);
  int rightReading = digitalRead(rightBtn);
  
  if (leftReading != leftLastButtonState) {
    leftLastDebounceTime = millis();
  }
  
  if (rightReading != rightLastButtonState) {
    rightLastDebounceTime = millis();
  }
  
  if((millis() - leftLastDebounceTime) > debounceDelay) {
    if(leftReading != leftButtonState) {
      leftButtonState = leftReading;
      
      if (leftButtonState == HIGH){
        digitalWrite(leds[0], LOW);
        digitalWrite(leds[1], LOW);
        digitalWrite(leds[2], LOW);
        digitalWrite(leds[3], LOW);
    
        selector = (selector + 1) % 4;
      }
    }
  }
  
  if((millis() - rightLastDebounceTime) > debounceDelay) {
    if(rightReading != rightButtonState) {
      rightButtonState = rightReading;
      
      if (rightButtonState == HIGH){
        digitalWrite(leds[selector], HIGH);
      }
    }
  }
  
  leftLastButtonState = leftReading;
  rightLastButtonState = rightReading;
}
```
# Game
https://www.tinkercad.com/things/lD81J6AEcYi-led-game?sharecode=x9mwJBXvOk8ahh_LDPndv0l1P7Svc5v7x9HzOWpI7qc
![[Pasted image 20251213140017.png]]
The Game does the following:
It is a basic memory game. The Game starts with the LEDs on the left blinking in a specific order. After that the player has to repeat that order by pressing the buttons. There are seven Levels with 6 blinks each. These levels are hardcoded in a two dimensional array to allow programming specific patterns for the leds. If the user beats all the levels a win animation is played and the game is over.

In this sketch the buttons are debounced using an array and a four loop, this makes it easier to store the debounce state and gets rid of a lot of the repetetive logic from the previous sketch.

```c++
const int leds[] = {13, 12, 11, 10};
const int btns[] = {5, 4, 3, 2};

int btnStates[] = {LOW, LOW, LOW, LOW};
int lastBtnStates[] = {LOW, LOW, LOW, LOW};
unsigned long lastDebounceTime[] = {0, 0, 0, 0};
unsigned long debounceDelay = 50;

bool canInput = false;
bool animation = false;
const int blinkDuration = 500;
int currentLevel = 0;
int levelPreviewCounter = 0;
bool levelPreviewOn = false;
unsigned long levelPreviewStartTime;
int inputCounter = 0;
bool failed = false;
int pollLed = -1;
unsigned long pollLedStartTime;

const int levels[7][6] = {
  {0, 0, 0, 0, 0, 1},
  {0, 0, 0, 1, 2, 3}, 
  {1, 3, 2, 1, 3, 0},  
  {0, 1, 0, 1, 0, 2},  
  {3, 2, 3, 1, 2, 1},  
  {2, 2, 1, 2, 1, 1},  
  {0, 1, 0, 2, 0, 3}
};

unsigned long winLedStartTime;
bool winLedsOn = false;
int winCount = 0;

void setup()
{
  for(int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++) {
    pinMode(leds[i], OUTPUT);  
  }
  
  for(int i = 0; i < sizeof(btns) / sizeof(btns[0]); i++) {
    pinMode(btns[i], INPUT);  
  }
  
  Serial.begin(9600);
}

void loop()
{  
  if(pollLed >= 0 && (millis() - pollLedStartTime) > blinkDuration) {
  	digitalWrite(leds[pollLed], LOW);
    pollLed = -1;
  }
  
  if(pollLed < 0) {
    if(canInput) {
      pollBtns();
    } else if (animation) {
      winAnimation();  
    } else {
      showLevel(currentLevel);
    }
  }
}

void winAnimation() {
  if((millis() - winLedStartTime) > blinkDuration) {
    if(!winLedsOn) {
      for(int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++) {
        digitalWrite(leds[i], HIGH);
      }
      winLedsOn = true;
	  winLedStartTime = millis();
    } else {
      for(int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++) {
        digitalWrite(leds[i], LOW);
      }
      winLedStartTime = millis();
      winCount++;
      winLedsOn = false;
    }
  }
  
  if(winCount == 3) {
  	animation = false;
    currentLevel = 0;
    winCount = 0;
  }
}

void showLevel(int i) {
  if((millis() - levelPreviewStartTime) > blinkDuration) {
    if(!levelPreviewOn) {
      digitalWrite(leds[levels[i][levelPreviewCounter]], HIGH);
      levelPreviewStartTime = millis();
      levelPreviewOn = true;
    } else {
   	  digitalWrite(leds[levels[i][levelPreviewCounter]], LOW);
      levelPreviewStartTime = millis();
      levelPreviewOn = false;
      levelPreviewCounter++;
      
      if(levelPreviewCounter > (sizeof(levels[i]) / sizeof(levels[i][0])) - 1) {
        canInput = true;
      }
    }
  }
}

void pollBtns() {
  for(int i = 0; i < sizeof(btns) / sizeof(btns[0]); i++) {
    int reading = digitalRead(btns[i]);
    
    if (reading != lastBtnStates[i]) {
      lastDebounceTime[i] = millis();
    }
    
    if((millis() - lastDebounceTime[i]) > debounceDelay) {
      if(reading != btnStates[i]) {
        btnStates[i] = reading;
        
        if(btnStates[i] == HIGH) {
          Serial.println(i);
          Serial.println(levels[currentLevel][inputCounter]);
          
          if(i != levels[currentLevel][inputCounter]) {
          	failed = true;
          }
          
          inputCounter++;
          pollLed = i;
          digitalWrite(leds[pollLed], HIGH);
          pollLedStartTime = millis();
          
          
          if(inputCounter >= 6) {
          	canInput = false;
            inputCounter = 0;
            levelPreviewCounter = 0;
            
            if(!failed) {
              currentLevel++;
              
              if(currentLevel > 6) {
              	animation = true;
              }
            }
            
            failed = false;
          }
        }
      }
    }
    
    lastBtnStates[i] = reading;
  }
}
```

# Final Project - Simulation
https://www.tinkercad.com/things/09peXvtDDwc-finalproject-sim?sharecode=RJC8b4QNirZpRj1PC3QQeTXJfw8Ci0ighc6YJngbAVk
![[Pasted image 20260216223507.png]]
This simulation for my final project abstractly simulates handeling joystick input displaying that on a display. Since there is no lcd display that allows addressing individual pixels, it was not possible for me to program snake. Instead the user can move an X on the display and fix it in place by pressing the button.

The joystick is simulated using two potentiometers and a button. This is exactly the same as the actual joystick, just not in the nice package.

The actual functionality means that there is one X on the display. This can either be fixed in place, or movable, which is signified by the X blinking. To toggle whether the x is movable the user presses the button. To move the X the user uses the two potentiometers. The left potentiometer signifies left or right and the right one is for up and down (where down is right).

When starting the simulation the X is locked in place, to begin moving it has to be unlocked via the button first.
```c++
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

const int stickPressed = 2;
const int stickX = A0;
const int stickY = A1;

int stableButton = HIGH;
int lastStableButton = HIGH;
int lastRawButton = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long lastMoveTime = 0;
const unsigned long moveTimeout = 200;

int cursorCol = 0;
int cursorRow = 0;

int placedCol = -1;
int placedRow = -1;

bool blinkVisible = true;
bool shouldBlink = true;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 400;
int lastBlinkCol = -1;
int lastBlinkRow = -1;

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();

  pinMode(stickPressed, INPUT_PULLUP);
  Serial.begin(9600);

  blinkVisible = true;
  lastBlinkTime = millis();
  redrawAll();
}

void loop() {
  if (buttonPressedEdge()) {
    togglePlacedX();
    redrawAll();
  }

  handleJoystickMove();
  handleBlink();
}

bool buttonPressedEdge() {
  int raw = digitalRead(stickPressed);

  if (raw != lastRawButton) {
    lastDebounceTime = millis();
    lastRawButton = raw;
  }

  if (millis() - lastDebounceTime >= debounceDelay) {
    lastStableButton = stableButton;
    stableButton = raw;

    if (lastStableButton == HIGH && stableButton == LOW) {
      return true;
    }
  }

  return false;
}

void handleJoystickMove() {
  if (placedCol != -1) return;

  int joystickXRead = analogRead(stickX);
  int joystickYRead = analogRead(stickY);

  int x = (int)ceil(3.0 * joystickXRead / 1023.0) - 2;
  int y = (int)ceil(3.0 * joystickYRead / 1023.0) - 2;

  if (millis() - lastMoveTime < moveTimeout) return;

  bool moved = false;

  if (x != 0) {
    moved = (x > 0) ? moveCursor(1, 0) : moveCursor(-1, 0);
  } else if (y != 0) {
    moved = (y > 0) ? moveCursor(0, 1) : moveCursor(0, -1);
  }

  if (moved) {
    lastMoveTime = millis();
    redrawBlinkOnly();
  }
}

bool moveCursor(int dx, int dy) {
  int newCol = cursorCol + dx;
  int newRow = cursorRow + dy;

  newCol = constrain(newCol, 0, 15);
  newRow = constrain(newRow, 0, 1);

  if (newCol == cursorCol && newRow == cursorRow) return false;

  cursorCol = newCol;
  cursorRow = newRow;
  return true;
}

void togglePlacedX() {
  if (placedCol == -1) {
    placedCol = cursorCol;
    placedRow = cursorRow;

    blinkVisible = true;
    lastBlinkTime = millis();
    shouldBlink = false;
  } else {
    placedCol = -1;
    placedRow = -1;

    shouldBlink = true;
    blinkVisible = true;
    lastBlinkTime = millis();
    lastBlinkCol = -1;
    lastBlinkRow = -1;
  }
}

void redrawAll() {
  lcd.clear();

  if (placedCol != -1) {
    lastBlinkCol = -1;
    lastBlinkRow = -1;
    lcd.setCursor(placedCol, placedRow);
    lcd.print(blinkVisible ? 'X' : ' ');
    lastBlinkCol = placedCol;
    lastBlinkRow = placedRow;
  } else {
    lastBlinkCol = -1;
    lastBlinkRow = -1;
    lcd.setCursor(cursorCol, cursorRow);
    lcd.print(blinkVisible ? 'X' : ' ');
    lastBlinkCol = cursorCol;
    lastBlinkRow = cursorRow;
  }
}

void redrawBlinkOnly() {
  if (lastBlinkCol != -1) {
    lcd.setCursor(lastBlinkCol, lastBlinkRow);
    lcd.print(' ');
  }

  if (placedCol != -1) {
    lcd.setCursor(placedCol, placedRow);
    lcd.print(blinkVisible ? 'X' : ' ');
    lastBlinkCol = placedCol;
    lastBlinkRow = placedRow;
  } else {
    lcd.setCursor(cursorCol, cursorRow);
    lcd.print(blinkVisible ? 'X' : ' ');
    lastBlinkCol = cursorCol;
    lastBlinkRow = cursorRow;
  }
}

void handleBlink() {
  if (shouldBlink && millis() - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = millis();
    blinkVisible = !blinkVisible;
    redrawBlinkOnly();
  }
}
```