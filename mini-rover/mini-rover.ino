int leftPin = 9;
int rightPin = 11;
int triggerPin = 12;
int echoPin = 13;

int stoppingPoint = 1000; // 1000 is the stopping point



void setup() {
  // put your setup code here, to run once:
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  if(detection()) {
    goForward();
  } else {
    stopMoving();
  }
  delay(50);
}

void goForward() {
  analogWrite(rightPin, 100);
  analogWrite(leftPin, 100);
}

void stopMoving() {
  analogWrite(rightPin, 0);
  analogWrite(leftPin, 0);
}

bool detection() {
  long duration = 0;
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  Serial.print("Duration: ");
  Serial.println(duration);

  if(duration < stoppingPoint) {
    return false;
  } else {
    return true;
  }
}

