#include <Arduino.h>
#include <FastLED.h>
#include <OneWire.h>
#include <dht.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <Streaming.h>                  // http://arduiniana.org/libraries/streaming/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "./javascript.h"

/***********************************************************************/
//Settings for controller
/***********************************************************************/
int LowTemp      = 45;        //Min temp for heat exchanger
int HighTemp     = 48;        //Max temp for heat exchanger 
int QuickFlushTime  = 3 * 60;   //3 min flush
int FullFlushTime   = 10 * 60;  //10 min flush
int ReadTime = 5;               //10 second between reads of heat exchanger
//Set RGB colourd for modes
CRGB QuckColour = CRGB::Blue;   //Quick flush
CRGB FullColour = CRGB::Purple; //Full flush
CRGB ReadColour = CRGB::Red;    //data read
CRGB RunColour = CRGB::Green;   //normal running
CRGB PumpColour = CRGB::Orange; //pump running in normal mode
CRGB CurColour = RunColour;

//WiFI
const char* ssid     = "BlueEther";       // ERASE prior to publishing
const char* password = "iwillnotgiveyoumykey";    //   "" ditto
const char* APssid     = "Grundfos_Controler";       
const char* APpassword = "123456789";
IPAddress IP(192,168,1,10);
IPAddress gw(192,168,1,1);
IPAddress sub(255,255,255,0);
ESP8266WebServer server(80);
IPAddress lIP;
IPAddress aIP;


//Define PINs
const int LEDPin    = D4;   //internal LED for testing
const int RELAYPin  = D5;   //Pin for relay connected to grundfos pump
const int FlushQPin = D1;   //input pin for quick flush switch
const int FlushFPin = D2;   //input pin for full flush switch
const int DATA_PIN  = D7;   //data pin for RGB LED
const int OWPin     = D6;   //data pin for ds18b20-temp-sensor

//RGB LED SETUP
#define NUM_LEDS 1          //Only one LED on board
#define BRIGHTNESS  64      //brightness of RGB LED
#define LED_TYPE    WS2812B //Type of RGB LED
#define COLOR_ORDER GRB     //colour order
CRGB leds[NUM_LEDS];        //create LED array
#define UPDATES_PER_SECOND 100

//Dallas One Wire ds18b20-temp-sensor
OneWire  ds(OWPin);  
DallasTemperature sensors(&ds);
DeviceAddress addr0 = {0x28, 0xFF, 0xE0, 0xA5, 0x85, 0x16, 0x04, 0xCA};
DeviceAddress addr1 = {0x28, 0xFF, 0x07, 0xD0, 0x85, 0x16, 0x04, 0x68};

//General vars
bool HotFlush       = false;      //is hot flush active
unsigned long HotFlushTimer = 0;  //hot flush timer
byte PumpState      = 0;          //0=off 1=run 2=quick 3=full 6/7=read sensor error
float  tmpOW_shell       = 36;          //ds18b20 
float  tmpOW_honey       = 36;          //ds18b20 temp
int SavedHigh, SavedLow = 0;            //The temps that are written to eeprom

int histShell[historyCount];               //30 min of history for graph
int histHoney[historyCount];

unsigned long mills = 0;          //timer for read loop
unsigned long wait000 = 0UL, wait001 = 1000UL;


void setup() {
  pinMode(RELAYPin, OUTPUT);        //setup relay 
  digitalWrite(RELAYPin, LOW);      //pull relay low - off
  pinMode(OWPin, INPUT_PULLUP);     //setup ds18b20 pin
  pinMode(FlushQPin, INPUT_PULLUP); //setup switch for flush
  pinMode(FlushFPin, INPUT_PULLUP); //setup switch for flush
  Serial.begin(115200);             //setup soft serial for debuging
  delay(500);                       //wait for 1/10 of a sec for serial to come up
  
  loadTemps();                      //read stored temps from memory
  
  //setup RGB LED
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  
  digitalWrite(LEDPin, HIGH);       //turn internal LED off
  
  sensors.setResolution(addr0, 12); //set ds18b20 to 12 data bits (0.06 deg c)
  sensors.requestTemperatures();    //test ds18b20
  readTemp();
  Serial << (F("... Attempting to log into router... \r\n"));

  //WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  WiFi.softAPConfig (IP, gw, sub);
  WiFi.softAP(APssid, APpassword);
  //WiFi.mode(WIFI_AP_STA);
  Serial.print("AP IP address: ");
  lIP = WiFi.localIP();
  aIP = WiFi.softAPIP();
  Serial.println(aIP);
  
  Serial << (F("\n\nBOOTING ESP8266 ..."));
  Serial << (F("Connected to ")) << ssid << endl;
  Serial << (F("Station IP address: ")) << lIP << endl;

  server.on("/",              handleWebsite);
  server.on("/xml",           handleXML);
  server.on("/setESPval",     handleESPval);
  server.on("/setESPbstate",  handleESPbstate);
  server.on("/saveTemps",     handleSaveTemps);
  server.on("/chart",         handleChart);
  server.begin();
  
  Serial.println(F("Heat Exchanger Controller v1.1"));
  Serial.println(F("By Charles Fryett"));
  sliderVal[0]=(LowTemp-30)*3;
  sliderVal[1]=(HighTemp-30)*3;
}

void loop() {  
  server.handleClient();
  if (millis() > wait000) {
    buildXML();
    //build_H_XML();
    wait000 = millis() + 1000UL;
  }
  
//test if temp probe isn't erroring and flash red/blue is it is
  if((tmpOW_shell == -127) || (tmpOW_shell == 85)){
  Serial.println(F("error"));
    errorFlash();
    return;
  }
//if the temp probe has been in error but is better reset everything
  clearErrors();
  
//timer loop for flush
  flashTimer();
  
//Flush Heat Exchanger with quick flush
  if ((PumpState != 2) && (digitalRead(FlushQPin) == LOW)){                
    quickFlush();
  }
//Flush Heat Exchanger with full flush
  else if ((PumpState != 3) && (digitalRead(FlushFPin) == LOW)){         
    fullFlush();
  }
//read temp every <10> seconds  
  if (millis() - mills > ReadTime * 1000) {
    readTemp();
    if (HotFlushTimer == 0){
      processTemp();
    }
  }
}

