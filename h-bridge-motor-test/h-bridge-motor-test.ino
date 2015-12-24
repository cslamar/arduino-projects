// 1000 is the stopping point
#define stoppingPoint 1200

// Motor Pins Left
int lMotorPin1 = 9;
int lMotorPin2 = 10;
// Motor Pins Right
int rMotorPin1 = 6;
int rMotorPin2 = 5;

// Ultra Sonic Sensor pins
int triggerPin = 7;
int echoPin = 8;

// Arming button
int stopButton = 2;
int armingLed = 4;

// Misc Pins
int buzzerPin = 13;

// Additional data
int value = 0;

volatile bool moving = false;
volatile bool enabled = false;

void setup() {
  Serial.begin(9600);

  // USS initialization
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);

  // Motor initialization
  pinMode(lMotorPin1, OUTPUT);
  pinMode(lMotorPin2, OUTPUT);
  pinMode(rMotorPin1, OUTPUT);
  pinMode(rMotorPin2, OUTPUT);

  // Stop all motors
  digitalWrite(lMotorPin1, LOW);
  digitalWrite(lMotorPin2, LOW);
  digitalWrite(rMotorPin1, LOW);
  digitalWrite(rMotorPin2, LOW);

  // Misc initialization
  pinMode(buzzerPin, OUTPUT);

  // Arming LED and button
  pinMode(armingLed, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(stopButton), toggleArmed, RISING);
  digitalWrite(armingLed, !enabled);

  tone(buzzerPin, 262);
  delay(200);
  Serial.println("Beep");
  
  tone(buzzerPin, 294);
  delay(150);
  Serial.println("Beep");
  
  delay(150);
  tone(buzzerPin, 330, 200);
  Serial.println("Beep");

  Serial.println("Armed and Ready to Roll!");
}

void loop() {
  digitalWrite(armingLed, !enabled);
/* Ramp a direction up and down
  rampUpAndDown(motorPin1);
  delay(3000);
  rampUpAndDown(motorPin2);
  delay(3000);
*/

/* Stopping test
  delay(2000);
  digitalWrite(motorPin1, HIGH);
  delay(4000);
  stopSuddenly();
*/

/* Button Test
  value = digitalRead(stopButton);
  if(value) {
    Serial.println(value);
  }
*/

/* Interrupt Stop Test
  digitalWrite(motorPin1, HIGH);
  if(stopNow) {
    Serial.println("Stopping!!");
    stopSuddenly();
    delay(3000);
    stopNow = false;
  }
*/

  if(enabled) {
    if(detection() && enabled) {
      forwardSpeed(250);
      moving = true;
    } else if (moving && enabled) {
      stopSuddenly();
    } else if(!detection() && !moving){
      reverseAction();
    } else {
      killAll();
    }
  }
}

void forwardSpeed(int motorSpeed) {
  analogWrite(lMotorPin1, motorSpeed);
  analogWrite(rMotorPin1, motorSpeed);
}

void reverseSpeed(int motorSpeed) {
  analogWrite(lMotorPin2, motorSpeed);
  analogWrite(rMotorPin2, motorSpeed);
}

/*
void backAndForth() {
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, HIGH);
  delay(4000);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  delay (4000);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  delay(1000);
}
*/

/*
void rampUpAndDown(int motorDirection){
  killAll();
  for( int i = 0; i < 255; i++){
    analogWrite(motorDirection, i);
    Serial.print("Speed: ");Serial.println(i);
    delay(50);
  }
  for( int i = 255; i >= 0; i--){
    analogWrite(motorDirection, i);
    Serial.print("Speed: ");Serial.println(i);
    delay(50);
  }
  killAll();
}
*/
void killAll() {
  digitalWrite(lMotorPin1, LOW);
  digitalWrite(lMotorPin2, LOW);
  digitalWrite(rMotorPin1, LOW);
  digitalWrite(rMotorPin2, LOW);
  moving = false;
}

void stopSuddenly() {
//  Serial.println("STOPPING!!");
  digitalWrite(lMotorPin1, LOW);
  digitalWrite(rMotorPin1, LOW);
  digitalWrite(lMotorPin2, HIGH);
  digitalWrite(rMotorPin2, HIGH);

  delay(175);
  killAll();
}

bool detection() {
  long duration = 0;
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  Serial.print("USS Duration: "); Serial.println(duration);

  if(duration < stoppingPoint) {
    return false;
  } else {
    return true;
  }
}

void toggleArmed() {
  enabled = !enabled;
}

void reverseAction() {
  reverseSpeed(120);
  tone(buzzerPin, 330, 300);
  delay(500);
}

