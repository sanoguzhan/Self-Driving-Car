//Left side of Motors
const int EnL = 5; 
const int HighL = 7;
const int LowL = 11;

//Right side of Motors
const int EnR = 6;
const int HighR = 9;
const int LowR = 8;

void setup() {
  pinMode(EnL, OUTPUT);
  pinMode(HighL, OUTPUT);
  pinMode(LowL, OUTPUT);
  pinMode(EnR, OUTPUT);
  pinMode(HighR, OUTPUT);
  pinMode(LowR, OUTPUT);
}

//Forward 
void Forward(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnL, 255);
  analogWrite(EnR, 255);

}

//Backward 
void Backward(){
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnL, 255);
  analogWrite(EnR, 255);

}

//Left

void Left(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnL,255);
  analogWrite(EnR, 10);

}
void Right(){
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnL, 10);
  analogWrite(EnR,255);

}

void Right_soft(){
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
      delay(1000);
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
    delay(200);

 }

 void Right_medium(){
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
  delay(600);
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
  delay(200);

 }
 void Left_soft(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
      delay(1000);
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
    delay(200);

 }

 void Left_medium(){
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
  delay(600);
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnR,255);
  analogWrite(EnL,255);
  delay(200);

 }



void loop() {
  //Forward();
  //Backward();

  //Left();
  //Right();
  //trial();
  //Right_medium();

}
