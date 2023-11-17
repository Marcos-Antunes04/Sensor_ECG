#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "../lib/index.h"

const char* ssid = "ANTUNES32";
const char* password = "21040MCANT";

const int pinSensor = A0;

AsyncWebServer server(80);

int ECG_Read(){
  if(digitalRead(40) == 1 || digitalRead(41) == 1){
    return 5000;
  }else{
    return analogRead(pinSensor);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(41, INPUT); // Setup for leads off detection LO +
  pinMode(40, INPUT); // Setup for leads off detection LO -

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", html);
  });
  
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    //int sensorValue = analogRead(pinSensor);
    String json = "{\"value\":" + String(ECG_Read()) + "}";
    request->send(200, "application/json", json);
  });
  
  server.begin();
}

void loop() {
}
