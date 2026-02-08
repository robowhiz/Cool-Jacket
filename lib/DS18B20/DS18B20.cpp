#include "DS18B20.h"

DS18B20::DS18B20(uint8_t pin) : oneWire(pin)
{
}

uint8_t DS18B20::begin()
{
    oneWire.reset_search();
    if (!oneWire.search(DS18B20_Address[0]))
        return 0;

    if (!oneWire.search(DS18B20_Address[1]))
        return 1;
    else
        return 2;
}

bool DS18B20::startTemperatureConversion()
{
    if (!oneWire.reset())
        return 0;
    oneWire.skip();
    oneWire.write(0x44);
    return 1;
}

bool DS18B20::readScratchpad(uint8_t deviceAddress[], uint8_t *data)
{
    if (!oneWire.reset())
        return 0;
    oneWire.select(deviceAddress);
    oneWire.write(0xBE); // Read Scratchpad

    data[0] = oneWire.read();
    data[1] = oneWire.read();
    oneWire.reset();
    return 1;
}

float DS18B20::getTemperature(uint8_t num)
{
    uint8_t data[2];

    if (!readScratchpad(DS18B20_Address[num], data))
        return 999;

    // Convert the data to actual temperature
    int16_t rawTemperature = (data[1] << 8) | data[0];
    float celsius = (float)rawTemperature / 16.0;

    return celsius;
}