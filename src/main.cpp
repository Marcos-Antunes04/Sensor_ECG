#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "ANTUNES32"; // Insira aqui o nome da sua rede Wi-Fi
const char* password = "21040MCANT"; // Insira aqui a senha da sua rede Wi-Fi

const int pinSensor = A0;

AsyncWebServer server(80);

const char* html = R"rawliteral(
  <html>
    <head>
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css">
      <script src='https://cdn.plot.ly/plotly-latest.min.js'></script>
    
    <style>
    html {
      font-family: Arial;
      display: inline-block;
      margin: 0px auto;
      text-align: center;
    }
    h1 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels {
      font-size: 1.5rem;
      vertical-align: middle;
      padding-bottom: 15px;
    }
  </style>
    
    </head>
    <body>
      <div >
      <h1>ESP32 ECG</h1>
      <p>
      <i class="fas fa-heartbeat heartbeat-icon" style="color:red"></i>
      </p>
      </div>
      <div id='chart'></div>
      <script>
        var xValues = [];
        var yValues = [];
        var startTime = new Date().getTime();

        function updateGraph() {
          fetch('/data')
            .then(response => response.json())
            .then(data => {
              var currentTime = new Date().getTime() - startTime;
              xValues.push(currentTime);
              yValues.push(data.value);

              // Manter apenas os últimos 10 segundos de dados
              var removeIndex = 0;
              for (var i = 0; i < xValues.length; i++) {
                if (xValues[i] < (currentTime - 10000)) {
                  removeIndex = i;
                } else {
                  break;
                }
              }
              if (removeIndex > 0) {
                xValues.splice(0, removeIndex);
                yValues.splice(0, removeIndex);
              }

              Plotly.newPlot('chart', [{x: xValues, y: yValues}], {title: 'Leitura Eletrocardiograma' , line:{color:'red'}});
              Plotly.restyle(chart, {line: {color: 'red'}});
            });
        }

        var graphUpdate = setInterval(updateGraph, 100);
      </script>
    </body>
  </html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  
  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  // Configuração do servidor
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", html);
  });
  
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    int sensorValue = analogRead(pinSensor);
    String json = "{\"value\":" + String(sensorValue) + "}";
    request->send(200, "application/json", json);
  });
  
  server.begin();
}

void loop() {
  // Não é necessário código no loop para o servidor funcionar
}
