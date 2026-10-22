// ================================================================
//  Introducción a la Bioinstrumentación · Quiz de Laboratorio
//  ESP32-S3 · Código integrado — todos los periféricos
// ================================================================
//
//  CONEXIONES:
//  ┌─────────────────┬──────────────────────────────────────────┐
//  │ Periférico      │ Conexión                                 │
//  ├─────────────────┼──────────────────────────────────────────┤
//  │ LED externo     │ GPIO4 → resistencia 220Ω → GND           │
//  │ Pulsador        │ GPIO5 (INPUT_PULLUP) → GND               │
//  │ Potenciómetro 1 │ señal → GPIO6 · extremos a 3.3V y GND   │
//  │ Potenciómetro 2 │ señal → GPIO7 · extremos a 3.3V y GND   │
//  │ OLED SSD1306    │ SDA → GPIO8 · SCL → GPIO9               │
//  │ NeoPixel        │ GPIO48 (LED integrado ESP32-S3)          │
//  └─────────────────┴──────────────────────────────────────────┘
//
//  BIBLIOTECAS REQUERIDAS (instalar desde el gestor de Arduino):
//    · Adafruit GFX Library
//    · Adafruit SSD1306
//    · Adafruit NeoPixel
//    · Wire (incluida con Arduino)
//
//  COMANDOS POR MONITOR SERIE (115200 baud):
//    r → NeoPixel rojo      g → NeoPixel verde    b → NeoPixel azul
//    n → Siguiente color    l → Toggle LED externo
//    v → Leer voltajes ahora
// ================================================================

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

// ── Definición de pines ──────────────────────────────────────
#define PIN_LED       4     // LED externo con resistencia 220Ω
#define PIN_BOTON     5     // Pulsador con INPUT_PULLUP
#define PIN_POT1      6     // Potenciómetro 1
#define PIN_POT2      7     // Potenciómetro 2
#define PIN_SDA       8     // OLED SDA
#define PIN_SCL       9     // OLED SCL
#define PIN_NEOPIXEL  48    // LED RGB integrado
#define NUM_PIXELS    1

// ── Parámetros ADC ───────────────────────────────────────────
#define ADC_MAX   4095.0    // Resolución 12 bits
#define V_REF     3.3       // Voltaje de referencia ESP32-S3

// ── Parámetros OLED ──────────────────────────────────────────
#define OLED_ANCHO  128
#define OLED_ALTO   64
#define OLED_ADDR   0x3C

// ── Objetos ──────────────────────────────────────────────────
Adafruit_SSD1306  oled(OLED_ANCHO, OLED_ALTO, &Wire, -1);
Adafruit_NeoPixel pixel(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// ── Tabla de colores NeoPixel ────────────────────────────────
struct Color { uint8_t r, g, b; const char* nombre; };
const Color COLORES[] = {
  { 255,   0,   0, "Rojo"     },
  {   0, 255,   0, "Verde"    },
  {   0,   0, 255, "Azul"     },
  { 255, 255,   0, "Amarillo" },
  { 128,   0, 128, "Morado"   }
};
const int NUM_COLORES = 5;

// ── Variables de estado ──────────────────────────────────────
int           colorActual    = 0;     // Índice del color NeoPixel actual
bool          estadoLED      = false; // Estado del LED externo
bool          botonAnterior  = HIGH;  // Para detectar flanco del pulsador
unsigned long tAnterior      = 0;     // Control de parpadeo
const long    INTERVALO_MS   = 500;   // Intervalo de parpadeo del LED

// ================================================================
void setup() {

  // ── 1. Comunicación serial ───────────────────────────────────
  Serial.begin(115200);
  Serial.println("================================================");
  Serial.println("  Quiz ESP32-S3 · Bioinstrumentación");
  Serial.println("================================================");
  Serial.println("Comandos disponibles en el Monitor Serie:");
  Serial.println("  r → NeoPixel Rojo     g → NeoPixel Verde");
  Serial.println("  b → NeoPixel Azul     n → Siguiente color");
  Serial.println("  l → Toggle LED        v → Leer voltajes");
  Serial.println("================================================\n");

  // ── 2. Configuración de pines ────────────────────────────────
  pinMode(PIN_LED,   OUTPUT);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  analogReadResolution(12);           // ADC de 12 bits (0–4095)

  // ── 3. OLED por I2C ──────────────────────────────────────────
  Wire.begin(PIN_SDA, PIN_SCL);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("[ERROR] OLED no encontrada.");
    Serial.println("        Verifica SDA (GPIO8) y SCL (GPIO9).");
    while (true);
  }
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println("  Quiz ESP32-S3");
  oled.println("Bioinstrumentacion");
  oled.println();
  oled.println("  Iniciando...");
  oled.display();
  delay(1500);

  // ── 4. NeoPixel ──────────────────────────────────────────────
  pixel.begin();
  pixel.setBrightness(80);
  aplicarColor(colorActual);

  Serial.println("Sistema listo.\n");
}

// ================================================================
void loop() {
  unsigned long ahora = millis();

  // ============================================================
  // TAREA 1 · LED EXTERNO
  // El LED parpadea automáticamente cada INTERVALO_MS milisegundos.
  // También puede togglearse con el comando 'l' por Monitor Serie.
  // ============================================================
  if (ahora - tAnterior >= INTERVALO_MS) {
    tAnterior  = ahora;
    estadoLED  = !estadoLED;
    digitalWrite(PIN_LED, estadoLED);
  }

  // ============================================================
  // TAREA 2 · PULSADOR
  // Detecta cuando el botón se presiona (flanco de bajada).
  // Cambia el color del NeoPixel y muestra mensaje por Serial.
  // ============================================================
  bool botonActual = digitalRead(PIN_BOTON);
  if (botonActual == LOW && botonAnterior == HIGH) {
    // Botón recién presionado → avanzar color
    colorActual = (colorActual + 1) % NUM_COLORES;
    aplicarColor(colorActual);
    Serial.print("Boton pulsado. NeoPixel: ");
    Serial.println(COLORES[colorActual].nombre);
  }
  botonAnterior = botonActual;

  // ============================================================
  // TAREA 3 · POTENCIÓMETROS
  // Lee los valores analógicos y los convierte a voltaje (0–3.3 V).
  // Muestra los valores con dos cifras decimales en el Monitor Serie.
  // ============================================================
  int   raw1 = analogRead(PIN_POT1);
  int   raw2 = analogRead(PIN_POT2);
  float v1   = (raw1 / ADC_MAX) * V_REF;
  float v2   = (raw2 / ADC_MAX) * V_REF;

  // ============================================================
  // TAREA 4+5 · UART / MONITOR SERIE
  // Envía todos los valores formateados al computador cada ciclo.
  // ============================================================
  Serial.printf("Pot GPIO6: %.2f V  |  Pot GPIO7: %.2f V\n", v1, v2);

  // ============================================================
  // TAREA 4 · OLED SSD1306
  // Muestra en pantalla: voltajes, estado del botón y color actual.
  // ============================================================
  actualizarOLED(v1, v2, botonActual);

  // ============================================================
  // TAREA 5 · UART — RECIBIR COMANDOS
  // Permite controlar el sistema enviando un carácter desde el PC.
  // ============================================================
  procesarComandoSerial();

  delay(200);   // Pausa breve para no saturar la comunicación serial
}

// ================================================================
//  FUNCIONES AUXILIARES
// ================================================================

// Aplica el color del arreglo COLORES al NeoPixel
void aplicarColor(int indice) {
  pixel.setPixelColor(0, pixel.Color(
    COLORES[indice].r,
    COLORES[indice].g,
    COLORES[indice].b
  ));
  pixel.show();
}

// Refresca la pantalla OLED con todos los valores actuales
void actualizarOLED(float v1, float v2, bool boton) {
  oled.clearDisplay();
  oled.setTextSize(1);

  // Fila 0 — Título
  oled.setCursor(0, 0);
  oled.println("--- Quiz ESP32-S3 ---");

  // Fila 1 — Potenciómetro 1
  oled.setCursor(0, 12);
  oled.printf("Pot1 (G6): %.2f V", v1);

  // Fila 2 — Potenciómetro 2
  oled.setCursor(0, 22);
  oled.printf("Pot2 (G7): %.2f V", v2);

  // Fila 3 — Estado del botón
  oled.setCursor(0, 34);
  oled.print("Boton: ");
  oled.print(boton == LOW ? "PRESIONADO" : "Libre     ");

  // Fila 4 — Color NeoPixel actual
  oled.setCursor(0, 44);
  oled.print("LED RGB: ");
  oled.print(COLORES[colorActual].nombre);

  // Fila 5 — Estado LED externo
  oled.setCursor(0, 54);
  oled.print("LED ext: ");
  oled.print(digitalRead(PIN_LED) ? "ON " : "OFF");

  oled.display();
}

// Lee y ejecuta comandos recibidos por el Monitor Serie
void procesarComandoSerial() {
  if (!Serial.available()) return;

  char cmd = Serial.read();

  switch (cmd) {
    case 'r':                             // → Rojo
      colorActual = 0;
      aplicarColor(colorActual);
      Serial.println("Comando 'r' → NeoPixel Rojo");
      break;

    case 'g':                             // → Verde
      colorActual = 1;
      aplicarColor(colorActual);
      Serial.println("Comando 'g' → NeoPixel Verde");
      break;

    case 'b':                             // → Azul
      colorActual = 2;
      aplicarColor(colorActual);
      Serial.println("Comando 'b' → NeoPixel Azul");
      break;

    case 'n':                             // → Siguiente color
      colorActual = (colorActual + 1) % NUM_COLORES;
      aplicarColor(colorActual);
      Serial.print("Comando 'n' → NeoPixel ");
      Serial.println(COLORES[colorActual].nombre);
      break;

    case 'l':                             // → Toggle LED externo
      estadoLED = !estadoLED;
      digitalWrite(PIN_LED, estadoLED);
      Serial.print("Comando 'l' → LED externo ");
      Serial.println(estadoLED ? "ON" : "OFF");
      break;

    case 'v': {                           // → Lectura inmediata de voltajes
      float va = (analogRead(PIN_POT1) / ADC_MAX) * V_REF;
      float vb = (analogRead(PIN_POT2) / ADC_MAX) * V_REF;
      Serial.printf("Lectura manual → GPIO6: %.2f V | GPIO7: %.2f V\n", va, vb);
      break;
    }

    default:
      break;   // Ignora caracteres no reconocidos (saltos de línea, etc.)
  }
}
