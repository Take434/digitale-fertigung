---
title: Embedded Programming
draft: false
tags:
  -
---
# Tinker CAD 

## LED Control
https://www.tinkercad.com/things/5jIxxLDL1OE-led-address?sharecode=ZzfQ8HE0A_rFtyqUWMIS7ii4qkZixvuxi3DH0F1pdDk
![[Pasted image 20251213140103.png]]
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
## Game
https://www.tinkercad.com/things/lD81J6AEcYi-led-game?sharecode=x9mwJBXvOk8ahh_LDPndv0l1P7Svc5v7x9HzOWpI7qc
![[Pasted image 20251213140017.png]]
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