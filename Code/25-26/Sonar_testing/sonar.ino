#include <NewPing.h>

#define SONAR_NUM 1
#define MAX_DISTANCE 400

NewPing sonar[SONAR_NUM] = {
  NewPing(8, 9, MAX_DISTANCE)  
};


void setup() { Serial.begin(115200); }

void loop() {
  for (int i = 0; i < SONAR_NUM; i++) {
    delay(50);
    Serial.print(i);
    Serial.print("=");
    Serial.print(sonar[i].ping_cm());
    Serial.print("cm ");
  }

  Serial.println();
}
