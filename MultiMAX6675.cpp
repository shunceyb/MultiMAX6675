#include "MultiMAX6675.h"

Thermocouple MultiMAX6675::bind(uint8_t pin, float * var, SPIClass * spi, Unit unit) {
  Thermocouple tc = { .unit = unit, .pin = pin, .spi = spi, .var = var };
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  thermocouples.push_back(tc);
  return tc;
}

void MultiMAX6675::loop() {
  std::size_t size = this->thermocouples.size();
  static unsigned long read_time;
  static bool is_data_ready = false, is_reading = false;
  if(!size) return;
  if(!is_reading && !is_data_ready) {
    is_reading = true;
    read_time = millis();
  } else if(is_reading && !is_data_ready && ((millis() - read_time) > this->conversion_duration_ms)) {
    is_reading = false, is_data_ready = true;
  } else if(!is_reading && is_data_ready) {
    for(std::size_t i = 0; i < size; i++) {
      digitalWrite(thermocouples[i].pin, LOW);
      thermocouples[i].spi->beginTransaction(SPISettings(4300000, SPI_MSBFIRST, SPI_MODE0));
      // [bit, description] - [0, dummy sign bit][1-12, the data we want, a 12-bit unsigned number(0-4096)][13, thermocouple input][14, device id][15, state]
      // I have no idea what the last 3 bits are for, so just shift them. lol 
      uint16_t raw_temperature = thermocouples[i].spi->transfer16(0) >> 3;
      thermocouples[i].spi->endTransaction();
      digitalWrite(thermocouples[i].pin, HIGH);
      *thermocouples[i].var = thermocouples[i].unit == Unit::CELCIUS ? raw_temperature / 4.0 : ((0.45 * raw_temperature) + 32);
    }
    is_data_ready = false;
  }
}