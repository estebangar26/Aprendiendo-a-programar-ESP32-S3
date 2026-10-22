#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
#include "MAX30100_PulseOximeter.h" 
// Pines I2C usados en el ESP32-S3 
#define I2C_SDA 8 
#define I2C_SCL 9 
// Configuracion de la pantalla OLED 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1 
#define OLED_ADDR 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Objeto de alto nivel para el MAX30100 
PulseOximeter pox; 
 
uint32_t lastUpdate = 0; 
 
void onBeatDetected() { 
  Serial.println("Latido detectado"); 
} 
 
void setup() { 
  Serial.begin(115200); 
  delay(1000); 
 
  // En ESP32-S3 es importante declarar explicitamente SDA y SCL 
  Wire.begin(I2C_SDA, I2C_SCL); 
 
  // Inicializacion de la pantalla OLED 
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) { 
    Serial.println("ERROR: No se detecto la pantalla OLED"); 
    while (true); 
  } 
 
  display.clearDisplay(); 
  display.setTextColor(SSD1306_WHITE); 
  display.setTextSize(1); 
  display.setCursor(0, 0); 
  display.println("Monitor signos vitales"); 
  display.println("Inicializando..."); 
  display.display(); 
 
  // Inicializacion del MAX30100 
  if (!pox.begin()) { 
    Serial.println("ERROR: No se detecto el MAX30100"); 
    display.clearDisplay(); 
    display.setCursor(0, 0); 
    display.println("ERROR:"); 
    display.println("MAX30100 no detectado"); 
    display.display(); 
    while (true); 
  } 
 
  // Corriente del LED infrarrojo. Puede ajustarse si la senal es muy debil. 
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA); 
  pox.setOnBeatDetectedCallback(onBeatDetected); 
 
  display.clearDisplay(); 
  display.setCursor(0, 0); 
  display.println("Sistema listo"); 
  display.println("Coloque el dedo"); 
  display.display(); 
} 
 
void loop() { 
  // Esta funcion debe llamarse continuamente para actualizar el algoritmo interno 
  pox.update(); 
 
  // Actualizar la pantalla y el puerto serial cada segundo 
  if (millis() - lastUpdate > 1000) { 
    lastUpdate = millis(); 
 
    float bpm = pox.getHeartRate(); 
    float spo2 = pox.getSpO2(); 
 
    Serial.print("BPM: "); 
    Serial.print(bpm); 
    Serial.print("\tSpO2: "); 
    Serial.println(spo2); 
 
    display.clearDisplay(); 
    display.setTextSize(1); 
    display.setCursor(0, 0); 
    display.println("Monitor de signos"); 
 
    display.setTextSize(2); 
    display.setCursor(0, 20); 
    display.print("BPM:"); 
    display.print(bpm, 0); 
 
    display.setCursor(0, 45); 
    display.print("SpO2:"); 
    display.print(spo2, 0); 
    display.print("%"); 
 
    display.display(); 
  } 
} 
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