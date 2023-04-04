//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "SHT85.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


#define SHT85_ADDRESS         0x45

SHT85 sht;

float data[2];

//create an RF24 object
RF24 radio(D4, D8);  // CE, CSN

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

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

  Wire.begin();
  sht.begin(SHT85_ADDRESS);
  Wire.setClock(100000);

  // konek ke nrf24l01
  radio.begin();
  radio.openWritingPipe(address); //set the address
  radio.stopListening(); //Set module as transmitter

   // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
}

void loop()
{
  sht.read();         // default = true/fast       slow = false
  float suhu = sht.getTemperature();
  float kelembapan = sht.getHumidity();


  // Isi data suhu dan kelembapan yang akan dikirim ke nRF24L01 receiver
  data[0] = suhu;
  data[1] = kelembapan;
  
  //Send message to receiver
  radio.write(data, sizeof(data));

  // cetak ke lcd
  print_lcd(suhu, kelembapan);
  
  delay(5000);
}
