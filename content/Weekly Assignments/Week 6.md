---
title: Embedded Programming
draft: false
tags:
  -
---
# Tinker CAD 

## LED Control
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

const int levels[7][6] = {
  {0, 0, 0, 0, 0, 1},
  {0, 0, 0, 1, 2, 3}, 
  {1, 3, 2, 1, 3, 0},  
  {0, 1, 0, 1, 0, 2},  
  {3, 2, 3, 1, 2, 1},  
  {2, 2, 1, 2, 1, 1},  
  {0, 1, 0, 2, 0, 3}
};

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
  if(canInput) {
    pollBtns();
  } else if (animation) {
    //play animation
  } else {
  	showLevel(currentLevel);
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
          canInput = false;
          currentLevel = 1;
          levelPreviewCounter = 0;
        }
      }
    }
    
    lastBtnStates[i] = reading;
  }
}
```