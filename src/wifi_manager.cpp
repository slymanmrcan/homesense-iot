#include <Arduino.h>
#include <WiFi.h>
#include "wifi_manager.h"
#include "secrets.h"

void connectWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("WiFi baglaniyor");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi baglandi.");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

bool isWifiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}
