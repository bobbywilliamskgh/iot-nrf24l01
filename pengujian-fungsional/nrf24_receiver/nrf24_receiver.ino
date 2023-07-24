//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

struct Data { int seq; float tc; float rh; bool goodSignal; };
Data data;
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
  Serial.begin(115200);
  // Serial.print("hello");
  // Serial.print(sizeof(data));
  radio.begin();
  radio.setChannel(100);
  radio.setDataRate(RF24_250KBPS);
  
  radio.openReadingPipe(1, addr);
  radio.startListening();
  // radio.printPrettyDetails();

  // konek ke wifi
  WiFi.begin("Redmi Note 11", "redminote");
  while (!WiFi.isConnected()) delay(500);

  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
}

void loop() {
  // HTTP request ke thingspeak
  WiFiClient wifi;
  HTTPClient http;
  
  if (radio.available()) {
    radio.read(&data, sizeof(data));
    Serial.print(data.seq); Serial.print(", ");
    Serial.print(data.tc);  Serial.print(", ");
    Serial.print(data.rh);  Serial.print(", ");
    Serial.println(data.goodSignal);

    // cetak ke thingspeak   
    http.begin(wifi, "http://api.thingspeak.com/update");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.POST("api_key=OIT3CPP5DYZSDJA9&field1=" + String(data.tc) + "&field2=" + data.rh);
    http.end();

    // cetak ke lcd
    print_lcd(data.tc, data.rh);
  }
}
