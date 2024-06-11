#ifndef MULTIMAX6675_H
#define MULTIMAX6675_H

#include <Arduino.h>
#include <SPI.h>
#include <vector>

struct Thermocouple {
  enum Unit { CELCIUS, FAHRENHEIT };
  Unit unit;
  uint8_t pin;
  SPIClass * spi;
  float * var;
};

class MultiMAX6675 {
  public:
    Thermocouple bind(uint8_t pin, float * var, SPIClass * spi, Thermocouple::Unit unit = Thermocouple::Unit::CELCIUS);
    void loop();
  private:
    std::vector<Thermocouple> thermocouples;
    const unsigned long conversion_duration_ms = 220;
};

#endif
