#include "MultiMAX6675.h"

int MultiMAX6675::bind(uint8_t pin, float * var, SPIClass * spi, Thermocouple::Unit unit) {
  Thermocouple * new_tc_arr = new Thermocouple[this->size + 1];
  for(int i = 0; i < size; i++) new_tc_arr[i] = thermocouples[i];
  new_tc_arr[size] = { .unit = unit, .pin = pin, .spi = spi, .var = var };
  delete[] thermocouples;
  thermocouples = new_tc_arr;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  return size++;
}

void MultiMAX6675::setUnit(int index, Thermocouple::Unit unit) {
  thermocouples[index].unit = unit;
}

void MultiMAX6675::loop() {
  static unsigned long read_time;
  static bool is_data_ready = false, is_reading = false;
  if(!this->size) return;
  if(!is_reading && !is_data_ready) {
    is_reading = true;
    read_time = millis();
  } else if(is_reading && !is_data_ready && ((millis() - read_time) > this->conversion_duration_ms)) {
    is_reading = false, is_data_ready = true;
  } else if(!is_reading && is_data_ready) {
    for(int i = 0; i < this->size; i++) {
      digitalWrite(thermocouples[i].pin, LOW);
      thermocouples[i].spi->beginTransaction(SPISettings(4300000, MSBFIRST, SPI_MODE0));
      // [bit, description] - [0, dummy sign bit][1-12, the data we want, a 12-bit unsigned number(0-4096)][13, thermocouple input][14, device id][15, state]
      // I have no idea what the last 3 bits are for, so just shift them. lol 
      uint16_t raw_temperature = thermocouples[i].spi->transfer16(0) >> 3;
      thermocouples[i].spi->endTransaction();
      digitalWrite(thermocouples[i].pin, HIGH);
      // °F = (1.8 × °C) + 32, the unit of the raw value is in °C, in increments of .25°C. °C = n / 4
      // so, we can just divide the constant, 1.8, by 4 for 1 less calculation.
      *thermocouples[i].var = thermocouples[i].unit == Thermocouple::Unit::CELCIUS ? raw_temperature / 4.0 : ((0.45 * raw_temperature) + 32);
    }
    is_data_ready = false;
  }
}
