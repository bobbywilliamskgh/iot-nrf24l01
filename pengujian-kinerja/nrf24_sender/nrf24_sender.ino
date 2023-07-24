//Include Libraries
//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "SHT85.h"

struct Data { int seq = 0; float tc; float rh; bool goodSignal; };
Data data;
SHT85 sht;
RF24 radio(D4, D8);


const byte addr[6] = "00001";



void setup() {
  sht.begin(0x45);

  Serial.begin(115200);
  //Serial.print("hello");
   
  radio.begin();
  radio.setChannel(100);
  radio.setDataRate(RF24_250KBPS);
 
  radio.openWritingPipe(addr);
  radio.stopListening(); 
  // radio.printPrettyDetails();
}

void loop() {
  sht.read(); 
  data.tc = sht.getTemperature();
  data.rh = sht.getHumidity();
  data.goodSignal = radio.testRPD();
  data.seq++; 
  radio.write(&data, sizeof(data));
  
  delay(5000);
}
