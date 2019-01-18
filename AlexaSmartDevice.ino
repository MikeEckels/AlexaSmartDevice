
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

fauxmoESP server;

#define WIFI_SSID          "SSID"
#define WIFI_PASS          "password"

#define DEVICE_ONE          2
#define ID_ONE             "light one"

int stateOne = 0;
int valueOne = 0;
boolean flagOne = false;

void wifiSetup() {

    // Set ESP as a node to connect to wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {

    wifiSetup();
    Serial.begin(115200);

    pinMode(DEVICE_ONE, OUTPUT);
    
    digitalWrite(DEVICE_ONE, LOW);

    server.setPort(80); // This is required for gen3 devices
    server.enable(true);

    // Add virtual devices
    server.addDevice(ID_ONE);

    server.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        
        if (strcmp(device_name, ID_ONE)==0) {
            stateOne = state;
            valueOne = value;
            flagOne = true;
        }
    });
}

void loop() {
    server.handle();
    
    if (flagOne == true)
    {
      flagOne = false;
      digitalWrite(DEVICE_ONE, stateOne);
      delay(500);
      digitalWrite(DEVICE_ONE, !stateOne);
      //digitalWrite(DEVICE_ONE, state ? LOW : HIGH);
    }
    // server.setState(ID_ONE, true, 255); //Use if state updated outside of callback (not by alexa)
}
