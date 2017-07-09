/*
 *This is a library file which is using for data parser of btc currency api and nice hash bitcoin wallet api 
 */
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "dataParser.h"

//Globals
char url_Currency [] = "http://blockchain.info/ticker";
char url_Balance_base[] = "http://api.nicehash.com/api?method=stats.provider&addr=";
char url_Balence[255];


DynamicJsonBuffer jsonBuffer_Currency;
DynamicJsonBuffer jsonBuffer_Balance;

String getJsonFromUrl(char *url)
{
    HTTPClient http;  //Declare an object of class HTTPClient
    String payload;
    http.begin(url);  //Specify request destination
    int httpCode = http.GET(); //Send the request
    if (httpCode > 0) //Check the returning code
    { 
      payload = http.getString();   //Get the request response payload
      //Serial.println(payload);      //Print the response payloa
    }
    http.end();   //Close connection
    return payload;
}

double getCurrency()
{
    JsonObject& root_Currency = jsonBuffer_Currency.parseObject( getJsonFromUrl(url_Currency) );
    double currency = root_Currency["USD"]["last"];
    return currency;
}

double getTotalBTC(const char *BitCoinWallet)
{     
  //Setup URLs
  strcpy( url_Balence, url_Balance_base );
  strcat( url_Balence, BitCoinWallet );
  JsonObject& root_Balance = jsonBuffer_Balance.parseObject( getJsonFromUrl(url_Balence) );
  double balances[6];
  int algos[6];
  double total_balance_btc = 0.0;
  for (int i = 0; i<6; i++)
  {
      algos[i] = root_Balance["result"]["stats"][i]["algo"];
      const char* balchar = root_Balance["result"]["stats"][i]["balance"];
      balances[i] = strtod(balchar, NULL);
      total_balance_btc += balances[i];
  }
  return total_balance_btc;
}




