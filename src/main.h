//
// Created by Strejda Jára on 31.01.2022.
//

#ifndef SOLARPANELSCONTROLER_MAIN_H
#define SOLARPANELSCONTROLER_MAIN_H
#define url "http://mfapi.westeurope.azurecontainer.io:8081/spstate"

const char *ssid = "ASUS";
const char *password = "hesloheslo";
const int solarPin = 2;

String PrepateToParseJson(String json);

String GetKeyValue(String data, const String& findingKey);

void ConnectToWifi();

#endif //SOLARPANELSCONTROLER_MAIN_H

