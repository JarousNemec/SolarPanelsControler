#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <main.h>

HTTPClient http;  //Declare an object of class HTTPClient
WiFiClient wiFiClient;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {

        delay(1000);
        Serial.print(".");

    }
    http.begin(wiFiClient, "http://192.168.1.58:8081/spstate");
    pinMode(solarPin,OUTPUT);
}

void loop() {

    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

        //Specify request destination
        int httpCode = http.GET();                                  //Send the request

        if (httpCode > 0) { //Check the returning code

            String payload = http.getString();   //Get the request response payload
            payload = PrepateToParseJson(payload);
            String state = GetValueFromString(payload, "state");
            if (state == "true") {
                Serial.println("panels: true");
                digitalWrite(solarPin, HIGH);
            } else if (state == "false") {
                Serial.println("panels: false");
                digitalWrite(solarPin, LOW);
            }
        }
    }

    delay(5000);    //Send a request every 5 seconds
}

String PrepateToParseJson(String json) {
    json.replace("\"", "");
    json.remove(0, 2);
    json.remove(json.length() - 2, 2);
    json.replace(":", ",");
    json += ",";
    return json;
}

String GetValueFromString(String data, const String& findingKey) {
    String temp = "";
    bool found = false;

    for (char & j : data) {
        if (j == ',') {
            if (found) {
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

    return "";
}