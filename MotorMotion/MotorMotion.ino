//Left side of Motors
const int EnableL1 = 5; 
//const int EnableL2 = 10;
const int HighL = 7;
const int LowL = 11;

//Right side of Motors
const int EnableR1 = 6;
//const int EnableR2 = 4;
const int HighR = 9;
const int LowR = 8;

void setup() {
  pinMode(EnableL1, OUTPUT);
  pinMode(HighL, OUTPUT);
  pinMode(LowL, OUTPUT);
  pinMode(EnableR1, OUTPUT);
  //pinMode(EnableR2, OUTPUT);
  //pinMode(EnableL2, OUTPUT);
  pinMode(HighR, OUTPUT);
  pinMode(LowR, OUTPUT);
}

//Forward 
void Forward(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnableL1, 255);
  //analogWrite(EnableL2, 255);
  analogWrite(EnableR1, 255);
  //analogWrite(EnableR2, 255);
}

//Backward 
void Backward(){
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL1, 255);
  //analogWrite(EnableL2, 255);
  analogWrite(EnableR1, 255);
  //analogWrite(EnableR2, 255);
}

//Left

void Left(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnableL1,255);
  //analogWrite(EnableL2, 255);
  analogWrite(EnableR1, 10);
  //analogWrite(EnableR2, 165);
}
void Right(){
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnableL1, 165);
  //analogWrite(EnableL2, 255);
  analogWrite(EnableR1,255);
  //analogWrite(EnableR2, 125);
}
void trial(){
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);

  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnableR1,255);

  analogWrite(EnableL1,255);
  delay(150);
   digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);

  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnableR1,255);

  analogWrite(EnableL1,255);

 }



void loop() {
  //Forward();
  //Backward();

  //Left();
  //Right();
  delay(1000);
  //trial();
  delay(1000);
}
