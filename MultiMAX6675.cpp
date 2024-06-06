#include "MultiMAX6675.h"

void MultiMAX6675::begin(SPIClass * spi) {
  current_millis = millis();
  this->spi = spi;
}

void MultiMAX6675::loop() {
  current_millis = millis();
  std::size_t queue_size = read_queue.size();
  if(queue_size == 0) return;
  for(uint16_t i = read_queue_free_on_beginning; i < queue_size; i++) {
    QueueItem item = read_queue[i];
    if(current_millis >= item.read_on) {
      digitalWrite(item.pin, LOW);
      spi->beginTransaction(SPISettings(4300000, SPI_MSBFIRST, SPI_MODE0));
      // [bit, description] - [0, dummy sign bit][1-12, the data we want, a 12-bit unsigned number(0-4096)][13, thermocouple input][14, device id][15, state]
      // I have no idea what the last 3 bits are for, so just shift them. lol 
      float temperature = (spi->transfer16(0) >> 3) / 4.0;
      spi->endTransaction();
      digitalWrite(item.pin, HIGH);
      *item.var = item.unit == Unit::CELCIUS ? temperature : ((1.8 * temperature) + 32);
      if(queue_size - 1 == i) {
        read_queue.pop_back();
      } else {
        read_queue_free_on_beginning = i;
      }
    }
  }
}

unsigned int MultiMAX6675::add(uint8_t CS_PIN) {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  cs_pins.push_back(CS_PIN);
  return cs_pins.size() - 1;
}

void MultiMAX6675::read(Unit unit, uint8_t pin_index, float * var) {
  digitalWrite(cs_pins[pin_index], LOW);
  digitalWrite(cs_pins[pin_index], HIGH);
  // delay(220);
  QueueItem item = QueueItem { .read_on = current_millis + 220, .pin = cs_pins[pin_index], .var = var, .unit = unit};
  if(read_queue_free_on_beginning == 0) {
    read_queue.push_back(item);
  } else {
    read_queue[read_queue_free_on_beginning] = item;
    read_queue_free_on_beginning--;
  }
}
