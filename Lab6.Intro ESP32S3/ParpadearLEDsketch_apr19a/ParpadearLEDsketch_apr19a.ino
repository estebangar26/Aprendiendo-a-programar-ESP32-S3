
#define LED_PIN 4 //defino el pin para el led
#define ADC_PIN 5 //indico donde va el potenciometro

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN,OUTPUT); //EL PIN GPIO4 será una salida digital
  Serial.begin(115200);
  analogReadResolution(12); //configuracion del ADC
  /* 3,3 --- 111111111111=4095
     0,0 --- 000000000000=0*/

}

/*Este es el código es el ciclo infinito
donde pongo mi codigo principal*/
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN,HIGH); //enciende el LED
  delay(500);   //el micocontrolador espera un retardo de 500 ms
  digitalWrite(LED_PIN,LOW); //apaga el LED
  delay(500);   //el micocontrolador espera un retardo de 500 ms

  int adcValue = analogRead(ADC_PIN);
  adcValue = analogRead(ADC_PIN); //lEO EL ADC

  
  Serial.print("Valor del ADC: ");
  Serial.println(adcValue);
}
