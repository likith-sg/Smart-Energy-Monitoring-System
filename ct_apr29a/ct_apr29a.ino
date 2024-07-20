#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3TUQcNJid"
#define BLYNK_TEMPLATE_NAME "Smart Plug"
#define BLYNK_AUTH_TOKEN "-i7gpy-gz8w3U8zkZe4H59-n3Nyc4Omk"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "EmonLib.h"
#include <Wire.h> // Include Emon Library

EnergyMonitor emon1; // Create an instance of the EnergyMonitor class

char auth[] = "-i7gpy-gz8w3U8zkZe4H59-n3Nyc4Omk"; // Blynk Auth Token
char ssid[] = "wolfce"; // Your WiFi Name
char pass[] = "abcde123"; // Your WiFi Password

#define RELAY_PIN 18 // Define the pin connected to the relay
#define CT_PIN 33 // Define the pin connected to the CT sensor

BlynkTimer timer;
float powerConsumption = 0.0;
float cumulativePower = 0.0;
bool relayState = false;
float maxPower = 1000.0; // Maximum power consumption threshold

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Initially turn off the relay
  relayState = false;

  // Setup a virtual pin to control the relay
  Blynk.virtualWrite(V2, LOW); // Set the initial state of the widget in the Blynk app

  // Setup a virtual pin to receive commands from the Blynk app
  Blynk.virtualWrite(V3, 0); // Set the initial state of the widget in the Blynk app

  // Assign a function to be called when the virtual pin state changes
  timer.setInterval(100L, checkVirtualPin);
  emon1.current(CT_PIN, 1); // Current: input pin, calibration.
}

void loop() {
  Blynk.run();
  timer.run();
}

// Function to control the relay based on the value received from the Blynk app
void relayControl(int value) {
  if (value == HIGH) {
    digitalWrite(RELAY_PIN, LOW); // Turn off the relay
    relayState = false;
    Blynk.virtualWrite(V2, LOW); // Update V2 to show relay is OFF
    Serial.println("Relay turned OFF");
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Turn on the relay
    relayState = true;
    Blynk.virtualWrite(V2, HIGH); // Update V2 to show relay is ON
    Serial.println("Relay turned ON");
  }
}

void checkVirtualPin() {
  int value = digitalRead(RELAY_PIN); // Read the current state of the relay pin
  Blynk.virtualWrite(V2, value); // Update the Blynk app with the current state

  if (relayState == false) {
    Blynk.virtualWrite(V2, cumulativePower); // Update V2 with cumulative power consumption
    cumulativePower = 0.0; // Reset cumulative power when relay is switched off
    Serial.println("Cumulative power reset");
  }
}

// Function to measure power consumption
void measurePowerConsumption() {
  // Read the analog value from the CT sensor
  int sensorValue = analogRead(CT_PIN);

  double Irms = emon1.calcIrms(1480); // Calculate Irms only

  // Apply calibration factor to compensate for the stepped-down voltage
  float current = Irms; // Assuming 5V ADC reference voltage
  
  // Calculate power consumption using a formula specific to your CT sensor and circuit
  // For example, if your CT sensor provides current in Amperes, and you have the voltage, you can use P = V * I
  float voltage = 230.0; // Assuming line voltage of 220V
  powerConsumption = voltage * current;
  cumulativePower += powerConsumption; // Add current power consumption to cumulative power

  // Send live power consumption data to Blynk
  Blynk.virtualWrite(V4, powerConsumption);

  // Check if power consumption exceeds the maximum value
  if (relayState && (powerConsumption > maxPower)) {
    // Turn off the relay if power consumption exceeds the maximum value
    relayControl(HIGH);
    Serial.println("Power consumption exceeded maximum threshold. Relay turned OFF");
  }

  Serial.print("Power consumed: ");
  Serial.print(powerConsumption);
  Serial.println(" watts");
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  relayControl(value);
}
