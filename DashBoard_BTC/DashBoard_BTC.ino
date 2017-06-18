/*
 *
 */

//ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
//Oled Screen
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
//Self developed files
#include "dataParser.h"
#include "frames.h"

//User Settings
const char ssid_1 [] = "xx";
const char pass_1 [] = "xx";
const char BitCoinWallet [] = "xx";

//Globals
ESP8266WiFiMulti WiFiMulti;
SSD1306 display(0x3c, D3, D5);


void setup() 
{
  
    //Setup Serial
    Serial.begin(115200);
    delay(10);

    //Setup Wifi
    WiFiMulti.addAP( ssid_1 , pass_1);
    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");
    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(500);

    //Setup Oled
    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.clear();
    
}



void printBuffer(double currency, double totol_balance_btc, double total_balance ) 
{
  // Initialize the log buffer
  // allocate memory to store 3 lines of text and 30 chars per line.
  display.setLogBuffer(3, 30);

  char line1[30];
  char line2[30];
  char line3[30];
  
  dtostrf(currency,0, 8, line1);
  dtostrf(totol_balance_btc,0, 8, line2);
  dtostrf(total_balance,0, 8, line3);

  // Some test data
  char* test[] = {
    line1,
    line2,
    line3
  };

  for (uint8_t i = 0; i < 3; i++) 
  {
    display.clear();
    // Print to the screen
    display.println(test[i]);
    // Draw it to the internal screen buffer
    display.drawLogBuffer(0, 0);
    // Display it on the screen
    display.display();
    delay(500);
  }
}


void loop() 
{

  if (WiFiMulti.run() == WL_CONNECTED) //Check WiFi connection status
  { 
    delay(5000);
    double currency = getCurrency();
    for(int i = 0; i<150; i++)
    {
      delay(5000);
      double total_balance_btc = getTotalBTC(BitCoinWallet);
      
      Serial.print("1 BTC: ");
      Serial.print(currency,9);
      Serial.println(" USD");
      Serial.print("Total BTC: ");
      Serial.println(total_balance_btc,9);
      Serial.print("Total USD: ");
      Serial.println(total_balance_btc*currency,9);
      printBuffer(currency, total_balance_btc, total_balance_btc*currency);
    }
    
  }//end of wifi check
  
}//end of loop







