

//#include <Wire.h>
//
//// Cooking API libraries
//#include <arduinoUART.h>
//#include <arduinoUtils.h>
//
//// Sigfox library
//#include <arduinoSigfox.h>
//
////////////////////////////////////////////////
//uint8_t socket = SOCKET0;     //Asign to UART0
////////////////////////////////////////////////
//
//unsigned long pausetime = 30000;  // pause in ms
//unsigned long previousMillis = 0; 

#include <SoftwareSerial.h>
SoftwareSerial SigFox(4,5); // RX, TX

// define variable to create a speficic frame to be sent
uint8_t data[12];

// define union-type variables
union
{
  String AT="AT$SF=";
  uint8_t id=1;
  int16_t temperature=-126;
  uint16_t humidity=563;
  uint16_t poids=1257;
  uint16_t in=2569;
  uint16_t out=5149;
  uint8_t vbat=51;
}data_union;

int size=sizeof(data_union.AT)+12;
Serial.println(size);


void setup() 
{
  
    Serial.begin(9600); // Open serial connection to report values to host
  Serial.println("Starting up");
       SigFox.begin(9600);

}


void loop() 
{
  
//        data[0] = id; // big-endian
//        data[1] = highByte(temperature);
//        data[2] = lowByte(temperature);
//        data[3] = highByte(humidity);
//        data[4] =lowByte(humidity);
//        data[5] = highByte(poids);
//        data[6] = lowByte(poids);
//        data[7] =highByte(in);
//        data[8] = lowByte(in);
//        data[9] = highByte(out);
//        data[10] =lowByte(out);
//        data[11] = vbat;
        
  String str = (char*)data_union;
  String str2 =String(String("AT$SF=")+ String(data));
        
  SigFox.write(String("AT$SF=")+String("11"));
  //SigFox.println(str);


  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000000); 


    
//    //////////////////////////////////////////////
//    // 2. create array with sensor data
//    // Sigfox Back-END device "Display type" must be: 
//    // Temp::float:32 Battery::uint:8 Digital8::bool:7 Digital7::bool:6
//    //       Digital6::bool:5 Digital5::bool:4 Digital4::bool:3
//    //       Digital3::bool:2 Digital2::bool:1 Digital1::bool:0
//    //////////////////////////////////////////////
//  
//    // 2. Reading sensors:
////    data_union.id =1
////    data_union.temperature = -106;
////    data_union.humidity = 523;
////    data_union.poids = 527;
////    data_union.in = 5695;
////    data_union.out = 3287;
////    data_union.vbat = 32;
//  
//    // 2.4. Fill structure fields
//    temp_union.value2 = temperature;  
//  
//    // fill 'data' buffer with data
//    data[0] = temp_union.value1[3]; // big-endian
//    data[1] = temp_union.value1[2];
//    data[2] = temp_union.value1[1];
//    data[3] = temp_union.value1[0];
//    data[4] = battery;
//    data[5] = digitalPins;
//    size = 6;
//         
//    //Final Frame to send in "data"
//  
//    //////////////////////////////////////////////
//    // 3. send data
//    //////////////////////////////////////////////
//   
//    //3. Sending packet:
//    error = Sigfox.send(data,size);
//  
//    // Check sending status
//    if( error == 0 ) 
//    {
//      //"Sigfox packet sent OK"
//      digitalWrite(error_led, LOW);     
//    }
//    else 
//    {
//      //"Sigfox packet sent ERROR" 
//      digitalWrite(error_led, HIGH);
//    } 
//  }
  //////////////////////////////////////////////
  // 4. sleep pause_time (minutes)
  //////////////////////////////////////////////
}
