#include "EmonLib.h"
#include  <Wire.h>                 // Include Emon Library
EnergyMonitor emon1; 
#define RELAY_PIN 18                     // Create an instance
int CT_pin = 33; //  CT sensor pin connected to A5 pin of Arduino
void setup()
{  
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  emon1.current(CT_pin, 0.5);             // Current: input pin,  change here for calibration.
}
void loop()
{
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  
  Serial.print(Irms*230.0);           // Apparent power
  Serial.print(" ");
  Serial.println(Irms);               // Irms
  //delay(50);
}