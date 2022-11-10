#include "DFRobot_OxygenSensor.h"
#define COLLECT_NUMBER    10             // range: 1-100.
#define Oxygen_IICAddress ADDRESS_3

#include <SPI.h>
#include <SD.h>

File myFile;
DFRobot_OxygenSensor Oxygen;
void setup() {
// Open serial communications and wait for port to open:
Serial.begin(9600);
  while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
Serial.print("Initializing SD card...");
if (!SD.begin(10)) {
  Serial.println("initialization failed!");
  while (1);
}
Serial.println("initialization done.");
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.
myFile = SD.open("test.txt", FILE_WRITE);
// if the file opened okay, write to it:
if (myFile) {
  Serial.print("Writing to test.txt...");
  myFile.println("This is a test file :)");
  myFile.println("testing 1, 2, 3.");
  for (int i = 0; i < 20; i++) {
    myFile.println(i);
  }
  // close the file:
  myFile.close();
  Serial.println("done.");
} 
  else {
  // if the file didn't open, print an error:
  Serial.println("error opening test.txt");
  }
  
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
