String webSite, javaScript, XML, hXML, webChart, javaScriptChart;
const int sliderMAX = 2;        // This sets the number of sliders you want
const int radioMAX = 4;        // This sets the number of buttons you want
int sliderVal[sliderMAX] = {60}, ESPval[sliderMAX];
int radioState[radioMAX] = {0};

const int historyCount = 360;

void buildJavascript() {
  javaScript = "<SCRIPT>\n";
  javaScript += "xmlHttp=createXmlHttpObject();\n";

  javaScript += "function createXmlHttpObject(){\n";
  javaScript += "  if(window.XMLHttpRequest){\n";
  javaScript += "    xmlHttp=new XMLHttpRequest();\n";
  javaScript += "  }else{\n";
  javaScript += "    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  javaScript += "  }\n";
  javaScript += "  return xmlHttp;\n";
  javaScript += "}\n";

  javaScript += "function process(){\n";
  javaScript += "  if(xmlHttp.readyState==0||xmlHttp.readyState==4){\n";
  javaScript += "    xmlHttp.onreadystatechange=function(){\n";
  javaScript += "      if(xmlHttp.readyState==4&&xmlHttp.status==200){\n";
  javaScript += "        xmlDoc=xmlHttp.responseXML;\n";
  javaScript += "        xmlmsg=xmlDoc.getElementsByTagName('millistime')[0].firstChild.nodeValue;\n";
  javaScript += "        document.getElementById('runtime').innerHTML=xmlmsg;\n";
  javaScript += "        for(i=0;i<" + (String)sliderMAX + ";i++){\n";
  javaScript += "          xmlmsg=xmlDoc.getElementsByTagName('sliderval'+i)[0].firstChild.nodeValue;\n";
  javaScript += "          document.getElementById('slider'+i).value=xmlmsg;\n";
  //javaScript += "          document.getElementById('Sliderval'+i).innerHTML=xmlmsg;\n";
  javaScript += "          xmlmsg=xmlDoc.getElementsByTagName('ESPval'+i)[0].firstChild.nodeValue;\n";
  javaScript += "          document.getElementById('ESPval'+i).innerHTML=xmlmsg;\n";
  javaScript += "        }\n";
  ///////////////////////////////////////////
  javaScript += "        xmlmsg=xmlDoc.getElementsByTagName('state')[0].firstChild.nodeValue;\n";
  javaScript += "        for(i=0;i<" + (String)radioMAX + ";i++){\n";
  javaScript += "            document.getElementById('radioB' + i).style.backgroundColor='#0090ff';\n";
  javaScript += "          if(parseInt(xmlmsg) == i){\n";
  javaScript += "            document.getElementById('radio'+i).checked=true;\n";
  javaScript += "          }\n";
  javaScript += "        }\n";
  javaScript += "        document.getElementById('radioB' + xmlmsg).style.backgroundColor='#ffaaaa';\n";
  javaScript += "        document.getElementById('radio1').disabled=true;\n";
  //////////////////////////////////////////
  
  javaScript += "        xmlmsg=xmlDoc.getElementsByTagName('shelltemp')[0].firstChild.nodeValue;\n";
  javaScript += "        document.getElementById('shelltemp').innerHTML=xmlmsg;\n";
  
  javaScript += "        xmlmsg=xmlDoc.getElementsByTagName('honeytemp')[0].firstChild.nodeValue;\n";
  javaScript += "        document.getElementById('honeytemp').innerHTML=xmlmsg;\n";
  
  javaScript += "        xmlmsg=xmlDoc.getElementsByTagName('savedLow')[0].firstChild.nodeValue;\n";
  javaScript += "        document.getElementById('SavedLow').innerHTML=xmlmsg;\n";
  
  javaScript += "        xmlmsg=xmlDoc.getElementsByTagName('savedHigh')[0].firstChild.nodeValue;\n";
  javaScript += "        document.getElementById('SavedHigh').innerHTML=xmlmsg;\n";
  
  javaScript += "      }\n";
  javaScript += "    }\n";
  javaScript += "    xmlHttp.open('PUT','xml',true);\n";
  javaScript += "    xmlHttp.send(null);\n";
  javaScript += "  }\n";
  javaScript += "  setTimeout('process()',1000);\n";
  
  javaScript += "}\n";
////////////////////////////////////////END PROCESS////////////////////////


  javaScript += "function Slider(cnt){\n";
  javaScript += "   if(parseInt(cnt)==0){ ;\n";
  javaScript += "     sliderVal0=document.getElementById('slider0').value;\n";
  javaScript += "     sliderVal1=document.getElementById('slider1').value;\n";
  javaScript += "     if(parseInt(sliderVal0)+6>parseInt(sliderVal1)){;\n";
  //javaScript += "       document.getElementById('Sliderval0').innerHTML=sliderVal0;\n";
  javaScript += "       document.getElementById('ESPval0').innerHTML=parseInt(30+sliderVal0/3);\n";
  javaScript += "       sliderVal1= +sliderVal0 + 6;\n";
  //javaScript += "       document.getElementById('Sliderval1').innerHTML=sliderVal1;\n";
  javaScript += "       document.getElementById('ESPval1').innerHTML=parseInt(30+sliderVal1/3);\n";
  javaScript += "     }\n";
  javaScript += "   }\n";
  javaScript += "   if(parseInt(cnt)==1){ ;\n";
  javaScript += "     sliderVal0=document.getElementById('slider0').value;\n";
  javaScript += "     sliderVal1=document.getElementById('slider1').value;\n";
  javaScript += "     if(parseInt(sliderVal1)<parseInt(sliderVal0)+6){;\n";
  //javaScript += "       document.getElementById('Sliderval1').innerHTML=sliderVal1;\n";
  javaScript += "       document.getElementById('ESPval1').innerHTML=parseInt(30+sliderVal1/3);\n";
  javaScript += "       sliderVal0= +sliderVal1 - 6;\n";
  //javaScript += "       document.getElementById('Sliderval0').innerHTML=sliderVal0;\n";
  javaScript += "       document.getElementById('ESPval0').innerHTML=parseInt(30+sliderVal0/3);\n";
  javaScript += "     }\n";
  javaScript += "   }\n";

  javaScript += "       if(xmlHttp.readyState==0||xmlHttp.readyState==4){\n";
  javaScript += "         xmlHttp.open('PUT','setESPval?val0='+sliderVal0+'&val1='+sliderVal1,true);\n";
  javaScript += "         xmlHttp.send(null);\n";
  javaScript += "       }\n";
  javaScript += "       }\n";
////////////////////////////////////////////////////
  javaScript += "function Radio(){\n";
  javaScript += "var state = document.forms[0];\n";
  javaScript += "  var txt = '';\n";
  javaScript += "  var i;\n";
  javaScript += "  for (i = 0; i < state.length; i++) {\n";
  javaScript += "      if (state[i].checked) {\n";
  javaScript += "          txt = state[i].value;\n";
  javaScript += "      }\n";
  javaScript += "  }\n";
  javaScript += "  if(xmlHttp.readyState==0||xmlHttp.readyState==4){\n";
  javaScript += "    xmlHttp.open('PUT','setESPbstate?val='+txt,true);\n";
  javaScript += "    xmlHttp.send(null);\n";
  javaScript += "  }\n";

  javaScript += "}\n";
//////////////////////////////////////////////////

  javaScript += "function Save(){\n";
  javaScript += "  if(xmlHttp.readyState==0||xmlHttp.readyState==4){\n";
  javaScript += "    xmlHttp.open('PUT','saveTemps',true);\n";
  javaScript += "    xmlHttp.send(null);\n";
  javaScript += "  }\n";
  javaScript += "}\n";
  javaScript += "</SCRIPT>\n";
}

void buildJavascriptChart() {
  javaScriptChart = "<SCRIPT>\n";
  javaScriptChart += "xmlHttp=createXmlHttpObject();\n";

  javaScriptChart += "function createXmlHttpObject(){\n";
  javaScriptChart += "  if(window.XMLHttpRequest){\n";
  javaScriptChart += "    xmlHttp=new XMLHttpRequest();\n";
  javaScriptChart += "  }else{\n";
  javaScriptChart += "    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  javaScriptChart += "  }\n";
  javaScriptChart += "  return xmlHttp;\n";
  javaScriptChart += "}\n";

  javaScriptChart += "function processChart(){\n";
  javaScriptChart += "  if(xmlHttp.readyState==0||xmlHttp.readyState==4){\n";
  javaScriptChart += "    xmlHttp.onreadystatechange=function(){\n";
  javaScriptChart += "      if(xmlHttp.readyState==4&&xmlHttp.status==200){\n";
  javaScriptChart += "        xmlDoc=xmlHttp.responseXML;\n";
  javaScriptChart += "        for(i=0;i<" + (String)historyCount + ";i++){\n";
  javaScriptChart += "          xmlmsg=xmlDoc.getElementsByTagName('historyShell'+i)[0].firstChild.nodeValue;\n";
  javaScriptChart += "          data.addRow(['(" + (String)historyCount + " - i)*5', xmlmsg])               \n";
  javaScriptChart += "        }\n";
  javaScriptChart += "      }\n";
 
  javaScriptChart += "    }\n";
  javaScriptChart += "    xmlHttp.open('PUT','xml',true);\n";
  javaScriptChart += "    xmlHttp.send(null);\n";
  javaScriptChart += "  }\n";
  javaScriptChart += "  setTimeout('processChart()',5000);\n";
  
  javaScriptChart += "}\n";
  javaScriptChart += "</SCRIPT>\n";
}
////////////////////////////////////////END PROCESS////////////////////////



