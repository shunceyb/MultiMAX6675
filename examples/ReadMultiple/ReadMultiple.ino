#include <Arduino.h>
#include <MultiMAX6675.h>

#define TC1_CS 26
#define TC2_CS 14
#define TC3_CS 13

float temp1;
float temp2;
float temp3;

MultiMAX6675 tc;

void setup() {
  Serial.begin(115200);
  SPI.begin();
                            // SPIClass object
  tc.bind(TC1_CS, &temp1, &SPI); // Bind a thermocouple CS Pin to a variable, they will update on their own, no need for users to manually read.
  tc.bind(TC2_CS, &temp2, &SPI, Thermocouple::Unit::FAHRENHEIT);
  // MultiMAX6675::bind returns the index of the Thermocouple object in the array.
  // You can save this value to update the unit dynamically.
  // Or not, if you already know which thermocouple is in what index.
  int tc3_index = tc.bind(TC3_CS, &temp3, &SPI, Thermocouple::Unit::FAHRENHEIT); 
  tc.setUnit(tc3_index, Thermocouple::Unit::CELCIUS);
}

void loop() {
  tc.loop(); // call this as often as possible
}