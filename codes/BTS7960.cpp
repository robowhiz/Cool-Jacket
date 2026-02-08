#include <Arduino.h>

#define RIS PA5
#define LIS PA6
#define EN PA7
#define RPWM PB0
#define LPWM PB1

struct receiver
{
  uint8_t en;
  uint8_t Rpwm;
  uint8_t Lpwm;
} r_data;

struct transmitter
{
  uint8_t Ris;
  uint8_t Lis;
} t_data;

void setup()
{
    Serial.begin(115200);

    pinMode(RIS, INPUT_ANALOG);
    pinMode(LIS, INPUT_ANALOG);
    pinMode(EN, OUTPUT);
    pinMode(RPWM, OUTPUT);
    pinMode(LPWM, OUTPUT);
}

void loop()
{
    t_data.Ris = analogRead(RIS);
    t_data.Lis = analogRead(LIS);

    digitalWrite(EN, LOW);
    analogWrite(RPWM, r_data.Rpwm);
    analogWrite(LPWM, r_data.Lpwm);
    digitalWrite(EN, HIGH);
}