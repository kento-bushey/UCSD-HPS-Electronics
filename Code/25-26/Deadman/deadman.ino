const int buttonPin = 2; // Button connected to pin 2
const int led1Pin = 8;   // First LED connected to pin 8
const int led2Pin = 9;   // Second LED connected to pin 9

int buttonState = 0;
int lastButtonState = 0;
unsigned long buttonReleaseTime = 0;
bool waitingForDelay = false;

void setup() {
  // Initialize pins
  pinMode(buttonPin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  // Start with LEDs off
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
}

void loop() {
  // Read button state
  buttonState = digitalRead(buttonPin);

  // Check if button is pressed
  if (buttonState == HIGH) {
    digitalWrite(led1Pin, HIGH); // Turn on first LED
    digitalWrite(led2Pin, LOW);  // Turn off second LED
    waitingForDelay = false;     // Cancel any waiting delay
  }

  // Check if button was just released
  if (buttonState == LOW && lastButtonState == HIGH) {
    digitalWrite(led1Pin, LOW);   // Turn off first LED
    buttonReleaseTime = millis(); // Record the time
    waitingForDelay = true;       // Start waiting for delay
  }

  // Check if 5 seconds have passed since button release
  if (waitingForDelay && (millis() - buttonReleaseTime >= 5000)) {
    digitalWrite(led2Pin, HIGH); // Turn on second LED
    waitingForDelay = false;     // Stop waiting
  }

  // Save button state for next loop
  lastButtonState = buttonState;
}
