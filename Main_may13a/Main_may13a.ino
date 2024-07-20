#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3TUQcNJid"
#define BLYNK_TEMPLATE_NAME "Smart Plug"
#define BLYNK_AUTH_TOKEN "-i7gpy-gz8w3U8zkZe4H59-n3Nyc4Omk"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "-i7gpy-gz8w3U8zkZe4H59-n3Nyc4Omk"; // Blynk Auth Token
char ssid[] = "wolfce"; // Your WiFi Name
char pass[] = "abcde123"; // Your WiFi Password

#define RELAY_PIN 2          // Define the pin connected to the relay
#define CT_PIN A0            // Define the pin connected to the CT sensor
#define MEASUREMENT_INTERVAL 1000  // Interval in milliseconds to measure power consumption

float calibrationFactor = 1.0; // Calibration factor for CT sensor readings
float maxPower = 1000.0; // Default maximum power consumption

BlynkTimer timer;

bool switchState = false; // Variable to store switch state
unsigned long lastMeasurementTime = 0; // Variable to store the last measurement time
float powerConsumption = 0.0; // Variable to store power consumption

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi...");
  
  // Initialize WiFi and Blynk
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  
  Blynk.begin(auth, ssid, pass);
  Serial.println("Connected to Blynk");
  
  // Setup pin modes
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Initially turn off the relay
  pinMode(CT_PIN, INPUT);
  
  // Assign a function to be called when the virtual pin state changes
  timer.setInterval(100L, checkVirtualPin);
}

void loop() {
  Blynk.run();
  timer.run();
  
  // Check if the switch is on and it's time to measure power consumption
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
}

void checkVirtualPin() {
  int value = digitalRead(RELAY_PIN); // Read the current state of the relay pin
  Blynk.virtualWrite(V2, value); // Update the Blynk app with the current state
}

// Function to measure power consumption
void measurePowerConsumption() {
  // Read the analog value from the CT sensor
  int sensorValue = analogRead(CT_PIN);
  // Apply calibration factor to compensate for the stepped-down voltage
  float current = map(sensorValue, 0, 1023, 0, 3.3) * calibrationFactor; // Assuming 5V ADC reference voltage
  // Calculate power consumption using a formula specific to your CT sensor and circuit
  // For example, if your CT sensor provides current in Amperes, and you have the voltage, you can use P = V * I
  float voltage = 220.0; // Assuming line voltage of 220V
  powerConsumption = voltage * current;
  // Send power consumption data to Blynk
  Blynk.virtualWrite(V4, powerConsumption); // Update the Blynk app with power consumption
  
  // Check if power consumption exceeds the maximum value
  if (powerConsumption > maxPower) {
    // Turn off the relay if power consumption exceeds the maximum value
    relayControl(HIGH);
  }
  Serial.print("Power consumed so far: ");
  Serial.print(powerConsumption);
  Serial.println(" watts");
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V5);  // Request the current value of the slider when connected to Blynk
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  relayControl(value);
}

BLYNK_WRITE(V5) {
  maxPower = param.asInt(); // Update the maxPower variable with the slider value
  Blynk.virtualWrite(V6, maxPower); // Update the gauge widget with the new max value
  Serial.print("Maximum power set: ");
  Serial.print(maxPower);
  Serial.println(" watts");
}
