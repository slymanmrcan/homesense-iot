#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"
#include "temperature_sensor.h"
#include "api_client.h"

unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 2 * 60 * 1000; 

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

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("Sistem basliyor...");
    configTime(0, 0, "pool.ntp.org", "time.google.com");

    initTemperatureSensor();
    connectWifi();
}

void loop()
{
    if (millis() - lastReadTime >= READ_INTERVAL)
    {
        lastReadTime = millis();

        SensorData data = readTemperatureSensor();

        if (!data.valid)
        {
            Serial.println("Sensor verisi okunamadi.");
            return;
        }

        Serial.print("Sicaklik: ");
        Serial.print(data.temperature);
        Serial.print(" C | Nem: ");
        Serial.print(data.humidity);
        Serial.println(" %");

        bool success = sendSensorData(data.temperature, data.humidity);

        if (success)
        {
            Serial.println("Veri endpoint'e gonderildi.");
        }
        else
        {
            Serial.println("Veri gonderimi basarisiz.");
        }
    }
}
