

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__WIFI_POINT

#include <WiFi.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "ESP32S3"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377

#define LED_PIN 4 //defino el pin para el led
#define ADC_PIN 5 //indico donde va el potenciometro


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] =   // 69 bytes V19 
  { 255,1,0,4,0,62,0,19,0,0,0,0,31,1,106,200,1,1,2,0,
  2,32,43,44,22,0,135,26,31,31,79,78,0,79,70,70,0,71,25,99,
  57,57,56,0,149,144,135,0,0,0,0,0,240,127,69,0,0,122,68,0,
  0,250,67,0,0,72,66,24,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_01; // =1 if switch ON and =0 if OFF, from 0 to 1

    // output variables
  float instrument_01; // from 0 to 4095

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////




void setup() 
{
  RemoteXY_Init ();  // initialization by macros 
  pinMode(LED_PIN,OUTPUT); //EL PIN GPIO4 será una salida digital
  Serial.begin(115200);
  analogReadResolution(12); //configuracion del ADC
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXYEngine.handler ();   
  digitalWrite(LED_PIN,RemoteXY.switch_01); //enciende o apaga el LED segun la app Remote XY
  int adcValue = analogRead(ADC_PIN); //leo el ADC
  Serial.print("Valor del ADC: ");
  Serial.println(adcValue);

  RemoteXY.instrument_01 = adcValue; //le mando el valor ADC a la APP
  RemoteXY_delay(500);
   
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXYEngine.delay() 

}