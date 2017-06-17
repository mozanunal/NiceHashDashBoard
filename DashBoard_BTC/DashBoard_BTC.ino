/*
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;


const char ssid_1 [] = "BABALAR";
const char pass_1 [] = "24939300";
const char BitCoinWallet [] = "1FVJWQaaobMvr9omaRo566xRP5BJQAkE4k";
char url_Currency [] = "http://api.nicehash.com/api?method=stats.global.current";
char url_Balence[255];




void setup() {
  
    //Setup Serial
    Serial.begin(115200);
    delay(10);
    
    //Setup URLs
    char* url = "https://api.nicehash.com/api?method=stats.provider&addr=";
    strcpy( url_Balence, url );
    strcat( url_Balence, BitCoinWallet );
    Serial.println();
    Serial.println(url_Balence);

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

    
}


void loop() {

  if (WiFiMulti.run() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin(url_Currency);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
 
    }
 
    http.end();   //Close connection
 
  }
 
  delay(30000);    //Send a request every 30 seconds
}



