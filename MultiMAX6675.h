#ifndef MULTIMAX6675_H
#define MULTIMAX6675_H

#include <Arduino.h>
#include <SPI.h>
#include <vector>
#include <array>

enum Unit {
  CELCIUS,
  FAHRENHEIT
};

struct QueueItem {
  unsigned long read_on;
  uint8_t pin;
  float * var;
  Unit unit;
};

class MultiMAX6675 {
  public:
    void begin(SPIClass * spi);
    uint32_t add(uint8_t cs);
    void read(Unit unit, uint8_t pin_index, float * var);
    void loop();
  private:
    SPIClass * spi;
    std::vector<uint8_t> cs_pins;
    std::vector<QueueItem> read_queue;
    uint16_t read_queue_free_on_beginning = 0;
    unsigned long current_millis;
};

#endif
