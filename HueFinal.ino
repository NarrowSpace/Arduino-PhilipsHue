/*
This Script is based on and derived from the tigoe's hue-control file

Here is the link to the file
https://github.com/tigoe/hue-control/blob/main/ArduinoExamples/HueBlinkWithJson/HueBlinkWithJson.ino

Remember to download arduino_secrets.h file, otherwise you have manually create the WIFI info in the script

*/


#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include "arduino_secrets.h"


//Change the follow info in the "arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
char hueHubIP[] = HUE_BRIDGE;
String hueUserName = API_KEY;

// WIFI status
int status = WL_IDLE_STATUS;

// make a wifi instance and a HttpClient instance:
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, hueHubIP);

// a JSON object to hold the light state:
JSONVar lightState;

int hueOver = 0; // 0 = Red 

// int hueBlue = 43000; //40000 plus = blue

int hueUnder = 24382; // 20000 plus =  Green


//Light sensor
int sensorPin = A0;
int sensorVal;


void setup() {

  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network IP = ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}


void loop() {
  // Read sensor Val
  sensorVal = analogRead(sensorPin);
  Serial.print("Current Sensor Value is  = ");
  Serial.println(sensorPin);

  if (sensorVal > 600)
  {
    lightState["on"] = true;
    lightState["hue"] = hueOver;
  }

  else
  {
    lightState["on"] = true;
    lightState["hue"] = hueUnder;
  }

  sendRequest(1, lightState);
  delay(1000);
}


void sendRequest(int light, JSONVar myState) {

  // make a String for the HTTP request path:
  String request = "/api/" + String(API_KEY);
  request += "/lights/";
  request += light;
  request += "/state/";

  String contentType = "application/json";

  // make a string for the JSON command:
  String body  = JSON.stringify(lightState);

  // see what you assembled to send:
  Serial.print("PUT request to server: ");
  Serial.println(request);
  Serial.print("JSON command to server: ");
  Serial.println(body);

  // make the PUT request to the hub:
  httpClient.put(request, contentType, body);

  // read the status code and body of the response
  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("Server response: ");
  Serial.println(response);
  Serial.println();
}