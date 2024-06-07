#include <Arduino.h>
#include <MultiMAX6675.h>

#define SPI_SCLK 27
#define SPI_MISO 25
#define TC1_CS 26
#define TC2_CS 14
#define TC3_CS 13

float temp1;
float temp2;
float temp3;

MultiMAX6675 tc;

void setup() {
  Serial.begin(115200);
  SPI.begin(SPI_SCLK, SPI_MISO);

  tc.bind(TC1_CS, &temp1, &SPI); // Bind a thermocouple CS Pin to a variable, they will update on their own, no need for users to manually read.
  tc.bind(TC2_CS, &temp2, &SPI, Unit::FAHRENHEIT);
  Thermocouple tc3 = tc.bind(TC3_CS, &temp3, &SPI, Unit::FAHRENHEIT);
  tc3.unit = Unit::FAHRENHEIT; // You can dynamically change units.
}

void loop() {
  tc.loop(); // call this as often as possible
}
