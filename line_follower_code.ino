//motor one
#define A1 5
#define A 6

//motor two
#define B1 9
#define B 10

//sensor pins
#define SL 3
#define SR 2

int mSpeed = 130;//motor speed

void setup() {
  pinMode(A1, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(B, OUTPUT);

  pinMode(SL, INPUT);
  pinMode(SR, INPUT);
  Serial.begin(9600);
}

void loop() {
  bool LValue = digitalRead(SL);//get sensor value one
  bool RValue = digitalRead(SR);//get sensor value two

  Serial.print(LValue);
  Serial.println(RValue);

  if (RValue == 1 && LValue == 0 ) {
    turnRight();
    Serial.println("turnRight");
  } else if (LValue == 1 && RValue == 0) {
    turnLeft();
    Serial.println("turnLeft");
  } else if (LValue == 0 && RValue == 0) {
    forward();
    Serial.println("forward");
  } else if (LValue == 1 && RValue == 1) {
    Stop();
    Serial.println("stop");
  }

}
void turnRight() {
  analogWrite(B1, 140);
  analogWrite(B, 0);
  analogWrite(A1, 0);
  analogWrite(A, 0);
}
void turnLeft() {
  analogWrite(B1, 0);
  analogWrite(B, 0);
  analogWrite(A1, 140);
  analogWrite(A, 0);
}
void forward() {
  analogWrite(B1, 140);
  analogWrite(B, 0);
  analogWrite(A1, 140);
  analogWrite(A, 0);
}
void Stop() {
  analogWrite(A1, LOW);
  analogWrite(A, LOW);
  analogWrite(B1, LOW);
  analogWrite(B, LOW);
}