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

int readStick() { //returns 1 if the joystick was up, 2 if it is down, 0 for anything else
  // you may have to read from A0 instead of A1 depending on how you orient the joystick
  if (analogRead(A1) > 800) {
    return 2; // down
  }
  else if (analogRead(A1) < 200) {
    return 1; // up
  }
  
  else {
    return 0;
  }

}

unsigned char cflag = 0; // count flag
unsigned int count = 0;  // display count
unsigned long tcnt = 0; // tick count
unsigned int speed[3] = {4,2,1}; // array of speeds
unsigned int val = 1; // value to determine speed
enum LightStates { Wait, LightOn, ButtonHold, Countdown, OffCountdown } lightState;
enum LEDStates { LED_Off, LED_Countdown } ledState;

// button press
int button() {
  return !digitalRead(sw);
}

void lightTick() {
  switch (lightState) {
    case Wait:
      if (button() || readStick()) {
        lightState = LightOn;
      }
      break;
      
    case LightOn:
      outNum(10); // clear LED
      if (button()) {
        lightState = ButtonHold;
      } else if (tcnt == 0) {
        lightState = Wait;
      }
      break;

    case ButtonHold:
      if (!button()) {
        lightState = OffCountdown;
        tcnt = 5;
      }
      break;

    case Countdown:
      if (!button() && tcnt == 0) {
        lightState = Wait;
      }
      break;
      
    case OffCountdown:
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
      digitalWrite(rl, LOW); // room light off
      digitalWrite(gl, HIGH); 
      cflag = 0; // LED off
      tcnt = 0; // reset counter
      break;

    case LightOn:
      digitalWrite(rl, HIGH); // room light on
      digitalWrite(gl, LOW);
      cflag = 1; // LED on
      tcnt = 9; // start counter at 9
      break;

    case ButtonHold:
      digitalWrite(rl, HIGH); // room light on
      digitalWrite(gl, LOW); 
      cflag = 0; // LED off
      break;

    case Countdown:
    case OffCountdown:
      digitalWrite(rl, LOW); // room light off
      digitalWrite(gl, HIGH);
      cflag = 1; // LED on
      break;
  }
}

void ledTick() {
  switch (ledState) {
    case LED_Off:
      if (cflag) {
        ledState = LED_Countdown;
      }
      break;
      
    case LED_Countdown:
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
      outNum(tcnt);
      if (tcnt > 0) {
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
  TimerSet(  500);
  TimerOn();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledTick(); 
  lightTick();
  digitalRead(sw);
  Serial.print("digitalRead: ");
  Serial.print(digitalRead(sw));
  Serial.print("\n");
  Serial.print(readStick()); 
  Serial.print("\n");
  Serial.print("speed: "); 
  Serial.print(speed[val]);
  Serial.print("\n");
  while (!TimerFlag){}  // Wait for BL's period
  TimerFlag = 0;
}
