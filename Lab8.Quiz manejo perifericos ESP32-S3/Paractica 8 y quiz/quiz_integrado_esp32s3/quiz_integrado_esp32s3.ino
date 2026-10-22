// ================================================================
//  Introducción a la Bioinstrumentación · Quiz de Laboratorio
//  ESP32-S3 · Código integrado — todos los periféricos
// ================================================================
//  LED externo     → GPIO4 (resistencia 220Ω → GND)
//  Pulsador        → GPIO5 (INPUT_PULLUP → GND)
//  Potenciómetro 1 → GPIO6
//  Potenciómetro 2 → GPIO7
//  OLED SSD1306    → SDA GPIO8 · SCL GPIO9
//  NeoPixel        → GPIO48
//
//  Comandos por Monitor Serie (115200 baud):
//  r=Rojo  g=Verde  b=Azul  n=Siguiente color
//  l=Toggle LED    v=Leer voltajes
// ================================================================

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define PIN_LED       4
#define PIN_BOTON     5
#define PIN_POT1      6
#define PIN_POT2      7
#define PIN_SDA       8
#define PIN_SCL       9
#define PIN_NEOPIXEL  48
#define NUM_PIXELS    1
#define ADC_MAX       4095.0
#define V_REF         3.3
#define OLED_ADDR     0x3C

Adafruit_SSD1306  oled(128, 32, &Wire, -1);
Adafruit_NeoPixel pixel(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

struct Color { uint8_t r, g, b; const char* nombre; };
const Color COLORES[] = {
  { 255,   0,   0, "Rojo"     },
  {   0, 255,   0, "Verde"    },
  {   0,   0, 255, "Azul"     },
  { 255, 255,   0, "Amarillo" },
  { 128,   0, 128, "Morado"   }
};
const int NUM_COLORES = 5;

int           colorActual   = 0;
bool          estadoLED     = false;
bool          botonAnterior = HIGH;
unsigned long tAnterior     = 0;

// ── Aplica color al NeoPixel ─────────────────────────────────
void aplicarColor(int idx) {
  pixel.setPixelColor(0, pixel.Color(COLORES[idx].r, COLORES[idx].g, COLORES[idx].b));
  pixel.show();
}

// ── Refresca la OLED ─────────────────────────────────────────
void actualizarOLED(float v1, float v2, bool boton) {
  oled.clearDisplay();
  oled.setTextSize(0.5);
  oled.setCursor(0, 0);  oled.println("--- Quiz ESP32-S3 ---");
  oled.setCursor(0, 12); oled.printf("Pot1 (G6): %.2f V", v1);
  oled.setCursor(0, 22); oled.printf("Pot2 (G7): %.2f V", v2);
  oled.setCursor(0, 34); oled.print("Boton: ");
  oled.print(boton == LOW ? "PRESIONADO" : "Libre");
  oled.setCursor(0, 44); oled.print("LED RGB: ");
  oled.print(COLORES[colorActual].nombre);
  oled.setCursor(0, 54); oled.print("LED ext: ");
  oled.print(digitalRead(PIN_LED) ? "ON" : "OFF");
  oled.display();
}

// ── Procesa comandos Serial ───────────────────────────────────
void procesarComandoSerial() {
  if (!Serial.available()) return;
  char cmd = Serial.read();
  switch (cmd) {
    case 'r': colorActual = 0; aplicarColor(colorActual); Serial.println("NeoPixel → Rojo");    break;
    case 'g': colorActual = 1; aplicarColor(colorActual); Serial.println("NeoPixel → Verde");   break;
    case 'b': colorActual = 2; aplicarColor(colorActual); Serial.println("NeoPixel → Azul");    break;
    case 'n':
      colorActual = (colorActual + 1) % NUM_COLORES;
      aplicarColor(colorActual);
      Serial.print("NeoPixel → "); Serial.println(COLORES[colorActual].nombre);
      break;
    case 'l':
      estadoLED = !estadoLED;
      digitalWrite(PIN_LED, estadoLED);
      Serial.print("LED externo → "); Serial.println(estadoLED ? "ON" : "OFF");
      break;
    case 'v': {
      float va = (analogRead(PIN_POT1) / ADC_MAX) * V_REF;
      float vb = (analogRead(PIN_POT2) / ADC_MAX) * V_REF;
      Serial.printf("GPIO6: %.2f V | GPIO7: %.2f V\n", va, vb);
      break;
    }
    default: break;
  }
}

// ================================================================
void setup() {
  Serial.begin(115200);
  Serial.println("=== Quiz ESP32-S3 · Bioinstrumentacion ===");
  Serial.println("Comandos: r g b n l v");

  pinMode(PIN_LED,   OUTPUT);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  analogReadResolution(12);

  Wire.begin(PIN_SDA, PIN_SCL);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("[ERROR] OLED no encontrada. Revisa SDA/SCL.");
    while (true);
  }
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println("  Quiz ESP32-S3");
  oled.println("Bioinstrumentacion");
  oled.println("\n  Iniciando...");
  oled.display();
  delay(1500);

  pixel.begin();
  pixel.setBrightness(80);
  aplicarColor(colorActual);
}

// ================================================================
void loop() {
  unsigned long ahora = millis();

  // LED parpadea cada 500 ms
  if (ahora - tAnterior >= 500) {
    tAnterior = ahora;
    estadoLED = !estadoLED;
    digitalWrite(PIN_LED, estadoLED);
  }

  // Pulsador: detecta flanco de bajada
  bool botonActual = digitalRead(PIN_BOTON);
  if (botonActual == LOW && botonAnterior == HIGH) {
    colorActual = (colorActual + 1) % NUM_COLORES;
    aplicarColor(colorActual);
    Serial.print("Boton pulsado. NeoPixel: ");
    Serial.println(COLORES[colorActual].nombre);
  }
  botonAnterior = botonActual;

  // Potenciómetros → voltaje
  float v1 = (analogRead(PIN_POT1) / ADC_MAX) * V_REF;
  float v2 = (analogRead(PIN_POT2) / ADC_MAX) * V_REF;

  // Monitor Serie
  Serial.printf("Pot GPIO6: %.2f V  |  Pot GPIO7: %.2f V\n", v1, v2);

  // OLED
  actualizarOLED(v1, v2, botonActual);

  // Comandos Serial
  procesarComandoSerial();

  delay(200);
}