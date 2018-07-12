

String millis2time() {
  String Time = "";
  unsigned long ss;
  byte mm, hh;
  ss = millis() / 1000;
  hh = ss / 3600;
  mm = (ss - hh * 3600) / 60;
  ss = (ss - hh * 3600) - mm * 60;
  if (hh < 10)Time += "0";
  Time += (String)hh + ":";
  if (mm < 10)Time += "0";
  Time += (String)mm + ":";
  if (ss < 10)Time += "0";
  Time += (String)ss;
  return Time;
}

void handleWebsite() {
  buildWebsite();
  server.send(200, "text/html", webSite);
}

void handleXML() {
  buildXML();
  server.send(200, "text/xml", XML);
}

void handleESPval() {
//  int sliderCNT = server.arg("cnt").toInt();
//  sliderVal[sliderCNT] = server.arg("val").toInt();
  sliderVal[0] = server.arg("val0").toInt();
  sliderVal[1] = server.arg("val1").toInt();
  buildXML();
  server.send(200, "text/xml", XML);
  LowTemp = ESPval[0];
  HighTemp = ESPval[1];
    Serial.println(sliderVal[0]);
    Serial.println(sliderVal[1]);
}

void handleSaveTemps() {
  saveTemps();
  buildXML();
  
  server.send(200, "text/xml", XML);
}

void handleChart() {
  buildWebChart();
  server.send(200, "text/html", webChart);
  build_H_XML();
  server.send(200, "text/xml", hXML);
}

void handleESPbstate() {
  PumpState = server.arg("val").toInt();
  buildXML();
  server.send(200, "text/xml", XML);
  //Flush Heat Exchanger with quick flush
  if (PumpState == 2){                
    quickFlush();
  }
//Flush Heat Exchanger with full flush
  else if (PumpState == 3){         
    fullFlush();
  }
  else {
    HotFlushTimer = millis()-1;
  }
}

void clearErrors(void) {
  if (PumpState > 3){
    PumpState = 0;
    HotFlushTimer = 0;
    //turn led to normal
    leds[0] = RunColour; 
    FastLED.show(); 
    Serial.println(F("Pump off"));
  }
  return;
}

void errorFlash(void) {
  HotFlushTimer = 0;
  digitalWrite(RELAYPin, LOW);    //turn pump off 
  if(PumpState == 6){
    leds[0] = CRGB::Red; 
    FastLED.show();
    PumpState = 7;
  } else {
    leds[0] = CRGB::Blue; 
    FastLED.show();
    PumpState = 6;
  }
  delay(1000);
  sensors.requestTemperatures();    //test ds18b20 again
  tmpOW_shell = sensors.getTempC(addr0);
  tmpOW_honey = sensors.getTempC(addr1);
  Serial.println(tmpOW_shell); 
  Serial.println(tmpOW_honey); 
  return;
}

void flashTimer(void){
  if ((HotFlushTimer > 0) && (millis() > HotFlushTimer)){
    digitalWrite(RELAYPin, LOW);    //turn pump off 
    PumpState = 0;
    HotFlushTimer = 0;
    //turn led to normal
    leds[0] = RunColour; 
    FastLED.show(); 
    Serial.println(F("Pump off"));
  }
  return;
}

void quickFlush(void){
  HotFlushTimer = millis() + QuickFlushTime * 1000;
  digitalWrite(RELAYPin, HIGH);     //turn pump on
  PumpState = 2;                    //PumpState is 2 for quick flush
  
  leds[0] = QuckColour;             //turn led on for quick flush
  FastLED.show(); 
  Serial.println(F("Pump on QUICK"));
  return;
}

void fullFlush(void){
  HotFlushTimer = millis() + FullFlushTime * 1000;
  digitalWrite(RELAYPin, HIGH);     //turn pump on
  PumpState = 3;                    //PumpState is 3 for full flush
  
  leds[0] = FullColour;             //turn led on for full flush
  FastLED.show(); 
  Serial.println(F("Pump on FULL"));
  return;
}

void processTemp(void){
  //if temp is below low set turn pump on
  if ((PumpState == 0) &&(tmpOW_shell < LowTemp)) {
    digitalWrite(RELAYPin, HIGH);   //turn pump on
    PumpState = 1;
    Serial.println(F("Pump on"));
    leds[0] = PumpColour; 
    FastLED.show(); 
  }
  //if temp is above high set turn pump off
  if ((PumpState == 1) &&(tmpOW_shell >= HighTemp)){
    digitalWrite(RELAYPin, LOW);    //turn pump off 
    PumpState = 0;
    Serial.println(F("Pump off"));
    leds[0] = RunColour; 
    FastLED.show(); 
  }
  //turn pump LED back to normal or pumping
  if (PumpState == 0){
    leds[0] = RunColour;  //normal
    FastLED.show();      
  }    
  else if (PumpState == 1){
    leds[0] = PumpColour; //pumping
    FastLED.show();      
  } 
  return;
}

void readTemp(void){   
  digitalWrite(LEDPin, LOW);        //internal LED used for testing
  
  leds[0] = ReadColour;             //turn LED on for temp read
  FastLED.show(); 
  
  Serial.println(F("Read Temp"));
  sensors.requestTemperatures(); 
  //tmpOW_shell = sensors.getTempC(addr0);
  tmpOW_honey = sensors.getTempC(addr1);
  tmpOW_shell = (PumpState==1)? tmpOW_shell+1: tmpOW_shell-1; //for testing without sensor connected
  if (tmpOW_honey == -127 ) tmpOW_honey=0;
  
  Serial.println(F("Digital temp: "));
  Serial.print(F("Water = "));
  Serial.print(tmpOW_shell);
  Serial.println(F("c."));
  Serial.print(F("Honey = "));
  Serial.print(tmpOW_honey);
  Serial.println(F("c."));
  Serial << "Set Temp. for on: " << (String)LowTemp << endl;
  Serial << "Set Temp. for off: " << (String)HighTemp << endl;
//copy history array left, add new value to right
  memcpy(histShell, &histShell[1], sizeof(histShell) - sizeof(int));
  histShell[359] = tmpOW_shell;  

  memcpy(histHoney, &histHoney[1], sizeof(histHoney) - sizeof(int));
  histHoney[359] = tmpOW_honey; 
  
  digitalWrite(LEDPin, HIGH);   //turn internal led off  
  //turn pump LED back to normal or pumping
  if (PumpState == 0){
    leds[0] = RunColour;  //normal
    FastLED.show();      
  }    
  else if (PumpState == 1){
    leds[0] = PumpColour; //pumping
    FastLED.show();      
  } 
  //restart timer
  mills= millis();
  return;
}

