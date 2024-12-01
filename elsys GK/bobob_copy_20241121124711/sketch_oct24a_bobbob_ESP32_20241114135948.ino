#include <Wire.h> 

#include "AS726X.h" 

AS726X sensor; 

 

void setup() { 
  Serial.begin(115200); 
  Wire.begin(); // Initialiserer I2C 
  if (sensor.begin() == false) { 
    Serial.println("Kunne ikke finne AS726X-sensoren. Sjekk tilkoblingene."); 
    while (1); 

} 

Serial.println("AS726X-sensor oppdaget!"); 

} 

 

void loop() { 
  sensor.takeMeasurements(); // Ber sensoren om å ta målinger 

  Serial.print("Violet: "); Serial.println(sensor.getViolet()); 

  Serial.print("Blue: "); Serial.println(sensor.getBlue()); 

  Serial.print("Green: "); Serial.println(sensor.getGreen()); 

  Serial.print("Yellow: "); Serial.println(sensor.getYellow()); 

  Serial.print("Orange: "); Serial.println(sensor.getOrange()); 

  Serial.print("Red: "); Serial.println(sensor.getRed()); 
  
  delay(2000); // Vent før neste måling 

} 