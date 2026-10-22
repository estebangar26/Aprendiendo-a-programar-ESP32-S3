#include <Wire.h>                   // Biblioteca para I²C
#include <Adafruit_GFX.h>           // Biblioteca gráfica genérica
#include <Adafruit_SSD1306.h>       // Controlador para OLED SSD1306

// --- Configuración de la pantalla OLED ---
#define ANCHO_PANTALLA    128       // Pixeles de ancho
#define ALTO_PANTALLA     32        // Pixeles de alto
#define PIN_RESET_OLED    -1        // −1 = sin pin de reset
#define PIN_I2C_SDA       8         // Línea de datos I²C
#define PIN_I2C_SCL       9         // Línea de reloj I²C

Adafruit_SSD1306 pantalla(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, PIN_RESET_OLED);

// --- Pines y constantes del sistema de ventilación ---
const int PIN_ADC_TEMP         = 5;   // Potenciómetro simula sensor (50 mV/°C)
const uint8_t PIN_BTN_INC      = 6;   // Pulsador para +0.5 °C
const uint8_t PIN_BTN_DEC      = 7;   // Pulsador para −0.5 °C
const uint8_t PIN_PWM_VENT     = 4;   // Salida PWM para LED (“ventilador”)

volatile int contadorMediosGrados = 50; // 50 → 25.0 °C inicial

// Antirrebote (milisegundos)
const unsigned long DEBOUNCE_MS = 200;
volatile unsigned long ultimoInc = 0, ultimoDec = 0;

// Intervalo de refresco (ms)
const unsigned long TIEMPO_REFRESH = 500;
unsigned long ultimoRefresh = 0;

// Prototipo de función para calcular PWM
uint8_t calcularPWM(float tempMedida, float tempDeseada);

// --- Rutinas de interrupción con antirrebote ---
void IRAM_ATTR manejarIncremento() {
  unsigned long ahora = millis();
  if (ahora - ultimoInc > DEBOUNCE_MS) {
    contadorMediosGrados++;
    ultimoInc = ahora;
  }
}

void IRAM_ATTR manejarDecremento() {
  unsigned long ahora = millis();
  if (ahora - ultimoDec > DEBOUNCE_MS) {
    contadorMediosGrados--;
    ultimoDec = ahora;
  }
}

void setup() {
  Serial.begin(115200);

  // Inicia bus I²C en pines SDA/SCL
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  // Inicia la pantalla OLED
  if (!pantalla.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) {}  // Se detiene si falla
  }
  pantalla.clearDisplay();
  pantalla.setTextColor(SSD1306_WHITE);

  // Configura ADC a 12 bits y rango ~0–3.3 V
  analogReadResolution(12);


  // Configura pulsadores con pull-up e interrupciones
  pinMode(PIN_BTN_INC, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN_INC),
                  manejarIncremento, FALLING);
  pinMode(PIN_BTN_DEC, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN_DEC),
                  manejarDecremento, FALLING);

  // Configura pin PWM para el “ventilador” (LED)
  pinMode(PIN_PWM_VENT, OUTPUT);
  analogWrite(PIN_PWM_VENT, 0);  // Arranca con PWM = 0%
}

void loop() {
  unsigned long ahora = millis();
  if (ahora - ultimoRefresh < TIEMPO_REFRESH) return;
  ultimoRefresh = ahora;

  // 1) Leer potenciómetro y calcular temperatura (50 mV/°C)
  int raw = analogRead(PIN_ADC_TEMP);
  float voltaje    = raw * (3.3f / 4095.0f);
  float tempMedida = voltaje / 0.05f;

  // 2) Calcular temperatura deseada
  float tempDeseada = contadorMediosGrados * 0.5f;

  // 3) Calcular y aplicar PWM
  uint8_t ciclo = calcularPWM(tempMedida, tempDeseada);
  analogWrite(PIN_PWM_VENT, ciclo);

  // 4) Mostrar lecturas en OLED (texto tamaño 2)
  pantalla.clearDisplay();
  pantalla.setTextSize(2);
  pantalla.setCursor(0, 0);
  pantalla.printf("Med: %.1fC", tempMedida);
  pantalla.setCursor(0, 16);
  pantalla.printf("Des: %.1fC", tempDeseada);
  pantalla.display();
}

// --------------------------------------------------
// calcularPWM(): establece duty según diferencia
// --------------------------------------------------
uint8_t calcularPWM(float medida, float deseada) {
  float dif = medida - deseada;

  if (dif > 7.0f)       return 255;  // >7 °C → 100%
  else if (dif > 4.0f)  return 128;  // >4 °C →  50%
  else if (dif > 1.0f)  return  26;  // >1 °C →  10%
  else                  return   0;  // ≤1 °C →   0%
}
