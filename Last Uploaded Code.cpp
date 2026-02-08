#include <Arduino.h>
#include <OneWire.h>
#include <DS18B20.h>
#include <PID_v1.h>

// === DS18B20 Setup ===
#define DS18B20_PIN PA4
#define SENSOR_DISCONNECTED 999 // Error code for disconnected sensor
DS18B20 thermocouple(DS18B20_PIN);
double inputTemp, outputPWM, setpoint = 15.0; // Default setpoint for cooling
double Kp = 15.0, Ki = 0.5, Kd = 1.0;

// For cooling, the PID controller should be in REVERSE mode.
// This means that as the temperature gets farther *above* the setpoint, the output will increase.
PID pid(&inputTemp, &outputPWM, &setpoint, Kp, Ki, Kd, REVERSE);

// === Rotary Encoder Setup ===
volatile int encoderPos = 15;  // Initial setpoint for cooling
volatile bool buttonPressed = false;
uint32_t lastButtonPress = 0; // For debouncing
#define SW_PIN   PC13
#define DT_PIN   PC14
#define CLK_PIN  PC15

// === Motor Driver (Peltier) Output Pins ===
// Using L298N or similar motor driver
#define EN PA7
#define RPWM PB0 // Typically for one direction (e.g., heating)
#define LPWM PB1 // Typically for the other direction (e.g., cooling)

void readEncoder();
void onButtonPress();
void get_temperature();

void setup() {
  Serial.begin(115200);

  // DS18B20 Init
  while (!thermocouple.begin()) {
    Serial.println("DS18B20 not found!");
    delay(1000);
  }
  thermocouple.startTemperatureConversion();

  // Encoder Init
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLK_PIN), readEncoder, FALLING);
  attachInterrupt(digitalPinToInterrupt(SW_PIN), onButtonPress, FALLING);

  // Motor Driver (Peltier) Pins
  pinMode(EN, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);

  // Initialize motor driver to be off
  digitalWrite(EN, LOW);
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);

  // PID Configuration
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(0, 255); // PWM range from 0 to 255
}

void loop() {
  // --- Temperature Reading ---
    get_temperature();
    thermocouple.startTemperatureConversion(); // Start next conversion immediately

  // --- Setpoint Update ---
  // It's generally safer to handle flag checks outside of interrupts.
  if (buttonPressed) {
    noInterrupts(); // Disable interrupts to safely update the setpoint
    setpoint = encoderPos;
    interrupts();   // Re-enable interrupts
    
    buttonPressed = false; // Reset the flag
    
    Serial.print("New Setpoint: ");
    Serial.println(setpoint);
  }

  // --- PID Calculation ---
  pid.Compute(); // Run PID control algorithm

  // --- Peltier Control for Cooling ---
  // To cool, we need to drive the Peltier in the correct direction.
  // We'll use the LPWM pin for cooling and keep RPWM at 0.
  // The outputPWM from the PID will control the cooling power.
  digitalWrite(EN, HIGH); // Enable the motor driver
  analogWrite(LPWM, (int)outputPWM); // Apply cooling power
  analogWrite(RPWM, 0); // Ensure heating side is off
  
  // --- Serial Debugging ---
  Serial.print("Temp: ");
  Serial.print(inputTemp);
  Serial.print(" °C | Setpoint: ");
  Serial.print(setpoint);
  Serial.print(" °C | Cooling Power (PWM): ");
  Serial.println(outputPWM);

  delay(1000); // Delay for stability and readability
}

void get_temperature() {
  inputTemp = thermocouple.getTemperature(0);
  if (inputTemp == SENSOR_DISCONNECTED) {
    Serial.println("Error: Temperature sensor disconnected!");
    inputTemp = setpoint; // To prevent PID from maxing out on error
  }
}

void readEncoder() {
  // This basic encoder reading can be prone to noise.
  // A more robust implementation would involve state machine logic.
  if (digitalRead(DT_PIN) == HIGH) {
    encoderPos++;
  } else {
    encoderPos--;
  }
  // Adjust the range for cooling temperatures
  encoderPos = constrain(encoderPos, 5, 35); // e.g., 5°C to 35°C
}

void onButtonPress() {
  // Set a flag to be handled in the main loop.
  // Avoid complex logic inside an ISR.
  if (millis() - lastButtonPress > 200) {
    buttonPressed = true;
    lastButtonPress = millis();
  }
}