#include <SoftwareSerial.h>
unsigned char buffer_RTT[4] = {0};
uint8_t CS;
#define COM 0x55
int Distance = 0;
SoftwareSerial mySerial1(8, 9); // adjust to reflect input pins of sonar sensor in Arduino 
SoftwareSerial mySerial2(10, 11); // adjust to reflect input pins of sonar sensor in Arduino 
SoftwareSerial mySerial3(12, 13); // adjust to reflect input pins of sonar sensor in Arduino 
void setup() {
  Serial.begin(115200);
  mySerial1.begin(115200);
  mySerial2.begin(115200);
  mySerial3.begin(115200);
}
void loop() {
  mySerial1.write(COM);
  mySerial2.write(COM);
  mySerial3.write(COM);
  delay(100);
  if(mySerial1.available() > 0){
    delay(4);
    if(mySerial1.read() == 0xff){    
      buffer_RTT[0] = 0xff;
      for (int i=1; i<4; i++){
        buffer_RTT[i] = mySerial1.read();   
      }
      CS = buffer_RTT[0] + buffer_RTT[1]+ buffer_RTT[2];  
      if(buffer_RTT[3] == CS) {
        Distance = (buffer_RTT[1] << 8) + buffer_RTT[2];
        Serial.print("Distance:");
        Serial.print(Distance);
        Serial.println("mm");
      }
    }
  }
  if(mySerial2.available() > 0){
    delay(4);
    if(mySerial1.read() == 0xff){    
      buffer_RTT[0] = 0xff;
      for (int i=1; i<4; i++){
        buffer_RTT[i] = mySerial2.read();   
      }
      CS = buffer_RTT[0] + buffer_RTT[1]+ buffer_RTT[2];  
      if(buffer_RTT[3] == CS) {
        Distance = (buffer_RTT[1] << 8) + buffer_RTT[2];
        Serial.print("Distance:");
        Serial.print(Distance);
        Serial.println("mm");
      }
    }
  }
  if(mySerial3.available() > 0){
    delay(4);
    if(mySerial3.read() == 0xff){    
      buffer_RTT[0] = 0xff;
      for (int i=1; i<4; i++){
        buffer_RTT[i] = mySerial3.read();   
      }
      CS = buffer_RTT[0] + buffer_RTT[1]+ buffer_RTT[2];  
      if(buffer_RTT[3] == CS) {
        Distance = (buffer_RTT[1] << 8) + buffer_RTT[2];
        Serial.print("Distance:");
        Serial.print(Distance);
        Serial.println("mm");
      }
    }
  }
}