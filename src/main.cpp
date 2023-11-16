#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "../lib/index.h"

const char* ssid = "ANTUNES32"; // Insira aqui o nome da sua rede Wi-Fi
const char* password = "21040MCANT"; // Insira aqui a senha da sua rede Wi-Fi

const int pinSensor = A0;

AsyncWebServer server(80);



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
