#include <ESPmDNS.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <vector>

AsyncWebServer server(80);

void initHandlers() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.js", "text/javascript");
  });

  server.on("/on.svg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/on.svg", "image/svg+xml");
  });

  server.on("/off.svg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/off.svg", "image/svg+xml");
  });

  server.on("/pulse.svg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/pulse.svg", "image/svg+xml");
  });

  server.on("/relays.conf", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/relays.conf", "text/plain");
  });

  server.on("/network_setup.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/network_setup.html", String(), false);
  });

  server.on("/network_setup.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/network_setup.js", "text/javascript");
  });
}

void connector() {
  File config = SPIFFS.open("/network.conf");
  bool isSetup = false;
  std::vector<String> prop;

  if (config) {
    while (config.available()) {
      prop.push_back(config.readStringUntil(','));
    }

    config.close();
    isSetup = (prop[0].compareTo("1") == 0);
  }

  if(isSetup) {

    char* ssid = (char*)prop[1].c_str();
    char* password = (char*)prop[2].c_str();
    char* mdnsAddr = (char*)prop[3].c_str();
    bool staticIP = (prop[4].compareTo("static") == 0);
    Serial.println(prop[0]);
    Serial.println(ssid);
    Serial.println(password);
    Serial.println(mdnsAddr);

    if(staticIP) {
      IPAddress IPAddr;
      IPAddr.fromString(prop[5]);
      IPAddress gateway;
      gateway.fromString(prop[6]);
      IPAddress mask;
      mask.fromString(prop[7]);
      WiFi.config(IPAddr, gateway, mask);
    }

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
    initHandlers();

    
    Serial.println(WiFi.localIP());

  } else {

    // IPAddress IPaddr(192,168,1,2);
    // IPAddress gateway(192,168,1,1);
    // IPAddress mask(255,255,255,0);

    WiFi.mode(WIFI_AP);
    // WiFi.softAPConfig(IPaddr, gateway, mask);
    WiFi.softAP("ESP32 8-relay board", "12345678");

    IPAddress IP = WiFi.softAPIP();
    Serial.println(IP);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/network_setup.html", String(), false);
    });

    server.on("/network_setup.js", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/network_setup.js", "text/javascript");
    });

    server.on("/", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL,
      [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
        Serial.print("Data: ");
        for (size_t i = 0; i < len; i++) {
          Serial.write(data[i]);
        }
        Serial.println();
        request->send(200);
    });
  }

  if(!MDNS.begin("esp32")) {
      Serial.println("Could not start mDNS");
    }
    MDNS.addService("http", "tcp", 80);
}



void setup() {

  Serial.begin(9600);

  // initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // initialize wifi connection
  connector();
  

  // define request handlers
  

  // initialoze handler for individual requests
  server.on("/action", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebParameter* parameter1 = request->getParam(0);
    AsyncWebParameter* parameter2 = request->getParam(1);
    String relayString = parameter1->value();
    String stateString = parameter2->value();
    //int relay = relayString.toInt();
    //int state = stateString.toInt();
    Serial.println(relayString);
    Serial.println(stateString);
    request->send(SPIFFS, "/index.html", String(), false);
  });

  // start server
  server.begin();
}

void loop() {

}