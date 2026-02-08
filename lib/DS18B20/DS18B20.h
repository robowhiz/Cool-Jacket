#ifndef DS18B20_H
#define DS18B20_H

#include <OneWire.h>
#include <Arduino.h>

class DS18B20
{
public:
    DS18B20(uint8_t pin);
    uint8_t begin();
    bool startTemperatureConversion();
    bool readScratchpad(uint8_t deviceAddress[], uint8_t *data);
    float getTemperature(uint8_t num);

private:
    OneWire oneWire;
    uint8_t DS18B20_Address[2][8];
};

#endif