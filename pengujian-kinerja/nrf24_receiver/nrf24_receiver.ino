//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <math.h>

struct Data { int seq; float tc; float rh; bool goodSignal; };
Data data;
RF24 radio(D4, D8);

const byte addr[6] = "00001";

void setup() {
  Serial.begin(115200);
  // Serial.print("hello");
  // Serial.print(sizeof(data));
  radio.begin();
  radio.setChannel(100);
  radio.setDataRate(RF24_250KBPS);
  
  radio.openReadingPipe(1, addr);
  radio.startListening();
}

void loop() { 
  if (radio.available()) {
    radio.read(&data, sizeof(data));
    Serial.print(data.seq); Serial.print(", ");
    Serial.print(data.tc);  Serial.print(", ");
    Serial.print(data.rh);  Serial.print(", ");
    Serial.println(data.goodSignal);
  }
}
