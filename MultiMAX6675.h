#ifndef MULTIMAX6675_H
#define MULTIMAX6675_H

#include <Arduino.h>
#include <SPI.h>

struct Thermocouple {
  enum Unit { CELCIUS, FAHRENHEIT };
  Unit unit;
  uint8_t pin;
  SPIClass * spi;
  float * var;
};

class MultiMAX6675 {
  public:
    int bind(uint8_t pin, float * var, SPIClass * spi, Thermocouple::Unit unit = Thermocouple::Unit::CELCIUS);
    void loop();
    void setUnit(int index, Thermocouple::Unit unit);
  private:
    Thermocouple * thermocouples = nullptr;
    const unsigned long conversion_duration_ms = 220;
    int size = 0;
};

#endif
