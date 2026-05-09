#include <SoftwareSerial.h>
#include <Servo.h>

Servo testServo;

unsigned char buffer_RTT[4] = {0};
uint8_t CS;
#define COM 0x55

int Distance = -1, lastStableDistance = -1;
unsigned long stableStartTime = 0;

SoftwareSerial mySerial(8, 9);
 
void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);

  testServo.attach(7); // TODO Change this
}

void loop() {
  mySerial.write(COM); // COM = 0X55
  delay(100);
 
  if (mySerial.available() > 0) {
    delay(4);
    if (mySerial.read() == 0xff) {
      buffer_RTT[0] = 0xff;
      for (int i = 1; i < 4; i++) {
        buffer_RTT[i] = mySerial.read();
      }
      CS = buffer_RTT[0] + buffer_RTT[1] + buffer_RTT[2];
 
      if (buffer_RTT[3] == CS) {
        int newDistance = (buffer_RTT[1] << 8) + buffer_RTT[2];
 
        if (Distance == -1) {  
          // First valid reading, set immediately
          Distance = newDistance;
          lastStableDistance = newDistance;
          stableStartTime = millis();
        } else if (newDistance == Distance) {
          // Check if stable for 500ms
          if (millis() - stableStartTime >= 500 && newDistance != lastStableDistance) {
            lastStableDistance = newDistance;
          }
        } else {
          // Reset stability timer on change
          Distance = newDistance;
          stableStartTime = millis();
        }

        //testServo.write(sonarDistanceToDegrees(Distance))
        Serial.print("Distance:");
        Serial.print(Distance);
        Serial.println("mm");
      }
    }
  }
}

int sonarDistanceToDegrees(int distance) {
  // TODO whatever the formula is

  return 0;
}
