#define BLYNK_PRINT Serial

#include "EmonLib.h"
#include <Wire.h>
EnergyMonitor emon1;

#define BLYNK_TEMPLATE_ID "TMPL3TUQcNJid"
#define BLYNK_TEMPLATE_NAME "Smart Plug"
#define BLYNK_AUTH_TOKEN "-i7gpy-gz8w3U8zkZe4H59-n3Nyc4Omk"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "-i7gpy-gz8w3U8zkZe4H59-n3Nyc4Omk"; // Blynk Auth Token
char ssid[] = "wolfce"; // Your WiFi Name
char pass[] = "abcde123"; // Your WiFi Password

#define RELAY_PIN 18
#define CT_PIN 33          // Define the pin connected to the relay            // Define the pin connected to the CT sensor
#define MEASUREMENT_INTERVAL 100 // Interval in milliseconds to measure power consumption

float maxPower = 1000.0; // Default maximum power consumption
float totalPowerConsumed = 0.0; // Variable to store total power consumed

BlynkTimer timer;

bool switchState = false; // Variable to store switch state
unsigned long lastMeasurementTime = 0; // Variable to store the last measurement time
float power = 0.0;

void setup() {
  Serial.begin(9600);
  Serial.println("Connecting to WiFi...");
  
  // Initialize WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);
  Serial.println("Connected to Blynk");

  // Assign a function to be called when the virtual pin state changes
  timer.setInterval(100L, checkVirtualPin);
  pinMode(RELAY_PIN, OUTPUT); // Set relay pin as output
  digitalWrite(RELAY_PIN, HIGH); // Set relay pin high by default
  emon1.current(CT_PIN, 0.5);
  
}

void loop() {
  Blynk.run();
  timer.run();
  updateTotalPowerConsumed();
  
  // Check if it's time to measure power consumption
  if (switchState && (millis() - lastMeasurementTime >= MEASUREMENT_INTERVAL)) {
    measurePowerConsumption();
    lastMeasurementTime = millis();
  }
}

// Function to control the relay based on the value received from the Blynk app
void relayControl(int value) {
  digitalWrite(RELAY_PIN, value == HIGH ? LOW : HIGH); // Toggle the relay based on the value
  switchState = (value == HIGH); // Update switch state
  Serial.print("Relay state: ");
  Serial.println(switchState ? "ON" : "OFF");

  // Reset total power consumed to 0 when the switch is turned off
  if (!switchState) {
    totalPowerConsumed = 0.0;
    // Send the reset total power consumed value to Blynk
    Blynk.virtualWrite(V2, totalPowerConsumed);
  }
}

void checkVirtualPin() {
  int value = digitalRead(RELAY_PIN); // Read the current state of the relay pin
  Blynk.virtualWrite(V2, value); // Update the Blynk app with the current state
}

// Function to measure power consumption
void measurePowerConsumption() {
  // Read the analog value from the CT sensor
  double Irms = emon1.calcIrms(1480);

  // Calculate power consumption
  power = 230.0 * Irms;
  if (switchState) {
    // Add current power consumption to total power consumed
    totalPowerConsumed += power;

    // Send power consumption data to Blynk only when the switch is on
    Blynk.virtualWrite(V4, power);
  }
  // Print power consumption for debugging
  Serial.print("Power consumed: ");
  Serial.print(power);
  Serial.println(" watts");
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V5); // Request the current value of the slider when connected to Blynk
}

// Define the function to update total power consumed and control LED
void updateTotalPowerConsumed() {
  // Send total power consumed over interval to Blynk
  Blynk.virtualWrite(V2, totalPowerConsumed);

  // Check if power consumption exceeds the maximum value
  if (totalPowerConsumed > maxPower) {
    // Turn on the LED if power consumption exceeds the maximum value
    Blynk.virtualWrite(V7, HIGH);
  } else {
    // Turn off the LED if power consumption is within the maximum value
    Blynk.virtualWrite(V7, LOW);
  }

}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  relayControl(value);
  if (value == LOW) { // If the Blynk relay switch is turned off
    // Turn off the actual socket switch
    // Code to turn off the actual socket switch goes here
  } else {
    // Turn on the actual socket switch
    // Code to turn on the actual socket switch goes here
  }
}

BLYNK_WRITE(V5) {
  maxPower = param.asInt(); // Update the maxPower variable with the slider value
  Serial.print("Maximum power set: ");
  Serial.print(maxPower);
  Serial.println(" watts");
}
