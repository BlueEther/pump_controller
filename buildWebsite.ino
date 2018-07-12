void buildWebsite() {
  buildJavascript();
  webSite = "<!DOCTYPE HTML>\n";
  webSite += "<META name='viewport' content='width=device-width, initial-scale=1'>\n";
  webSite += javaScript;
  webSite += "<BODY style='background-color:#00ff90;' onload='process()'>\n";
  webSite += "<A HREF='/chart' >Grundfos / Heat Exchanger Controler</A><BR>\n";
  webSite += "Local IP ";
  webSite += (String)APssid +  " / " + aIP[0] + "." + aIP[1] + "." + aIP[2] + "." + aIP[3];
  webSite += "<BR>\n";
  webSite += "Network IP ";
  webSite += (String)ssid +  " / " + lIP[0]+  "." + lIP[1]+  "." + lIP[2]+  "." + lIP[3];
  webSite += "<BR>\n";
  webSite += "Current Heat Exchanger shell temp: <b><A ID='shelltemp'></A></b> deg C.<BR>\n";
  webSite += "Current Heat Exchanger honey temp: <b><A ID='honeytemp'></A></b> deg C.<BR>\n";
  webSite += "Uptime = <A ID='runtime'></A><BR>\n";
  webSite += "<FORM action='/'>\n";
  webSite += "<TABLE BORDER=1 style='text-align:center;border-collapse:collapse;background-color:#0090ff;' width='90%' align='center'>\n";
  for (int i = 0; i < sliderMAX; i++) {
    webSite += "<TR>\n";
    webSite += "<TD><BR><INPUT ID='slider" + (String)i + "' TYPE='range' ONCHANGE='Slider(" + (String)i + ")' style='width: 90%'></TD>\n"; //in Firefox, Chrome and Edge use ONINPUT
    if (i == 0){
      //webSite += "<TD>Slidervalue for off: <A ID='Sliderval" + (String)i + "'></A><BR>\n";
      webSite += "<TD>\n";
      webSite += "Set <b>ON</b> Temp: <br><A ID='ESPval" + (String)i + "'></A> degrees</TD>\n";
    }
    if (i == 1){
      //webSite += "<TD>Slidervalue for on: <A ID='Sliderval" + (String)i + "'></A><BR>\n";
      webSite += "<TD>\n";
      webSite += "Set <b>OFF</b> Temp: <br><A ID='ESPval" + (String)i + "'></A> degrees</TD>\n";
    }
    webSite += "</TR>\n";
  }
  webSite += "<TR><TD>\n";
  webSite += "<INPUT ID='save' TYPE='button' NAME='save' ONCLICK='Save()' style='width:120px; height:40px;' VALUE='Save'>";
  webSite += "</TD>\n";
  webSite += "<TD>\n";
  webSite += "Saved Temps: \n";
      webSite += "<br>Low: <B><A ID='SavedLow'></A></B> degrees\n";
      webSite += "<br>High: <B><A ID='SavedHigh'></A></B> degrees\n";
  webSite += "</TD></TR>\n";
    ///////////////////////
  for (int i = 0; i < radioMAX; i++) {
    webSite += "<TR>\n";
    webSite += "<TD ID='radioB" + (String)i + "'><BR><INPUT ID='radio" + (String)i + "' TYPE='radio' NAME='state' ONCHANGE='Radio()' style='width:40px; height:40px;' "; //in Firefox, Chrome and Edge use ONINPUT
    if (i == 0){
      webSite += " value='0'></TD>\n<TD>Running:<BR>\n";
      webSite += "Normal: Pump off</TD>\n";
    }
    if (i == 1){
      webSite += " value='1'></TD>\n<TD>Running:<BR>\n";
      webSite += "Normal: Pump on</TD>\n";
    }
    if (i == 2){
      webSite += " value='2'></TD>\n<TD>Running:<BR>\n";
      webSite += "Quick Flush.</TD>\n";
    }
    if (i == 3){
      webSite += " value='3'></TD>\n<TD>Running:<BR>\n";
      webSite += "Full Flush.</TD>\n";
    }

  }    
  webSite += "<TR>\n";
  webSite += "<TD>\n";
  webSite += "</TD>\n";
  webSite += "</TR>\n";
  webSite += "</TABLE>\n";
  webSite += "</FORM>\n";
  webSite += "</BODY>\n";
  webSite += "</HTML>\n";
}

void buildXML() {
  XML = "<?xml version='1.0'?>";
  XML += "<xml>";
  XML += "<millistime>";
  XML += millis2time();
  XML += "</millistime>";
  for (int i = 0; i < sliderMAX; i++) {
    XML += "<sliderval" + (String)i + ">";
    XML += String(sliderVal[i]);
    XML += "</sliderval" + (String)i + ">";
    XML += "<ESPval" + (String)i + ">";
    ESPval[i] = 30+sliderVal[i]/3;
    XML += String(ESPval[i]);
    XML += "</ESPval" + (String)i + ">";
  }
  
    XML += "<state>";
    XML += String(PumpState);
    XML += "</state>";
      
    XML += "<savedLow>";
    XML += String(SavedLow);
    XML += "</savedLow>";
      
    XML += "<savedHigh>";
    XML += String(SavedHigh);
    XML += "</savedHigh>";
    
    XML += "<shelltemp>";
    XML += String(tmpOW_shell);
    XML += "</shelltemp>";
    XML += "<honeytemp>";
    XML += String(tmpOW_honey);
    XML += "</honeytemp>";
        
  XML += "</xml>";
}

