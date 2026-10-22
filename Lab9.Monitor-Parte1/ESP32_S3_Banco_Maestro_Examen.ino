/*
 ESP32_S3_Banco_Maestro_Examen.ino
 Banco de estudio para examen
 OLED SSD1306 128x32
 SDA=8 SCL=9
 POT1=4 POT2=5
 SW=6 BTN1=7 BTN2=15
 LED1=10 LED2=11 LED3=12 LED4=13
 RGB=48
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define POT1 4
#define POT2 5
#define SW 6
#define BTN1 7
#define BTN2 15
#define LED1 10
#define LED2 11
#define LED3 12
#define LED4 13
#define RGBPIN 48

Adafruit_SSD1306 display(128,32,&Wire,-1);
Adafruit_NeoPixel rgb(1,RGBPIN,NEO_GRB+NEO_KHZ800);

volatile long contador=0;
unsigned long t0=0;
int modo=0;

void IRAM_ATTR isrBtn(){ contador++; }

void setup(){
  Serial.begin(115200);

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);

  pinMode(SW,INPUT_PULLUP);
  pinMode(BTN1,INPUT_PULLUP);
  pinMode(BTN2,INPUT_PULLUP);

  Wire.begin(8,9);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();
  display.display();

  rgb.begin();
  rgb.show();

  attachInterrupt(digitalPinToInterrupt(BTN1),isrBtn,FALLING);

  Serial.println("MENU:");
  Serial.println("1 ADC POT1");
  Serial.println("2 ADC POT2");
  Serial.println("3 Voltimetro Dual");
  Serial.println("4 OLED");
  Serial.println("5 Barra OLED");
  Serial.println("6 Secuencia LEDs");
  Serial.println("7 Knight Rider");
  Serial.println("8 Switch->LED");
  Serial.println("9 Contador ISR");
  Serial.println("10 RGB Pot");
  Serial.println("11 RGB Serial");
  Serial.println("12 Cronometro");
}

void loop(){

  if(Serial.available()){
    modo=Serial.parseInt();
  }

  switch(modo){
    case 1: modoADC1(); break;
    case 2: modoADC2(); break;
    case 3: modoVoltimetroDual(); break;
    case 4: modoOLED(); break;
    case 5: modoBarra(); break;
    case 6: modoSecuencia(); break;
    case 7: modoKnight(); break;
    case 8: modoSwitch(); break;
    case 9: modoContador(); break;
    case 10: modoRGBPot(); break;
    case 11: modoRGBSerial(); break;
    case 12: modoCronometro(); break;
  }
}

void modoADC1(){
  Serial.println(analogRead(POT1));
  delay(200);
}

void modoADC2(){
  Serial.println(analogRead(POT2));
  delay(200);
}

void modoVoltimetroDual(){
  float v1=analogRead(POT1)*3.3/4095.0;
  float v2=analogRead(POT2)*3.3/4095.0;

  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("V1="); display.println(v1);
  display.print("V2="); display.println(v2);
  display.display();
}

void modoOLED(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("EXAMEN");
  display.display();
}

void modoBarra(){
  int adc=analogRead(POT1);
  int ancho=map(adc,0,4095,0,128);

  display.clearDisplay();
  display.drawRect(0,10,128,12,WHITE);
  display.fillRect(0,10,ancho,12,WHITE);
  display.display();
}

void modoSecuencia(){
  static int led=0;

  if(millis()-t0>300){
    t0=millis();

    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED4,LOW);

    int p[4]={LED1,LED2,LED3,LED4};
    digitalWrite(p[led],HIGH);

    led=(led+1)%4;
  }
}

void modoKnight(){
  static int seq[6]={LED1,LED2,LED3,LED4,LED3,LED2};
  static int i=0;

  if(millis()-t0>200){
    t0=millis();

    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED4,LOW);

    digitalWrite(seq[i],HIGH);

    i++;
    if(i>=6)i=0;
  }
}

void modoSwitch(){
  digitalWrite(LED1,!digitalRead(SW));
}

void modoContador(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Cont:");
  display.println(contador);
  display.display();
}

void modoRGBPot(){
  int adc=analogRead(POT1);

  if(adc<1365) rgb.setPixelColor(0,rgb.Color(255,0,0));
  else if(adc<2730) rgb.setPixelColor(0,rgb.Color(0,255,0));
  else rgb.setPixelColor(0,rgb.Color(0,0,255));

  rgb.show();
}

void modoRGBSerial(){
  if(Serial.available()){
    char c=Serial.read();

    if(c=='R') rgb.setPixelColor(0,rgb.Color(255,0,0));
    if(c=='G') rgb.setPixelColor(0,rgb.Color(0,255,0));
    if(c=='B') rgb.setPixelColor(0,rgb.Color(0,0,255));

    rgb.show();
  }
}

void modoCronometro(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("ms:");
  display.println(millis());
  display.display();
}
