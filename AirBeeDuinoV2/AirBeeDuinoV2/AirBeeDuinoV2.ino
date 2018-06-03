//TIME
#include <TimeLib.h>
int heure = 99;
int ahora;

//activation du Serial pour le debug
byte DEBUG = false;

//WEIGHT https://github.com/bogde/HX711
#include "HX711.h"
HX711 scale;
float final;
float inter;

// SHT15 Temperature et humidité https://github.com/practicalarduino/SHT1x
#include <SHT1x.h>
#define dataPin  2
#define clockPin 3
SHT1x sht1x(dataPin, clockPin);

// connexion serie pour Sigfox
  #include <SoftwareSerial.h>
  SoftwareSerial SigFox(4,5); // RX, TX

//définition des variable pour le payload SF
  typedef struct rowPayload_s 
  {
    uint8_t id;
    int16_t temperature;
    uint16_t humidity;
    uint16_t weight; 
    uint16_t bee_in; 
    uint16_t bee_out; 
    uint8_t vbat; 
  };

  union payload_u
  {
    rowPayload_s data;
    uint8_t rawData[sizeof(rowPayload_s)];
  } payload;

//

void setup()
{
  pinMode(13, OUTPUT);
  
  if (DEBUG) 
  { Serial.begin(9600);Serial.println("Starting up"); delay(100);}
  SigFox.begin(9600);
  ahora = hour();
  if (DEBUG){print_date();  }
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  
  // WEIGHT
  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT	- pin #A1
  // HX711.PD_SCK	- pin #A0
  scale.begin(A1, A0);
  //scale.set_scale();
  scale.tare();
}

void loop()
{
  if (DEBUG){print_date();  }
       if (heure!=hour())
        { 
          heure=hour();
            if (DEBUG) {Serial.print("Heure differente: ");Serial.println (heure);}
          // on charge les valeurs
          payload.data.id=1;
          payload.data.temperature = int16_t (sht1x.readTemperatureC()*10);
          payload.data.humidity = int16_t (sht1x.readHumidity()*10);
          payload.data.weight=int16_t (getweight()*10);
          payload.data.bee_in=0;
          payload.data.bee_out=0;
          payload.data.vbat= int(analogRead(A3) * (50 / 1023.0));
           
          //on affiche
              if (DEBUG) {print_payload();}
   
          // On balance
            envoieSF();
        }
    delay(1000);
}
  
void print_date () 
{
    Serial.print(hour(), DEC);Serial.print(':');
    Serial.print(minute(), DEC);Serial.print(':');
    Serial.print(second(), DEC);Serial.println();
}

void envoieSF() 
{   
  if (DEBUG) {Serial.println("ENVOIE SF...");}
  SigFox.print("AT$SF=");
  for (byte i = 0; i < sizeof(payload); i++) 
  {
    if (payload.rawData[i] <= 0xF) SigFox.print("0"); // pour bien avoir 2 caractères
    SigFox.print(payload.rawData[i], HEX);
  }
  SigFox.print("\r");
  if (DEBUG) {Serial.println("ENVOIE SF OK !"); }
 }

void print_payload()
{ 
  Serial.println("-----PAYLOAD-----");
  Serial.print("ID:");Serial.println(payload.data.id);
  Serial.print("Temperature: ");Serial.print(float (payload.data.temperature)/10,1);Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(float(payload.data.humidity)/10,1);Serial.println(" %");
  Serial.print("Poids:");Serial.print(float(payload.data.weight)/10,1);Serial.println(" kg");
  Serial.print("Bee IN:");Serial.println(payload.data.bee_in);
  Serial.print("Bee OUT:");Serial.println(payload.data.bee_out);
  Serial.print("Vbat:");Serial.print(float(payload.data.vbat)/10,1);Serial.println(" Volt");
  Serial.println("-----------------");
}


float getweight(void)
{
  scale.power_up();
  final=scale.get_value(10)/21500;
  scale.power_down();			        // put the ADC in sleep mode
  //if (final>0) {
  return final;
//}
  //else {return 0;}
}



