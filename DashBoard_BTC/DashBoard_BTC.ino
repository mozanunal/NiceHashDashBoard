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
const char ssid_1 [] = "BABALAR";
const char pass_1 [] = "24939300";
const char BitCoinWallet [] = "1FVJWQaaobMvr9omaRo566xRP5BJQAkE4k";

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




void frameLoop(double currency, double totol_balance_btc, double total_balance)
{
  
  //currency
  delay(2000);
  char currency_line[30];
  dtostrf(currency,0, 2, currency_line);
  sprintf(currency_line,"%s %s", currency_line, " $");
  display.clear();
  display.drawString(0 , 0,  "1 BTC:");
  display.drawString(0 , 20, currency_line);
  display.display();


  // total dollar
  delay(2000);
  char total_balance_line[30];
  dtostrf(total_balance,0, 8, total_balance_line); 
  sprintf(total_balance_line,"%s %s", total_balance_line, " $");
  display.clear();
  display.drawString(0 , 0,  "Total:");
  display.drawString(0 , 20, total_balance_line);
  display.display();


  // total btc
  delay(2000);
  char totol_balance_btc_line[30];
  dtostrf(totol_balance_btc,0, 8, totol_balance_btc_line);
  sprintf(totol_balance_btc_line,"%s %s", totol_balance_btc_line, " BTC");
  display.clear();
  display.drawString(0 , 0,  "Total:");
  display.drawString(0 , 20, totol_balance_btc_line);
  display.display();
  
} 


void loop() 
{
  if (WiFiMulti.run() == WL_CONNECTED) //Check WiFi connection status
  { 

    display.clear();
    display.drawString(0 , 20, "UPDATING...");
    display.display();
    double currency = getCurrency();
    double total_balance_btc = getTotalBTC(BitCoinWallet);
    Serial.print("1 BTC: ");
    Serial.print(currency,9);
    Serial.println(" USD");
    Serial.print("Total BTC: ");
    Serial.println(total_balance_btc,9);
    Serial.print("Total USD: ");
    Serial.println(total_balance_btc*currency,9);
    for(int i = 0; i<10; i++)// 3 * 2000 ms * 10
    {
      frameLoop(currency, total_balance_btc, total_balance_btc*currency);
    }
    
  }//end of wifi check
  
}//end of loop







