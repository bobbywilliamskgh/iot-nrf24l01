//Include Libraries
//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "SHT85.h"
#include <LiquidCrystal_I2C.h>

struct Data { int seq = 0; float tc; float rh; bool goodSignal; };
Data data;
SHT85 sht;
RF24 radio(D4, D8);


// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);


const byte addr[6] = "00001";


void print_lcd(float s, float k) {
  lcd.clear();
  lcd.print("T : "); 
  lcd.print(s, 1);
  lcd.print((char)223); // kode derajat suhu
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("RH: ");
  lcd.print(k, 1);
  lcd.print(" %");
}


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

  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
}

void loop() {
  sht.read(); 
  data.tc = sht.getTemperature();
  data.rh = sht.getHumidity();
  data.goodSignal = radio.testRPD();
  data.seq++; 
  radio.write(&data, sizeof(data));

  // cetak ke lcd
  print_lcd(data.tc, data.rh);
 
  delay(30000);
}
