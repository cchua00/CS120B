int B[4] = {2, 3, 4, 5};
int x = 6;
int y = 7;

void setup() {

  // put your setup code here, to run once:
  pinMode(x, INPUT); 
  pinMode(y, INPUT_PULLUP); 
  for (int i = 0; i < 4; i++) {
    pinMode(B[i], OUTPUT);
 }
  Serial.begin(9600);
}


void loop() {
    
  // put your main code here, to run repeatedly:
  int On = digitalRead(y); 
  int Off = digitalRead(x); 
  if(On == LOW){
    for (int i = 0; i < 4; i++) {
      digitalWrite(B[i], HIGH);
    }
    delay(500);
  }
    else if(Off == HIGH){ 
      for (int i = 0; i < 4; i++) {
      digitalWrite(B[i], LOW);
    }
      delay(500); 
    }
  
}
