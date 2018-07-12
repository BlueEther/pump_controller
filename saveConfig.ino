void loadTemps() {
  EEPROM.begin(512);
  EEPROM.get(0, LowTemp);
  EEPROM.get(0+sizeof(LowTemp), HighTemp);
  char ok[2+1];
  EEPROM.get(0+sizeof(LowTemp)+sizeof(HighTemp), ok);
  EEPROM.end();
  if (String(ok) != String("OK")) {
    LowTemp = 45;
    HighTemp = 48;
  }
  SavedLow = LowTemp;
  SavedHigh = HighTemp;
  
  Serial.println();
  Serial.println("Recovered Temps:");
  Serial.print("Low Temp:");
  Serial.println(LowTemp);
  Serial.print("High Temp:");
  Serial.println(HighTemp);
}

/** Store WLAN credentials to EEPROM */
void saveTemps() {
  EEPROM.begin(512);
  EEPROM.put(0, LowTemp);
  EEPROM.put(0+sizeof(LowTemp), HighTemp);
  char ok[2+1] = "OK";
  EEPROM.put(0+sizeof(LowTemp)+sizeof(HighTemp), ok);
  EEPROM.commit();
  EEPROM.end();
  
  SavedLow = LowTemp;
  SavedHigh = HighTemp;
  
  Serial.println();
  Serial.println("Saved Temps:");
  Serial.print("Low Temp:");
  Serial.println(LowTemp);
  Serial.print("High Temp:");
  Serial.println(HighTemp);
}
