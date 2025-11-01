const int led1Pin = 8;
const int led2Pin = 9;
const int buttonPin = 2;

int buttonState = 0;
int lastButtonState = 0;
unsigned long buttonReleaseTime = 0;
bool waitingForDelay = false;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, LOW);
    waitingForDelay = false;
  }

  if (buttonState == LOW && lastButtonState == HIGH) {
    digitalWrite(led1Pin, LOW);
    buttonReleaseTime = millis();
    waitingForDelay = true;
  }
}
