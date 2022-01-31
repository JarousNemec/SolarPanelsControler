//
// Created by Strejda Jára on 31.01.2022.
//

#ifndef SOLARPANELSCONTROLER_MAIN_H
#define SOLARPANELSCONTROLER_MAIN_H

const char *ssid = "ASUS";
const char *password = "hesloheslo";
const int solarPin = 2;

String PrepateToParseJson(String json);

String GetValueFromString(String data, const String& findingKey);

#endif //SOLARPANELSCONTROLER_MAIN_H

