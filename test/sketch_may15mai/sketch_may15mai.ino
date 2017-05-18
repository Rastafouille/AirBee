struct payload {
  uint8_t id = 0x02;
  int16_t temperature=-123;
  uint16_t humidity=523;
  uint16_t poids=1263;
  uint16_t bee_in=2569;
  uint16_t bee_out=6589;
  uint8_t vbat=48;
} ;
#include <SoftwareSerial.h>
SoftwareSerial SigFox(4,5); // RX, TX
 
void setup() {
  payload data; 
  //Serial.begin(9600);
  SigFox.begin(9600);
  SigFox.print("AT$SF=");
  SigFox.print(data.id,HEX);
  //SigFox.print(data.temperature,BIN);
  //SigFox.print(data.humidity,BIN);
  //SigFox.print(data.poids,BIN);
  //SigFox.print(data.bee_in,BIN);
  //SigFox.print(data.bee_out,BIN);
  //SigFox.print(data.vbat,BIN);
  SigFox.println(); 
}
 
void loop() {}
