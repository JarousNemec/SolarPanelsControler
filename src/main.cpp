#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <main.h>
#include <Arduino.h>

void ConnectToWifi() {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected");

}

String ParametersString(Parameter parameters[], unsigned int size) {
    String output = "";
    for (unsigned int i = 0; i < size; ++i) {
        output += parameters[i].tag;
        output += "=";
        output += parameters[i].value;
        if (i + 1 < size) {
            output += "&";
        }
    }
    return output;
}

IRAM_ATTR void my_isr() {
    counter++;
}

float readTemp() {
    VRT = analogRead(A0);              //Acquisition analog value of VRT
    VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
    VR = VCC - VRT;
    RT = VRT / (VR / R);               //Resistance of RT

    ln = log(RT / RT0);
    tx = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor

    tx = tx - 273.15f;                 //Conversion to Celsius

    Serial.print(tx);
    Serial.println(" C");

    return tx;
}


void onTimerWifi() {
    if (WiFi.status() == WL_CONNECTED) {
        wind.value = String(mps);

        t = readTemp();

        if (isnan(t)) {
            temp.value = String(0);
        } else {
            temp.value = String(t);
        }

        Parameter parameters[] = {wind, temp};

        Serial.print("URL: ");
        String URL = url + ParametersString(parameters, 2);
        Serial.println(URL);
        http.begin(wiFiClient, URL.c_str());
        int httpCode = http.GET();
        if (httpCode > 0) {
            String payload = http.getString();
            if (payload == "True") {
                Serial.println(" - panels: active");
                digitalWrite(solarPin, LOW);
            } else if (payload == "False") {
                Serial.println(" - panels: blocked");
                digitalWrite(solarPin, HIGH);
            }
        } else {
            Serial.print("Error code: ");
            Serial.println(httpCode);
        }
        http.end();
    } else {
        Serial.println("---------------------");
        Serial.println("Connection lost!");
        ConnectToWifi();
    }
}

void onTimerValues() {
    timer++;
    //mps = counter / 3;
    //mps = 1.9*mps;
    mps = counter * 0.3;
    Serial.print(mps);
    Serial.println(" m/s");
    Serial.print((mps / 1000) * 3600);
    Serial.println(" km/h");
    counter = 0;
}


void setup() {
    wind.tag = "w";
    temp.tag = "t";

    Serial.begin(9600);
    ConnectToWifi();
    pinMode(solarPin, OUTPUT);

    pinMode(anemo_pin, INPUT_PULLUP);
    attachInterrupt(anemo_pin, my_isr, FALLING);
    T0 = 67 + 273.15;
    timer = millis();
}

int timer2 = 0;

void loop() {
    if (millis() - timer >= 1000) {
        onTimerValues();
        if (timer2 >= 20) {
            onTimerWifi();
            timer2 = 0;
        }
        //Serial.println(WiFi.RSSI());
        timer = millis();
        timer2++;
    }
}


//#include "ESP8266WiFi.h"
//
//void setup()
//{
//    Serial.begin(115200);
//    Serial.println();
//
//    WiFi.mode(WIFI_STA);
//    WiFi.disconnect();
//    delay(100);
//}
//
//void loop()
//{
//    Serial.print("Scan start ... ");
//    int n = WiFi.scanNetworks();
//    Serial.print(n);
//    Serial.println(" network(s) found");
//    for (int i = 0; i < n; i++)
//    {
//        Serial.println(WiFi.SSID(i));
//        Serial.println(WiFi.RSSI());
//    }
//    Serial.println();
//
//    delay(1000);
//}