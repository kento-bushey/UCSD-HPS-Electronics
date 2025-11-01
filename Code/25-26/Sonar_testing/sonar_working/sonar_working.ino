#include <NewPing.h>

#define SONAR_NUM 1
#define MAX_DISTANCE 1000

NewPing sonar[SONAR_NUM] = {
  NewPing(8, 9, MAX_DISTANCE)  
};


void setup() { Serial.begin(9600); }

void loop() {
  float distance = 0;
  for (int i = 0; i < SONAR_NUM; i++) {
    delay(750);
    //Serial.print(i);
    // Serial.print("=");
    distance = sonar[i].ping_cm()/100.0;
    if (distance == 0.00) {
      continue;
    }
    Serial.println(distance);


    //Serial.print(" m");
  }
}
