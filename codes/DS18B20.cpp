#include <Arduino.h>
#include <OneWire.h>
#include <DS18B20.h>

#define DS18B20_PIN PA4

DS18B20 thermocouple(DS18B20_PIN);

void get_temparature();

struct Data
{
  float ds18b20[2];
} data;

void setup()
{
  Serial.begin(115200);

  thermocouple.begin();
  if (!thermocouple.startTemperatureConversion())
  {
    Serial.print("DS18B20: Error starting measurements, DS18B20 not responding for start tempareture\n");
  }
}

void loop()
{
  get_temparature();
  delay(1000);
}

void get_temparature()
{
  data.ds18b20[0] = thermocouple.getTemperature(0);
  data.ds18b20[1] = thermocouple.getTemperature(1);

  if (data.ds18b20[0] == 999)
  {
    Serial.print("DS18B20: Error reading stratch pad of Sensor 1\n");
  }

  if (data.ds18b20[1] == 999)
  {
    Serial.print("DS18B20: Error reading stratch pad of Sensor 2\n");
  }

  Serial.print("DS18B20: 1: ");
  Serial.print(data.ds18b20[0]);
  Serial.print(" C, 2: ");
  Serial.print(data.ds18b20[1]);
  Serial.println(" C");
  if (!thermocouple.startTemperatureConversion())
  {
    Serial.print("DS18B20: Error starting measurements, DS18B20 not responding for start tempareture\n");
  }
}