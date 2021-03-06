//activation du Serial pour le debug
byte DEBUG = false;

//WEIGHT https://github.com/bogde/HX711
#include "HX711.h"
HX711 scale;
float final;
float inter;


//SLEEP MODE http://donalmorrissey.blogspot.fr/2010/04/sleeping-arduino-part-5-wake-up-via.html
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
volatile int f_wdt=1;


// SHT15 Temperature et humidité https://github.com/practicalarduino/SHT1x
#include <SHT1x.h>
#define dataPin  2
#define clockPin 3
SHT1x sht1x(dataPin, clockPin);


// DS1307 RTC connected via I2C and Wire lib
  #include <Wire.h>
  #include "RTClib.h" //https://github.com/adafruit/RTClib.git
  RTC_DS1307 rtc;
  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

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
int heure = 99;
DateTime now;

void setup()
{
  pinMode(13, OUTPUT);
  
  if (DEBUG) 
  { Serial.begin(9600);Serial.println("Starting up"); delay(100);}
  SigFox.begin(9600);
  rtc.begin();now = rtc.now();
  if (DEBUG){print_date();  }
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   
 /*** Setup the WDT ***/
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  /* In order to change WDE or the prescaler, we need to set WDCE (This will allow updates for 4 clock cycles).*/
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  
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
  
if(f_wdt == 1)
  {
      now = rtc.now();
       if (now.hour()!=heure)
        { 
          heure=now.hour();
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
       
    /* Don't forget to clear the flag. */
    f_wdt = 0;
    
    /* Re-enter sleep mode. */
    enterSleep();
  }
}
  
void print_date () 
{
    Serial.print(now.year(), DEC);Serial.print('/');
    Serial.print(now.month(), DEC);Serial.print('/');
    Serial.print(now.day(), DEC);Serial.print(" - ");
    Serial.print(now.hour(), DEC);Serial.print(':');
    Serial.print(now.minute(), DEC);Serial.print(':');
    Serial.print(now.second(), DEC);Serial.println();
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

ISR(WDT_vect)
{
  if(f_wdt == 0){f_wdt=1;}
  //else  {Serial.println("WDT Overrun!!!");}
}


void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);   
  sleep_enable();delay(100);
  
  /* Now enter sleep mode. */
    if (DEBUG) {Serial.println("SLEEP Mode activated");}
    delay(100);
  sleep_mode();
  
    /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();delay(100);
  if (DEBUG) {Serial.println("SLEEP Mode disactivated");}
}
float getweight(void)
{
  scale.power_up();
  final=scale.get_value(10)/21500;
  scale.power_down();			        // put the ADC in sleep mode
  if (final>0) {return final;}
  else {return 0;}
}



