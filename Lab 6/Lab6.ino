#include "Timer.h"

int a = 3;
int b = 2;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
int dp = 9;
int sw = 10;
int gl = 12; 
int rl = 13; 
int xDirect = 0; 
int yDirect = 0; 
int motionFlag = 0; // Flag to indicate motion detection
int LEDS[7] {a, b, c, d, e, f, g};
int nums[11][7] {
  {1, 1, 1, 1, 1, 1, 0}, //0
  {0, 1, 1, 0, 0, 0, 0}, //1
  {1, 1, 0, 1, 1, 0, 1}, //2
  {1, 1, 1, 1, 0, 0, 1}, //3
  {0, 1, 1, 0, 0, 1, 1}, //4
  {1, 0, 1, 1, 0, 1, 1}, //5
  {1, 0, 1, 1, 1, 1, 1}, //6
  {1, 1, 1, 0, 0, 0, 0}, //7
  {1, 1, 1, 1, 1, 1, 1}, //8
  {1, 1, 1, 1, 0, 1, 1}, //9
  {0, 0, 0, 0, 0, 0, 0}, //off
};

void outNum(int x) { //outputs number x on the display
  for (int i = 0; i < 7; i++) {
    if (nums[x][i] == 1) {
      digitalWrite(LEDS[i], HIGH);
    }
    else {
      digitalWrite(LEDS[i], LOW);
    }
  }
}
/*
int readStick() { //returns 1 if the joystick was up, 2 if it is down, 0 for anything else
  // you may have to read from A0 instead of A1 depending on how you orient the joystick
  if (analogRead(A1) > 350 || analogRead(A0) > 350) {
    return 1; // down
  }
  else if (analogRead(A1) < 300||analogRead(A0) < 300) {
    return 2; // up
  }
  else {
    return 0;
  }

}
*/
int readStick() { 
  // Reading joystick values from both axes
  int x = analogRead(A0);
  int y = analogRead(A1);
  
  // If joystick moved in any direction, return 1
  if ((x < 300 || x > 360) || (y < 300 || y > 360)) {
    return 1;
  } else {
    return 0;
  }
}

unsigned char cflag = 0; // count flag
unsigned int count = 0;  // display count
unsigned long tcnt = 0; // tick count
unsigned int val = 1; // value to determine speed
enum LightStates { Wait, LightOn, ButtonHold, Countdown, OffCountdown } lightState;
enum LEDStates { LED_Off, LED_Countdown } ledState;

int button(){ 
  return (!digitalRead(sw));
}

void lightTick() {
  switch (lightState) {
    case Wait:
    Serial.print("WAIT"); 
    Serial.print("\n");  
      if (button() || readStick()> 1) {
        lightState = LightOn; 
        tcnt = 9; 
      }
      break;
      
    case LightOn:
      Serial.print("LIGHT ON "); 
      Serial.print("\n");  
      outNum(10); // clear LED
      if (button()) {
        lightState = ButtonHold;
      } else if (tcnt == 0) {
        lightState = Wait;
      }
      break;

    case ButtonHold:
    Serial.print("BUTTON HOLD "); 
    Serial.print("\n");  
      if (!button()) {
        lightState = OffCountdown;
        tcnt = 5;
      }
      break;

    case Countdown:
    Serial.print("COUNTDOWN "); 
    Serial.print("\n");  
      if (!button() && tcnt == 0) {
        lightState = Wait;
      }
      break;
      
    case OffCountdown:
    Serial.print("OFFCOUNTDOWN "); 
    Serial.print("\n");  
      if (tcnt == 0) {
        lightState = Wait;
      }
      break;

    default:
      lightState = Wait;
      break;
  }

  switch (lightState) {
    case Wait:
      outNum(10); 
      digitalWrite(rl, LOW); // room light off
      digitalWrite(gl, cflag ? HIGH : LOW); //indicator light blinking
      cflag = !cflag; // LED of
      tcnt = 0; // reset counter
      break;

    case LightOn:
      digitalWrite(rl, HIGH); // room light on
      digitalWrite(gl, LOW);//indicator light off
      cflag = 1; // LED on
      //tcnt = 9; // start counter at 9
      break;

    case ButtonHold:
      digitalWrite(rl, HIGH); // room light on
      digitalWrite(gl, LOW); // indicator light off
      cflag = 0; // LED off
      break;

    case Countdown:
      digitalWrite(rl, HIGH);//room light on 
      digitalWrite(gl, LOW); //indicator light off 
      cflag = 1; 
      break;
    case OffCountdown:
      digitalWrite(rl, LOW); // room light off
      digitalWrite(gl, HIGH); // indicator light on
      cflag = 1; // LED on
      break;
  }
}

void ledTick() {
  switch (ledState) {
    case LED_Off:
    Serial.print("LED_OFF"); 
    Serial.print("\n");  
      if (cflag) {
        ledState = LED_Countdown;
      }
      break;
      
    case LED_Countdown:
    Serial.print("LED_COUNTDOWN"); 
    Serial.print("\n");  
      if (!cflag) {
        ledState = LED_Off;
      }
      break;

    default:
      ledState = LED_Off;
      break;
  }

  switch (ledState) {
    case LED_Off:
      outNum(10); // clear LED
      break;

    case LED_Countdown:
      outNum(tcnt);//display light 
      Serial.print("tcnt: ");
      Serial.print(tcnt); 
      Serial.print(" "); 
      if (tcnt > 0) {//decrementing
        tcnt--;
      }
      break;
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(dp, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  pinMode(gl, OUTPUT); 
  pinMode(rl, OUTPUT); 
  TimerSet(500);
  TimerOn();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledTick(); 
  lightTick();
  digitalRead(sw);
  analogRead(A1); 
  analogRead(A0);
  Serial.print("anaogRead: "); 
  Serial.print(analogRead(A1));
  Serial.print("\n"); 
  Serial.print("anaogRead: "); 
  Serial.print(analogRead(A0));
  Serial.print("\n");   
  Serial.print("digitalRead: ");
  Serial.print(digitalRead(sw));
  Serial.print("\n");
  Serial.print("readstick() = "); 
  Serial.print(readStick()); 
  Serial.print("\n");
  
  while (!TimerFlag){}  // Wait for BL's period
  TimerFlag = 0;
}
