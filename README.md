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
#include <SPI.h>
#include <MultiMAX6675.h>

// Pins, modify accordingly.

#define SPI_SCLK 27
#define SPI_MISO 25
#define TC1_CS 26
#define TC2_CS 14

MultiMAX6675 thermocouples;

uint8_t thermocouple2_index;
uint8_t thermocouple1_index;

void setup() {
  Serial.begin(9600);
  // Initialize SPI, or create your own SPIClass.
  // It is assumed that they all share the same SPI Bus.
  SPI.begin(SPI_SCLK, SPI_MISO);
  thermocouples.begin(&SPI); // Takes a pointer to an SPIClass

  thermocouple1_index = thermocouples.add(TC1_CS); // Takes a CS(Chip Select) pin of the thermocouple, returns an index. Either save the index to a variable, or you don't have to if you already know the order.
  thermocouple2_index = thermocouples.add(TC2_CS);
}

float thermocouple1_temperature;
float thermocouple2_temperature;

unsigned long prev = 0;

void loop() {
  // [Unit::CELCIUS, Unit::FAHRENHEIT][Thermocouple Index, look above][Pointer to a float, where you want to save the temperature.]
  // You can ONLY read every 250ms(preferably a bit more), in order to not run out of memory(?), due to sensor limitation, resulting in a crash. Use millis() to do so, otherwise, what's point of this library. xD
  if(millis() >= (prev + 300)) {
    prev = millis();
    thermocouples.read(Unit::CELCIUS, thermocouple1_index, &thermocouple1_temperature);
    thermocouples.read(Unit::FAHRENHEIT, thermocouple2_index, &thermocouple2_temperature);
    Serial.println(thermocouple1_temperature);
    Serial.println(thermocouple2_temperature);
  }
  thermocouples.loop(); // Call this as often as possible.
}
```
