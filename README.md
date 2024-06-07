# MultiMAX6675
---
### A simple Arduino library that allows you to **non-blocking(ly)** read multiple MAX6675 K-Type Thermocouple Sensors.

## **Library is still in-progress, if you get any bugs, please open an issue.**

## Todo
### - add errors when thermocouple is not connected.
### - optimizations

## Example Usage
```c++
#include <Arduino.h>
#include <MultiMAX6675.h>

#define SPI_SCLK 27
#define SPI_MISO 25
#define TC1_CS 26
#define TC2_CS 14

float temp1;
float temp2;

MultiMAX6675 tc;

void setup() {
  Serial.begin(115200);
  SPI.begin(SPI_SCLK, SPI_MISO);

  // Chip Select Pin - Pointer to a float, which will store the temperature, Pointer to an SPIClass
  tc.bind(TC1_CS, &temp1, &SPI); // Bind a thermocouple CS Pin to a variable, they will update on their own, no need for users to manually read.
  tc.bind(TC2_CS, &temp2, &SPI, Unit::FAHRENHEIT); // They can also use different SPIClass objects now, unlike my previous version.
}

void loop() {
  tc.loop(); // call this as often as possible
}
```
