#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <main.h>

HTTPClient http;
WiFiClient wiFiClient;

void setup() {
    Serial.begin(115200);
    ConnectToWifi();
    pinMode(solarPin, OUTPUT);
}

void ConnectToWifi() {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {

        delay(1000);
        Serial.print(".");

    }
    Serial.println();
    Serial.println("Connected");
    Serial.println("---------------------");
    http.begin(wiFiClient, url);
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        int httpCode = http.GET();

        if (httpCode > 0) {
            String payload = http.getString();
            String preparedString = PrepateToParseJson(payload);
            String state = GetKeyValue(preparedString, "state");
            if (state == "true") {
                Serial.println(" - panels: true");
                digitalWrite(solarPin, HIGH);
            } else if (state == "false") {
                Serial.println(" - panels: false");
                digitalWrite(solarPin, LOW);
            }
        }
    }
    else{
        Serial.println("---------------------");
        Serial.println("Connection lost!");
        ConnectToWifi();
    }
    Serial.println("---------------------");
    delay(5000);
}

String PrepateToParseJson(String json) {
    Serial.println(" - Preparing json to parse");
    json.replace("\"", "");
    json.remove(0, 2);
    json.remove(json.length() - 2, 2);
    json.replace(":", ",");
    json += ",";
    Serial.println(" - Prepared");
    return json;
}

String GetKeyValue(String data, const String &findingKey) {
    Serial.println(" - Parsing value from json");
    String temp = "";
    bool found = false;

    for (char &j : data) {
        if (j == ',') {
            if (found) {
                Serial.println(" - Value parsed");
                return temp;
            }
            if (temp == findingKey) {
                found = true;
            }
            temp = "";
        } else {
            temp += j;
        }
    }
    Serial.println(" - Cant parse founded value");
    return "";
}