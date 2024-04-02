                                                                                                                                                                                                                                                                                                                                    int B[4] = {2, 3, 4, 5};
int pot = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pot, INPUT); 
  for (int i = 0; i < 4; i++) {
    pinMode(B[i], OUTPUT);
 }
  Serial.begin(9600);
}


void loop(){ 
  int potRead = analogRead(pot); 
  int lightstage = map(potRead, 0, 1023, 0, 16); 
  //Serial.println(lightstage, BIN);
  Serial.print((lightstage >> 0) & 1);
  Serial.print((lightstage >> 1) & 1);
  Serial.print((lightstage >> 2) & 1);
  Serial.println((lightstage >> 3) & 1);
  for(int i = 0; i < 4; i++){ 
    digitalWrite(B[i], (lightstage >> i) & 1);
  } 
  delay(200); 
}
                     
                                                                                                            
