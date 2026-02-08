#include <Arduino.h>

volatile int encoderPos = 0;
volatile bool buttonPressed = false;

#define CLK_PIN  PC14
#define DT_PIN   PC15
#define SW_PIN   PC13

void readEncoder();
void onButtonPress();

void setup() {
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CLK_PIN), readEncoder, FALLING);
  attachInterrupt(digitalPinToInterrupt(SW_PIN), onButtonPress, FALLING);

  Serial.begin(115200);
  Serial.println("Serial Encoder");
}

void loop() {
  static int lastPos = 0;

  if (encoderPos != lastPos) {
    Serial.print("Encoder: ");
    Serial.println(encoderPos);
    lastPos = encoderPos;
  }

  if (buttonPressed) {
    buttonPressed = false;
    Serial.println("Button Pressed!");
  }
}

void readEncoder() {
  if (digitalRead(DT_PIN) == HIGH)
    encoderPos++;
  else
    encoderPos--;
}

void onButtonPress() {
  buttonPressed = true;
}
