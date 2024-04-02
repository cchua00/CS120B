int B[4] = {2, 3, 4, 5};
int x = 6;
int y = 7;
int counter = 0; 

void setup() {

  // put your setup code here, to run once:
  pinMode(x, INPUT); 
  pinMode(y, INPUT_PULLUP); 
  for (int i = 0; i < 4; i++) {
    pinMode(B[i], OUTPUT);
 }
  Serial.begin(9600);
}

void flash() {
    for (int i = 0; i < 4; i++) {
      digitalWrite(B[i], HIGH);
    }
    delay(500);
    for (int i = 0; i < 4; i++) {
      digitalWrite(B[i], LOW);
    }
    delay(500);
    Serial.println(analogRead(A0));
}

void lightstage(){ 
 digitalWrite(B[0], LOW);
 digitalWrite(B[1], LOW);
 digitalWrite(B[2], LOW); 
 digitalWrite(B[3], LOW);
  switch(counter){ 
    case 0: 
    break; 
    case 1: 
    digitalWrite(B[3], HIGH);
    delay(500); 
    break;
    case 2: 
    digitalWrite(B[3], HIGH); 
    digitalWrite(B[2], HIGH);
    delay(500);
    break;
    case 3: 
    digitalWrite(B[3], HIGH); 
    digitalWrite(B[2], HIGH);
    digitalWrite(B[1], HIGH); 
    delay(500);
    break;
    case 4: 
    for (int i = 0; i < 4; i++) {
      digitalWrite(B[i], HIGH);
    }
    delay(500);
    break;
   case 5: 
    digitalWrite(B[3], HIGH); 
    digitalWrite(B[2], HIGH);
    digitalWrite(B[1], HIGH); 
    delay(500);
    break;
    case 6:
    digitalWrite(B[3], HIGH); 
    digitalWrite(B[2], HIGH);
    delay(500);
    break;
    case 7: 
    digitalWrite(B[3], HIGH);
    delay(500);
    break;
    default: 
    break; 
}
}
void loop(){
    
  // put your main code here, to run repeatedly:
  int On = digitalRead(y); 
  int Off = digitalRead(x);
  if(Off == HIGH){
    counter++; 
    if(counter > 7){ 
      counter = 7;
    } 
     lightstage(); 
  }
  if(On == LOW){ 
    counter--; 
    if(counter < 0){ 
      counter = 0; 
    }
    lightstage(); 
  }
}
