---
title: How to publish Obsidian notes with Quartz on GitHub Pages
draft: false
tags:
  - 
---
<div style="width: 100%; height: 400px; background-color: gray; display: flex">
<div style="margin: auto; color: black; font-weight: 800;">Image Placeholder</div>
</div>




- [ ] Poster
- [x] Presentation
- [x] Artifacts (Downloadables)
- [x] The Idea (again)
- [x] Understanding the BROSE Flipdot Module
	- [x] How the module works
	- [x] How to address the module
		- [x] The Comparator logic
		- [x] The FP2840 Chip
		- [x] The 60pin Connector
- [ ] Actually flipping some dots (testing)
- [ ] Designing the Boards
	- [ ] Controller Board
	- [ ] Decoder Board
- [ ] Manufacturing the Controller Board
- [ ] The Joystick and Case
- [ ] The Code
- [ ] Problems with the Decoder Board, Changes for v2
- [ ] Problems with v2, Changes for v3






















Tests mit einem NPN Transistor und NPN MosFET


![[Pasted image 20251209151758.png]]

BOM:
https://www.reichelt.com/de/en/shop/product/pin_connector_60-pin_with_interlock_straight-14878?country=de&CCTYPE=private&LANGUAGE=en

https://www.reichelt.de/de/de/shop/produkt/decoder_mpx_decimal_3_15_v_dip-16-12587

https://www.amazon.de/sourcing-map-Flachbandkabel-Stecker-Abstand/dp/B07SWJH7DF

https://www.reichelt.de/de/de/shop/produkt/mosfet_n-logl_30v_86a_0_0058r_to252aa-254838?search=IRLR8726&

https://www.digikey.de/de/products/detail/texas-instruments/lm340mpx-5-0-nopb/367021?_gl=1*8jtwim*_up*MQ..*_gs*MQ..&gclid=Cj0KCQiArt_JBhCTARIsADQZaykTTG1ZNrvjT8GBAVe2rledYsfHU3jME8hS58xY8ynrQ0BKBolanuEaAg1XEALw_wcB&gclsrc=aw.ds&gbraid=0AAAAADrbLlhe6aXNQm53MHciUciWaB_mp

https://www.reichelt.com/de/en/shop/product/developer_boards_-_joystick_module-376780


WIP PCB:
[[digiFabFinal.zip]]





Truth Table

| 2   | 7   | 6   | 5   | 4   | 3   | OUTPUT |
| --- | --- | --- | --- | --- | --- | ------ |
| 0   | x   | x   | x   | x   | x   | /      |
| 1   | 0   | 0   | 0   | 0   | 0   | PMOS1  |
| 1   | 0   | 0   | 0   | 0   | 1   | PMOS2  |
| 1   | 0   | 0   | 0   | 1   | 0   | PMOS3  |
| 1   | 0   | 0   | 0   | 1   | 1   | PMOS4  |
| 1   | 0   | 0   | 1   | 0   | 0   | PMOS5  |
| 1   | 0   | 0   | 1   | 0   | 1   | PMOS6  |
| 1   | 0   | 0   | 1   | 1   | 0   | PMOS7  |
| 1   | 0   | 0   | 1   | 1   | 1   | PMOS8  |
| 1   | 0   | 1   | 0   | 0   | 0   | PMOS9  |
| 1   | 0   | 1   | 0   | 0   | 1   | PMOS10 |
| 1   | 0   | 1   | 0   | 1   | 0   | PMOS11 |
| 1   | 0   | 1   | 0   | 1   | 1   | PMOS12 |
| 1   | 0   | 1   | 1   | 0   | 0   | PMOS13 |
| 1   | 0   | 1   | 1   | 0   | 1   | PMOS14 |
| 1   | 0   | 1   | 1   | 1   | 0   | PMOS15 |
| 1   | 0   | 1   | 1   | 1   | 1   | PMOS16 |
| 1   | 1   | 0   | 0   | 0   | 0   | NMOS1  |
| 1   | 1   | 0   | 0   | 0   | 1   | NMOS2  |
| 1   | 1   | 0   | 0   | 1   | 0   | NMOS3  |
| 1   | 1   | 0   | 0   | 1   | 1   | NMOS4  |
| 1   | 1   | 0   | 1   | 0   | 0   | NMOS5  |
| 1   | 1   | 0   | 1   | 0   | 1   | NMOS6  |
| 1   | 1   | 0   | 1   | 1   | 0   | NMOS7  |
| 1   | 1   | 0   | 1   | 1   | 1   | NMOS8  |
| 1   | 1   | 1   | 0   | 0   | 0   | NMOS9  |
| 1   | 1   | 1   | 0   | 0   | 1   | NMOS10 |
| 1   | 1   | 1   | 0   | 1   | 0   | NMOS11 |
| 1   | 1   | 1   | 0   | 1   | 1   | NMOS12 |
| 1   | 1   | 1   | 1   | 0   | 0   | NMOS13 |
| 1   | 1   | 1   | 1   | 0   | 1   | NMOS14 |
| 1   | 1   | 1   | 1   | 1   | 0   | NMOS15 |
| 1   | 1   | 1   | 1   | 1   | 1   | NMOS16 |
0 ... 15 => PMOS1 ... PMOS16
16 ... 31 => NMOS1 ... NMOS 16




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