/*
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

//User Settings
const char ssid_1 [] = "xx";
const char pass_1 [] = "xx";
const char BitCoinWallet [] = "xx";

//Globals
char url_Currency [] = "http://blockchain.info/ticker";
char url_Balance_base[] = "http://api.nicehash.com/api?method=stats.provider&addr=";
char url_Balence[255];
ESP8266WiFiMulti WiFiMulti;
SSD1306  display(0x3c, D3, D5);


void setup() 
{
  
    //Setup Serial
    Serial.begin(115200);
    delay(10);
    
    //Setup URLs
    strcpy( url_Balence, url_Balance_base );
    strcat( url_Balence, BitCoinWallet );
    Serial.println();

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

String getJsonFromUrl(char *url)
{
    HTTPClient http;  //Declare an object of class HTTPClient
    String payload;
    http.begin(url);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
    if (httpCode > 0) //Check the returning code
    { 
      payload = http.getString();   //Get the request response payload
      Serial.println(payload);      //Print the response payloa
    }
    http.end();   //Close connection
    return payload;
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
  Serial.println(line1);

  // Some test data
  char* test[] = {
    line1,
    line2,
    line3
  };

  for (uint8_t i = 0; i < 3; i++) {
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

DynamicJsonBuffer jsonBuffer_Currency;
DynamicJsonBuffer jsonBuffer_Balance;

void loop() 
{

  if (WiFiMulti.run() == WL_CONNECTED) //Check WiFi connection status
  { 


    //Currency
    JsonObject& root_Currency = jsonBuffer_Currency.parseObject( getJsonFromUrl(url_Currency) );
    double currency = root_Currency["USD"]["last"];

    for (int i = 0; i < 150; i++ )
    {
      //Balance
      delay(5000);
      JsonObject& root_Balance = jsonBuffer_Balance.parseObject( getJsonFromUrl(url_Balence) );
  
      double balances[6];
      int algos[6];
      double total_balance_btc = 0.0;
      for (int i = 0; i<6; i++)
      {
          algos[i] = root_Balance["result"]["stats"][i]["algo"];
          const char* balchar = root_Balance["result"]["stats"][i]["balance"];
          balances[i] = strtod(balchar, NULL);
          //Serial.println(algos[i]);
          //Serial.println(balchar);
          //Serial.println(balances[i],8);
          total_balance_btc += balances[i];
      }
      
      Serial.print("1 BTC: ");
      Serial.print(currency,9);
      Serial.println(" USD");
      Serial.print("Total BTC: ");
      Serial.println(total_balance_btc,9);
      Serial.print("Total USD: ");
      Serial.println(total_balance_btc*currency,9);
      printBuffer(currency, total_balance_btc, total_balance_btc*currency);
    }


  }
  
}







