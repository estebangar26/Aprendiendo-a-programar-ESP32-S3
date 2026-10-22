
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS 0x3C
#define OLED_SDA        8
#define OLED_SCL        9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PIN_NEOPIXEL 48
#define NUMPIXELS     1

Adafruit_NeoPixel pixel(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

const int pinPot    = 5;
const int pinLedExt = 4;

int   leerADCpromedio();
void  setRGB(int r, int g, int b, const char* nombreColor);
void  updateOLED(const char* header, const char* data);

void setup() {
  Serial.begin(115200);
  pinMode(pinLedExt, OUTPUT);
  digitalWrite(pinLedExt, LOW);
  pinMode(pinPot, INPUT);
  analogReadResolution(12);
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Error: no se detecto la pantalla OLED"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Sistema iniciado...");
  display.display();

  pixel.begin();
  pixel.clear();
  pixel.show();

  Serial.println("=============================");
  Serial.println("  Sistema iniciado - P07");
  Serial.println("=============================");
  Serial.println("Comandos disponibles:");
  Serial.println("  1   -> Encender LED externo");
  Serial.println("  0   -> Apagar LED externo");
  Serial.println("  V   -> Voltaje por Monitor Serie");
  Serial.println("  v   -> Voltaje en pantalla OLED");
  Serial.println("  X   -> ADC hexadecimal por Monitor Serie");
  Serial.println("  x   -> ADC hexadecimal en pantalla OLED");
  Serial.println("  N   -> ADC decimal por Monitor Serie");
  Serial.println("  n   -> ADC decimal en pantalla OLED");
  Serial.println("  R/G/B/M/C/A -> Color NeoPixel");
  Serial.println("  Z   -> Apagar NeoPixel");
  Serial.println("=============================");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == '\n' || cmd == '\r') return;

    int   adcVal  = leerADCpromedio();
    float voltage = adcVal * 3.3f / 4095.0f;

    switch (cmd) {
      case '1':
        digitalWrite(pinLedExt, HIGH);
        Serial.println("LED externo encendido");
        break;

      case '0':
        digitalWrite(pinLedExt, LOW);
        Serial.println("LED externo apagado");
        break;

      case 'V':
        Serial.printf("ADC decimal: %d | ADC hexadecimal: 0x%03X | Voltaje: %.2f V\n", adcVal, adcVal, voltage);
        break;

      case 'v':
        {
          char buf[16];
          snprintf(buf, sizeof(buf), "%.2f V", voltage);
          updateOLED("VOLTAJE:", buf);
          Serial.printf("OLED actualizado -> Voltaje: %.2f V\n", voltage);
        }
        break;

      case 'X':
        Serial.printf("Valor ADC en hexadecimal: 0x%03X\n", adcVal);
        break;

      case 'x':
        {
          char buf[16];
          snprintf(buf, sizeof(buf), "0x%03X", adcVal);
          updateOLED("ADC HEX:", buf);
          Serial.printf("OLED actualizado -> ADC HEX: 0x%03X\n", adcVal);
        }
        break;

      case 'N':
        Serial.printf("Valor ADC en decimal: %d\n", adcVal);
        break;

      case 'n':
        {
          char buf[16];
          snprintf(buf, sizeof(buf), "%d", adcVal);
          updateOLED("ADC DEC:", buf);
          Serial.printf("OLED actualizado -> ADC DEC: %d\n", adcVal);
        }
        break;

      case 'R': setRGB(255,   0,   0, "ROJO");     break;
      case 'G': setRGB(  0, 255,   0, "VERDE");    break;
      case 'B': setRGB(  0,   0, 255, "AZUL");     break;
      case 'M': setRGB(255,   0, 255, "MAGENTA");  break;
      case 'C': setRGB(  0, 255, 255, "CIAN");     break;
      case 'A': setRGB(255, 255,   0, "AMARILLO"); break;
      case 'Z': setRGB(  0,   0,   0, "APAGADO");  break;

      default:
        Serial.printf("Comando no valido: %c\n", cmd);
        break;
    }
  }
}

int leerADCpromedio() {
  long suma = 0;
  for (int i = 0; i < 10; i++) {
    suma += analogRead(pinPot);
    delayMicroseconds(500);
  }
  return (int)(suma / 10);
}

void setRGB(int r, int g, int b, const char* nombreColor) {
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
  Serial.printf("NeoPixel configurado en color %s\n", nombreColor);
  updateOLED("RGB:", nombreColor);
}

void updateOLED(const char* header, const char* data) {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(2);
  display.println(header);
  display.setCursor(0, 35);
  display.setTextSize(2);
  display.println(data);
  display.display();
}
