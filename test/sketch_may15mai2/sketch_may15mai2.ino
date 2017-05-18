typedef struct rowPayload_s {
  uint8_t id;
  int16_t temperature;
  uint16_t humidity;
  uint16_t poids; // pas terrible de mélanger français et anglais dans les noms des champs, faites un choix
  uint16_t in; // pas très parlant comme nom de variable
  uint16_t out; // pas très parlant comme nom de variable
  uint8_t vba; // pas très parlant comme nom de variable
};

union payload_u
{
  rowPayload_s data;
  uint8_t rawData[sizeof(rowPayload_s)];
} payload;

#include <SoftwareSerial.h>
SoftwareSerial SigFox(4,5); // RX, TX

void setup() {

  SigFox.begin(9600);
  Serial.begin(9600);


  // on initialise avec des valeurs reconnnaissables
  payload.data.id = 0xAA;
  payload.data.temperature = -123;
  payload.data.humidity = 523;
  payload.data.poids = 2563;
  payload.data.in = 5698;
  payload.data.out = 3562;
  payload.data.vba = 48;

  SigFox.print("AT$SF=");

  for (byte i = 0; i < sizeof(payload); i++) {
    if (payload.rawData[i] <= 0xF) SigFox.print("0"); // pour bien avoir 2 caractères
    SigFox.print(payload.rawData[i], HEX);
  }
  SigFox.print("\r");
    Serial.println("DONE");


}

void loop() {}
