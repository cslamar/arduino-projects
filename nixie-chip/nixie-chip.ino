int PIN_A = 4;
int PIN_B = 5;
int PIN_C = 6;
int PIN_D = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_D, OUTPUT);

  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  reset_pins();

  display_one();
  delay(1000);
  display_two();
  delay(1000);
  display_three();
  delay(1000);
  display_four();
  delay(1000);
  display_five();
  delay(1000);
  display_six();
  delay(1000);
  display_seven();
  delay(1000);
  display_eight();
  delay(1000);
  display_nine();
  delay(1000);
  display_zero();
  delay(1000);  
}

void reset_pins(){
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, LOW);
}

void display_one(){
  reset_pins();
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, LOW);
}

void display_two(){
  reset_pins();
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, LOW);
}

void display_three(){
  reset_pins();
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, LOW);
}

void display_four(){
  reset_pins();
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_D, LOW);
}

void display_five(){
  reset_pins();
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_D, LOW);
}

void display_six(){
  reset_pins();
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_D, LOW);
}

void display_seven(){
  reset_pins();
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_D, LOW);
}

void display_eight(){
  reset_pins();
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, HIGH);
}

void display_nine(){
  reset_pins();
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, HIGH);
}

void display_zero(){
  reset_pins();
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, LOW);
}
