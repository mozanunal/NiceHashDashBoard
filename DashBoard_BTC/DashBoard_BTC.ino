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
const char ssid_1 [] = "";
const char pass_1 [] = "";
const char ssid_2 [] = "";
const char pass_2 [] = "";
const char BitCoinWallet [] = "";

//Globals
ESP8266WiFiMulti WiFiMulti;
SSD1306 display(0x3c, D3, D5);


void setup() 
{
    //Setup Oled
    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.clear();


    //Setup Serial
    Serial.begin(115200);
    delay(10);

    //Setup Wifi
    WiFiMulti.addAP( ssid_1 , pass_1);
    WiFiMulti.addAP( ssid_2 , pass_2);
    display.clear();
    display.drawString(0 , 20, "Waiting for wifi...");
    display.display();
    int xpos = 0;
    while(WiFiMulti.run() != WL_CONNECTED) {
        display.drawString(xpos ,40 , ".");
        xpos= xpos + 3;
        display.display();
        delay(500);
    }
    display.clear();
    display.drawString(0 , 20, "Connected to wifi...");
    //display.drawString(0 , 40, WiFi.localIP()));
    display.display();
    delay(2000);
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


int loopCounter = 0;
double currency = 0;
double total_balance_btc = 0;
void loop() 
{
  if (WiFiMulti.run() == WL_CONNECTED) //Check WiFi connection status
  { 
    display.clear();
    display.drawString(0 , 20, "UPDATING...");
    display.display();
    if(loopCounter%20==0){ currency = getCurrency(); }
    if(loopCounter%2==0){ total_balance_btc = getTotalBTC(BitCoinWallet); }
    ///////////////
    Serial.print("1 BTC: ");
    Serial.print(currency,9);
    Serial.println(" USD");
    Serial.print("Total BTC: ");
    Serial.println(total_balance_btc,9);
    Serial.print("Total USD: ");
    Serial.println(total_balance_btc*currency,9);
    ///////////
    for(int i = 0; i<3; i++)// 3 frame * 2000 ms * 10 loop
    {
      frameLoop(currency, total_balance_btc, total_balance_btc*currency);
    }
    loopCounter++;
  }//end of wifi check
  
}//end of loop







