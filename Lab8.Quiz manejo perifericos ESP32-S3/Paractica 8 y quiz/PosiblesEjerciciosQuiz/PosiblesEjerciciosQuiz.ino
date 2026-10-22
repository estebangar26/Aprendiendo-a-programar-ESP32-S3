////////////////////////////////////////////////////////////
// 1. LED externo en GPIO4: Parpadeo simple
////////////////////////////////////////////////////////////

#define LED_PIN 4
 
void setup() {
    pinMode(LED_PIN, OUTPUT);
}
 
void loop() {
    digitalWrite(LED_PIN, HIGH);   // LED encendido
    delay(500);
    digitalWrite(LED_PIN, LOW);    // LED apagado
    delay(500);
}

////////////////////////////////////////////////////////////
// 2. LED externo controlado por pulsador
////////////////////////////////////////////////////////////

#define LED_PIN  4
#define BTN_PIN  5
 
void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BTN_PIN, INPUT_PULLUP);
}
 
void loop() {
    int boton = digitalRead(BTN_PIN);
 
    if (boton == LOW) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
}

////////////////////////////////////////////////////////////
// 3. Pulsador por Monitor Serie
////////////////////////////////////////////////////////////

#define BTN_PIN 5
 
void setup() {
    Serial.begin(115200);
    pinMode(BTN_PIN, INPUT_PULLUP);
}
 
void loop() {
    if (digitalRead(BTN_PIN) == LOW) {
        Serial.println("Boton presionado");
        delay(100);
    }
}

////////////////////////////////////////////////////////////
// 4. Detectar solo una pulsación del botón
////////////////////////////////////////////////////////////

#define BTN_PIN 5
 
int estadoAnterior = HIGH;
 
void setup() {
    Serial.begin(115200);
    pinMode(BTN_PIN, INPUT_PULLUP);
}
 
void loop() {
    int estadoActual = digitalRead(BTN_PIN);
 
    if (estadoActual == LOW && estadoAnterior == HIGH) {
        Serial.println("Pulsacion detectada!");
    }
 
    estadoAnterior = estadoActual;
    delay(20);
}

////////////////////////////////////////////////////////////
// 5. Potenciómetro en GPIO6: lectura ADC
////////////////////////////////////////////////////////////

#define POT1_PIN 6
 
void setup() {
    Serial.begin(115200);
}
 
void loop() {
    int lectura = analogRead(POT1_PIN);
    Serial.print("ADC GPIO6: ");
    Serial.println(lectura);
    delay(200);
}

////////////////////////////////////////////////////////////
// 6. Potenciómetro en GPIO6 convertido a voltaje
////////////////////////////////////////////////////////////

#define POT1_PIN 6
 
void setup() {
    Serial.begin(115200);
}
 
void loop() {
    int lectura = analogRead(POT1_PIN);
    float voltaje = lectura * 3.3 / 4095.0;
 
    Serial.print("Voltaje GPIO6: ");
    Serial.print(voltaje, 2);
    Serial.println(" V");
    delay(200);
}

////////////////////////////////////////////////////////////
// 7. Leer los dos potenciómetros GPIO6 y GPIO7
////////////////////////////////////////////////////////////

#define POT1_PIN 6
#define POT2_PIN 7
 
void setup() {
    Serial.begin(115200);
}
 
void loop() {
    float v1 = analogRead(POT1_PIN) * 3.3 / 4095.0;
    float v2 = analogRead(POT2_PIN) * 3.3 / 4095.0;
 
    Serial.print("Pot GPIO6: ");
    Serial.print(v1, 2);
    Serial.print(" V  |  Pot GPIO7: ");
    Serial.print(v2, 2);
    Serial.println(" V");
    delay(300);
}

////////////////////////////////////////////////////////////
// 8. LED controlado por potenciómetro
////////////////////////////////////////////////////////////

#define LED_PIN  4
#define POT1_PIN 6
 
void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
}
 
void loop() {
    float voltaje = analogRead(POT1_PIN) * 3.3 / 4095.0;
 
    if (voltaje > 1.65) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
 
    Serial.print("Voltaje: ");
    Serial.print(voltaje, 2);
    Serial.println(" V");
    delay(100);
}

////////////////////////////////////////////////////////////
// 9. OLED SSD1306: mostrar mensaje básico
////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
void setup() {
    Wire.begin(8, 9);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Hola ESP32!");
    display.display();
}
 
void loop() {
}

////////////////////////////////////////////////////////////
// 10. OLED mostrando un potenciómetro
////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define POT1_PIN     6
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
void setup() {
    Wire.begin(8, 9);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}
 
void loop() {
    float voltaje = analogRead(POT1_PIN) * 3.3 / 4095.0;
 
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Pot GPIO6:");
    display.print(voltaje, 2);
    display.println(" V");
    display.display();
    delay(200);
}

////////////////////////////////////////////////////////////
// 11. OLED mostrando dos potenciómetros
////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define POT1_PIN     6
#define POT2_PIN     7
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
void setup() {
    Wire.begin(8, 9);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}
 
void loop() {
    float v1 = analogRead(POT1_PIN) * 3.3 / 4095.0;
    float v2 = analogRead(POT2_PIN) * 3.3 / 4095.0;
 
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
 
    display.setCursor(0, 0);
    display.print("Pot6: ");
    display.print(v1, 2);
    display.println(" V");
 
    display.setCursor(0, 20);
    display.print("Pot7: ");
    display.print(v2, 2);
    display.println(" V");
 
    display.display();
    delay(200);
}

////////////////////////////////////////////////////////////
// 12. OLED mostrando estado del botón
////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define BTN_PIN      5
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
void setup() {
    Wire.begin(8, 9);
    pinMode(BTN_PIN, INPUT_PULLUP);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}
 
void loop() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
 
    if (digitalRead(BTN_PIN) == LOW) {
        display.println("PRESIONADO");
    } else {
        display.println("SUELTO");
    }
 
    display.display();
    delay(100);
}

////////////////////////////////////////////////////////////
// 13. NeoPixel: encender un color fijo
////////////////////////////////////////////////////////////

#include <Adafruit_NeoPixel.h>
 
#define NEOPIXEL_PIN 48
#define NUM_PIXELS   1
 
Adafruit_NeoPixel pixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
    pixel.begin();
    pixel.setBrightness(50);
    pixel.setPixelColor(0, pixel.Color(255, 0, 0));
    pixel.show();
}
 
void loop() {
}

////////////////////////////////////////////////////////////
// 14. NeoPixel cambia con el botón
////////////////////////////////////////////////////////////

#include <Adafruit_NeoPixel.h>
 
#define BTN_PIN      5
#define NEOPIXEL_PIN 48
#define NUM_PIXELS   1
 
Adafruit_NeoPixel pixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
    pinMode(BTN_PIN, INPUT_PULLUP);
    pixel.begin();
    pixel.setBrightness(50);
}
 
void loop() {
    if (digitalRead(BTN_PIN) == LOW) {
        pixel.setPixelColor(0, pixel.Color(0, 255, 0));
    } else {
        pixel.setPixelColor(0, pixel.Color(255, 0, 0));
    }
    pixel.show();
    delay(50);
}

////////////////////////////////////////////////////////////
// 15. NeoPixel cambia según potenciómetro
////////////////////////////////////////////////////////////

#include <Adafruit_NeoPixel.h>
 
#define POT1_PIN     6
#define NEOPIXEL_PIN 48
#define NUM_PIXELS   1
 
Adafruit_NeoPixel pixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
    pixel.begin();
    pixel.setBrightness(50);
}
 
void loop() {
    float voltaje = analogRead(POT1_PIN) * 3.3 / 4095.0;
 
    if (voltaje < 1.1) {
        pixel.setPixelColor(0, pixel.Color(0, 0, 255));
    } else if (voltaje < 2.2) {
        pixel.setPixelColor(0, pixel.Color(0, 255, 0));
    } else {
        pixel.setPixelColor(0, pixel.Color(255, 0, 0));
    }
 
    pixel.show();
    delay(100);
}

////////////////////////////////////////////////////////////
// 16. UART para LED externo
////////////////////////////////////////////////////////////

#define LED_PIN 4
 
void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("Listo. Comandos: e=encender, a=apagar, p=parpadear");
}
 
void loop() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();
 
        if (cmd == 'e') {
            digitalWrite(LED_PIN, HIGH);
            Serial.println("LED encendido");
        } else if (cmd == 'a') {
            digitalWrite(LED_PIN, LOW);
            Serial.println("LED apagado");
        } else if (cmd == 'p') {
            digitalWrite(LED_PIN, HIGH);
            delay(300);
            digitalWrite(LED_PIN, LOW);
            Serial.println("LED parpadeado");
        }
    }
}

////////////////////////////////////////////////////////////
// 17. UART para NeoPixel
////////////////////////////////////////////////////////////

#include <Adafruit_NeoPixel.h>
 
#define NEOPIXEL_PIN 48
#define NUM_PIXELS   1
 
Adafruit_NeoPixel pixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
    Serial.begin(115200);
    pixel.begin();
    pixel.setBrightness(50);
    pixel.show();
    Serial.println("Comandos: r=rojo, g=verde, b=azul, o=apagar");
}
 
void loop() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();
 
        if (cmd == 'r') {
            pixel.setPixelColor(0, pixel.Color(255, 0, 0));
        } else if (cmd == 'g') {
            pixel.setPixelColor(0, pixel.Color(0, 255, 0));
        } else if (cmd == 'b') {
            pixel.setPixelColor(0, pixel.Color(0, 0, 255));
        } else if (cmd == 'o') {
            pixel.setPixelColor(0, pixel.Color(0, 0, 0));
        }
 
        pixel.show();
    }
}

////////////////////////////////////////////////////////////
// 18. Integración: potenciómetros + OLED + LED
////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define LED_PIN   4
#define POT1_PIN  6
#define POT2_PIN  7
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    Wire.begin(8, 9);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}
 
void loop() {
    float v1 = analogRead(POT1_PIN) * 3.3 / 4095.0;
    float v2 = analogRead(POT2_PIN) * 3.3 / 4095.0;
 
    if (v1 > 2.0) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
 
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
 
    display.setCursor(0, 0);
    display.print("Pot6: ");
    display.print(v1, 2);
    display.println(" V");
 
    display.setCursor(0, 16);
    display.print("Pot7: ");
    display.print(v2, 2);
    display.println(" V");
 
    display.setCursor(0, 32);
    display.print("LED: ");
    display.println(v1 > 2.0 ? "ON" : "OFF");
 
    display.display();
    delay(200);
}

////////////////////////////////////////////////////////////
// 19. Integración completa
////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define LED_PIN      4
#define BTN_PIN      5
#define POT1_PIN     6
#define POT2_PIN     7
#define NEOPIXEL_PIN 48
#define NUM_PIXELS   1
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_NeoPixel pixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BTN_PIN, INPUT_PULLUP);
    Wire.begin(8, 9);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    pixel.begin();
    pixel.setBrightness(50);
}
 
void loop() {
    float v1 = analogRead(POT1_PIN) * 3.3 / 4095.0;
    float v2 = analogRead(POT2_PIN) * 3.3 / 4095.0;
    bool presionado = (digitalRead(BTN_PIN) == LOW);
 
    digitalWrite(LED_PIN, presionado ? HIGH : LOW);
 
    if (v1 < 1.1) {
        pixel.setPixelColor(0, pixel.Color(0, 0, 255));
    } else if (v1 < 2.2) {
        pixel.setPixelColor(0, pixel.Color(0, 255, 0));
    } else {
        pixel.setPixelColor(0, pixel.Color(255, 0, 0));
    }
    pixel.show();
 
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
 
    display.setCursor(0, 0);
    display.print("Pot6: "); display.print(v1, 2); display.println(" V");
 
    display.setCursor(0, 12);
    display.print("Pot7: "); display.print(v2, 2); display.println(" V");
 
    display.setCursor(0, 24);
    display.print("Boton: ");
    display.println(presionado ? "PRESIONADO" : "SUELTO");
 
    display.setCursor(0, 36);
    display.print("NeoPixel: ");
    if (v1 < 1.1)      display.println("Azul");
    else if (v1 < 2.2) display.println("Verde");
    else                display.println("Rojo");
 
    display.display();
 
    Serial.print("Pot6="); Serial.print(v1, 2);
    Serial.print(" Pot7="); Serial.print(v2, 2);
    Serial.print(" Btn="); Serial.println(presionado ? "ON" : "OFF");
 
    delay(200);
}

////////////////////////////////////////////////////////////
// 20. Código Base Recomendado
////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
 
#define LED_PIN      4
#define BTN_PIN      5
#define POT1_PIN     6
#define POT2_PIN     7
#define NEOPIXEL_PIN 48
#define NUM_PIXELS   1
 
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
Adafruit_NeoPixel pixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
    Serial.begin(115200);
 
    pinMode(LED_PIN, OUTPUT);
    pinMode(BTN_PIN, INPUT_PULLUP);
 
    Wire.begin(8, 9);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
 
    pixel.begin();
    pixel.setBrightness(50);
    pixel.show();
}
 
void loop() {
    float v1 = analogRead(POT1_PIN) * 3.3 / 4095.0;
    float v2 = analogRead(POT2_PIN) * 3.3 / 4095.0;
    bool presionado = (digitalRead(BTN_PIN) == LOW);
 
    Serial.print("Pot6="); Serial.print(v1, 2);
    Serial.print(" | Pot7="); Serial.print(v2, 2);
    Serial.print(" | Btn="); Serial.println(presionado);
 
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Pot6: "); display.print(v1, 2); display.println(" V");
    display.setCursor(0, 12);
    display.print("Pot7: "); display.print(v2, 2); display.println(" V");
    display.display();
 
    delay(200);
}