//
// Created by Strejda Jára on 31.01.2022.
//

#ifndef SOLARPANELSCONTROLER_MAIN_H
#define SOLARPANELSCONTROLER_MAIN_H

#include "Parameter.h"
#define RT0 10000   // Ω
#define B 3977      // K
#define VCC 5    //Supply voltage
#define R 10000  //R=10KΩ
#define solarPin 4 \


String url = "http://solarpanelcontroller.azurewebsites.net/api/ControllerEndpoint?";
//String url =  "http://192.168.1.58:7071/api/ControllerEndpoint?";
HTTPClient http;
WiFiClient wiFiClient;
const char *ssid = "ASUS";
const char *password = "hesloheslo";
double mps = 0;
unsigned long timer = 0;
double t = 0;
float RT, VR, ln, tx, T0, VRT;
int counter = 0;
int anemo_pin = 14;

Parameter wind;
Parameter temp;

void ConnectToWifi();

#endif //SOLARPANELSCONTROLER_MAIN_H

