#include <Arduino.h>
#include "adxl375.h"
// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Enabling serial communication
  ADXL375_init(); // Initializing the ADXL375 accelerometer

}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}