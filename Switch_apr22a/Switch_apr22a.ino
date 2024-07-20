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

#define RELAY_PIN 2 // Define the pin connected to the relay

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Initially turn off the relay

  // Setup a virtual pin to control the relay
  Blynk.virtualWrite(V2, LOW); // Set the initial state of the widget in the Blynk app

  // Setup a virtual pin to receive commands from the Blynk app
  Blynk.virtualWrite(V3, 0); // Set the initial state of the widget in the Blynk app

  // Assign a function to be called when the virtual pin state changes
  timer.setInterval(100L, checkVirtualPin);
}

void loop() {
  Blynk.run();
  timer.run();
}

// Function to control the relay based on the value received from the Blynk app
void relayControl(int value) {
  if (value == HIGH) {
    digitalWrite(RELAY_PIN, LOW); // Turn off the relay
    Blynk.virtualWrite(V2, LOW); // Update V2 to show relay is OFF
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Turn on the relay
    Blynk.virtualWrite(V2, HIGH); // Update V2 to show relay is ON
  }
}

void checkVirtualPin() {
  int value = digitalRead(RELAY_PIN); // Read the current state of the relay pin
  Blynk.virtualWrite(V2, value); // Update the Blynk app with the current state
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  relayControl(value);
}
