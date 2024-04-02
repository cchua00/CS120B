#include "Timer.h"

// Define the pins used for the LEDs and button
const int greenLED = 4;
const int whiteLED = 3;
const int buttonPin = 5;
const int motionSensorPin = A0;

// Define the states for the bathroom light state machine
enum BL_States { BL_SMStart, BL_Off1, BL_On2, BL_On1, BL_Off2 } BL_State;

// Define the variables for the motion sensor and timing
unsigned long motionStartTime;
unsigned long buttonStartTime;
unsigned long motionTimeout = 20000; // 20 seconds
unsigned long buttonTimeout = 10000; // 10 seconds
bool motionDetected = false;
bool buttonPressed = false;
int period = 200; 
void TickFct_BathroomLight() {
  switch (BL_State) {
    case BL_SMStart:
      BL_State = BL_Off2;
      break;
    case BL_Off2:
      Serial.print("OFF\n");
      if (motionDetected || buttonPressed) {
        if(buttonPressed){ 
           BL_State = BL_On1;
           buttonTimeout = 10000; 
           motionTimeout = 20000; 
        }else{ 
          BL_State = BL_On1; 
        }
        Serial.print("OFF->ON motion or button\n");
      } else if(!buttonPressed){ 
        buttonTimeout -= period; 
        if(buttonTimeout <=0) {
          buttonPressed = false; 
          buttonTimeout = 10000; 
        }
      }
      break;
    case BL_On1:
    Serial.print("Wait->ON\n");
      if (buttonPressed) {
        BL_State = BL_On1;
        Serial.print("wait ON->motiondetected\n");
      } else if(!buttonPressed) { 
        buttonTimeout -= period; 
        if (buttonTimeout <= 0) {
          BL_State = BL_Off1;
          motionTimeout = 20000;
        }
      } else {
        motionTimeout = 20000;
      }
      break;
    case BL_On2:
      if(buttonPressed) {
        BL_State = BL_Off1;
        Serial.print("ON2->buttonpressed\n");
      } 
      break;
    case BL_Off1:
    if(!buttonPressed){
    buttonTimeout -= period;
      if (buttonTimeout <= 0){
         BL_State = BL_Off2;
         buttonTimeout = 100000;
        Serial.print("OFF1 -> OFF2\n");
      }
      break;
    }
  }

  switch (BL_State) {
    case BL_Off2:
      digitalWrite(greenLED, HIGH);
      digitalWrite(whiteLED, LOW);
      break;
    case BL_On1:
      digitalWrite(greenLED, LOW);
      digitalWrite(whiteLED, HIGH);
      break;
    case BL_On2:
      digitalWrite(greenLED, LOW);
      digitalWrite(whiteLED, HIGH);
      break;
    case BL_Off1:
      digitalWrite(greenLED, HIGH);
      digitalWrite(whiteLED, LOW);
      break;
  }
}

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);
  pinMode(buttonPin, INPUT);
  TimerSet(period);
  TimerOn();
  Serial.begin(9600);
}

void loop() {
  
  // Check for button press
  if (digitalRead(buttonPin) == HIGH) {
    Serial.print("buttonPin true\n");
    buttonPressed = true;
  }else{ 
    buttonPressed = false;
    buttonStartTime = millis();
  }

  // Check for motion
  int motionSensorReading = analogRead(motionSensorPin);
  Serial.print("motionSensorReading: "); 
  Serial.print(analogRead(motionSensorPin));
  Serial.print("\n"); 
  if (motionSensorReading >= 300) {
    motionDetected = true;
    Serial.print("motiondetected true\n"); 
    
  } else {
    motionDetected = false;
    motionStartTime = millis();
    
  }
  TickFct_BathroomLight();
  while (!TimerFlag) {} // Wait for the timer period
  TimerFlag = 0;
}
