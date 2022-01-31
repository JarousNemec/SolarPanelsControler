//#include <Arduino.h>
//
//#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
//
////void setup() {
////    Serial.begin(9600);
////    pinMode(2, OUTPUT);
////}
////
////int state = false;
////
////void loop() {
////    digitalWrite(2, state);
////    state = !state;
////    Serial.println("69");
////    delay(1000);
////}
//
//const char* ssid     = "ASUS";
//const char* password = "hesloheslo";
//
//const char* host = "mfapi.westeurope.azurecontainer.io";
//const char* streamId   = "....................";
//const char* privateKey = "....................";
//HTTPClient http;
//void setup() {
//    Serial.begin(115200);
//    delay(10);
//
//    // We start by connecting to a WiFi network
//
//    Serial.println();
//    Serial.println();
//    Serial.print("Connecting to ");
//    Serial.println(ssid);
//
//    WiFi.begin(ssid, password);
//
//    while (WiFi.status() != WL_CONNECTED) {
//        delay(500);
//        Serial.print(".");
//    }
//
//    Serial.println("");
//    Serial.println("WiFi connected");
//    Serial.println("IP address: ");
//    Serial.println(WiFi.localIP());
//      //Declare an object of class HTTPClient
//WiFiClient wiFiClient;
//    http.begin(wiFiClient,"http://mfapi.westeurope.azurecontainer.io:8081/allresults");
//}
//
//
//void loop() {
//    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
//
//         //Specify request destination
//        int httpCode = http.GET();                                  //Send the request
//
//        if (httpCode > 0) { //Check the returning code
//
//            String payload = http.getString();   //Get the request response payload
//            Serial.println(payload);             //Print the response payload
//
//        }
//
//        http.end();   //Close connection
//
//    }
//}

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

using namespace std;

const char *ssid = "ASUS";
const char *password = "hesloheslo";
const int solarPin = 2;
HTTPClient http;  //Declare an object of class HTTPClient
WiFiClient wiFiClient;

String PrepateToParseJson(String json);

String GetValueFromString(String data, const String& findingKey);


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

