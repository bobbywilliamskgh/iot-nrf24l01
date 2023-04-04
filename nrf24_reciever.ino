//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <math.h>

//create an RF24 object
RF24 radio(D4, D8);  // CE, CSN

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

float data[2];
float suhu, kelembapan;

//address through which two modules communicate.
const byte address[6] = "00001";

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

void setup()
{
  while (!Serial);
    Serial.begin(9600);
  
  radio.begin();
  
  //set the address
  radio.openReadingPipe(1, address);
  
  //Set module as receiver
  radio.startListening();

   // konek ke wifi
  WiFi.begin("Redmi Note 11", "redminote");
  while (!WiFi.isConnected()) delay(500);

    // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
}

void loop()
{
  // HTTP request ke thingspeak
  WiFiClient wifi;
  HTTPClient http;
  
  //Read the data if available in buffer
  if (radio.available())
  {
    // Baca data dari sender
    radio.read(data, sizeof(data));

    // masukkan nilai array data ke variabel suhu dan kelembapan
    suhu = data[0];
    kelembapan = data[1];

    // cetak ke serial
    Serial.print(suhu);
    Serial.print(",");
    Serial.println(kelembapan);

    // cetak ke thingspeak   
    http.begin(wifi, "http://api.thingspeak.com/update");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.POST("api_key=OIT3CPP5DYZSDJA9&field1=" + String(suhu) + "&field2=" + kelembapan);
    http.end();

    // cetak ke lcd
    print_lcd(suhu, kelembapan);
  }
  delay(1000);
}
