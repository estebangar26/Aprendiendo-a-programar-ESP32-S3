#include <Wire.h> 
#include "MAX30100.h" 
 
#define I2C_SDA 8 
#define I2C_SCL 9 
 
MAX30100 sensor; 
 
uint16_t irValue; 
uint16_t redValue; 
 
void setup() { 
  Serial.begin(115200); 
  delay(1000); 
 
  Wire.begin(I2C_SDA, I2C_SCL); 
 
  Serial.println("Inicializando MAX30100..."); 
 
  if (!sensor.begin()) { 
  Serial.println("ERROR: No se detecta el MAX30100"); 
  while (1); 
  } 
 
  sensor.setMode(MAX30100_MODE_SPO2_HR); 
  sensor.setLedsCurrent(MAX30100_LED_CURR_7_6MA, 
                        MAX30100_LED_CURR_7_6MA); 
  sensor.setLedsPulseWidth(MAX30100_SPC_PW_1600US_16BITS); 
  sensor.setSamplingRate(MAX30100_SAMPRATE_100HZ); 
 
  // Etiquetas para el Serial Plotter 
  Serial.println("Red\tIR"); 
} 
 
void loop() { 
  sensor.update(); 
 
  sensor.getRawValues(&irValue, &redValue); 
 
  // Formato compatible con el Serial Plotter del Arduino IDE 
  Serial.print("Red:"); 
  Serial.print(redValue); 
  Serial.print("\t"); 
 
  Serial.print("IR:"); 
  Serial.println(irValue); 
 
  // 10 ms corresponde aproximadamente a 100 muestras por segundo 
  delay(10); 
}