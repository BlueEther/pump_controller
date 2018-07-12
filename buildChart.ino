void buildWebChart() {
  buildJavascriptChart();
  webChart = "<!DOCTYPE HTML>\n";
  webChart += "<META name='viewport' content='width=device-width, initial-scale=1'>\n";
  webChart += "<meta http-equiv='refresh' content='60'>\n";
  webChart += "  <head>\n";
  //webChart +=  javaScriptChart;
  webChart += "    <!--Load the AJAX API-->\n";
  webChart += "    <script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>\n";
  webChart += "    <script type='text/javascript'>\n";

  webChart += "      // Load the Visualization API and the piechart package.\n";
  webChart += "      google.charts.load('current', {'packages':['corechart']});\n";

  webChart += "      // Set a callback to run when the Google Visualization API is loaded.\n";
  webChart += "      google.charts.setOnLoadCallback(drawChart);\n";

  webChart += "      // Callback that creates and populates a data table, \n";
  webChart += "      // instantiates the pie chart, passes in the data and\n";
  webChart += "      // draws it.\n";
  webChart += "      function drawChart() {\n";

  webChart += "      // Create the data table.\n";
  webChart += "      var data = new google.visualization.DataTable();\n";
  webChart += "      data.addColumn('string', 'Time');\n";
  webChart += "      data.addColumn('number', 'Shell Temp');\n";
  webChart += "      data.addColumn('number', 'Honey Temp');\n";
  webChart += "      data.addRows([\n";
//  webChart += "        ['9', 23],\n";
//  webChart += "        ['0', 15]\n";
//  webChart += "      ]);\n";
  for(int i=0;i<historyCount-1;i++){
    webChart += "        ['" + (String)(i-historyCount) + "'," + histShell[i] + "," + histHoney[i] + "],\n";
  }
  webChart += "        ['" + (String)0 + "'," + histShell[historyCount-1]+ "," + histHoney[historyCount-1] + "]\n";
  webChart += "      ]);\n";
   
  webChart += "      // Set chart options\n";
  webChart += "      var options = {'title':'Temps of Heat Exchanger',\n";
  webChart += "                     'width':800,\n";
  webChart += "                     'height':300};\n";

  webChart += "      // Instantiate and draw our chart, passing in some options.\n";
  webChart += "      var chart = new google.visualization.LineChart(document.getElementById('chart_div'));\n";
  webChart += "      chart.draw(data, options);\n";
  webChart += "    }\n";
  webChart += "    </script>\n";
  webChart += "  </head>\n";

  webChart += "  <body onload='processChart()'>\n";
  webChart += "  <A HREF='/' >Grundfos / Heat Exchanger Controler</A>\n";
  webChart += "<!--Div that will hold the pie chart-->\n";
  webChart += "    <div id='chart_div' style='width:400; height:300'></div>\n";
  webChart += "  </body>\n";
  webChart += "</html>\n";


  
}

void build_H_XML() {
  hXML = "<?xml version='1.0'?>";
  hXML += "<xml>";
  for (int i = 0; i < historyCount; i++) {
    hXML += "<historyShell" + (String)i + ">";
    hXML += (String)histShell[i];
    hXML += "</historyShell" + (String)i + ">";  
    
    hXML += "<historyHoney" + (String)i + ">";
    hXML += (String)histHoney[i];
    hXML += "</historyHoney" + (String)i + ">";  
  }      
  hXML += "</xml>";
}
