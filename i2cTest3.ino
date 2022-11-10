#include "DFRobot_OxygenSensor.h"
#define COLLECT_NUMBER    10             // range: 1-100.
#define Oxygen_IICAddress ADDRESS_3

DFRobot_OxygenSensor Oxygen;
void setup() {
  Serial.begin(9600);
  while(!Oxygen.begin(Oxygen_IICAddress)) {
    Serial.println("I2c error");
    delay(1000);
  }
  Serial.println("I2c conected");
}

void loop() {
  float o2 = Oxygen.getOxygenData(COLLECT_NUMBER);
  Serial.print("O2 concentration is ");
  Serial.print(o2);
  Serial.println(" %vol");
  delay(1000);
}
