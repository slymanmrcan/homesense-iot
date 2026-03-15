#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_system.h>
#include <time.h>
#include "api_client.h"
#include "secrets.h"

static String generateBatchKey()
{
    char uuid[37];

    uint32_t p1 = esp_random();
    uint16_t p2 = (uint16_t)esp_random();
    uint16_t p3 = ((uint16_t)esp_random() & 0x0FFF) | 0x4000;
    uint16_t p4 = ((uint16_t)esp_random() & 0x3FFF) | 0x8000;
    uint32_t p5a = esp_random();
    uint16_t p5b = (uint16_t)esp_random();

    snprintf(
        uuid,
        sizeof(uuid),
        "%08lx-%04x-%04x-%04x-%08lx%04x",
        (unsigned long)p1,
        p2,
        p3,
        p4,
        (unsigned long)p5a,
        p5b);

    return String(uuid);
}

static String getRecordedAtUtc()
{
    time_t now = time(nullptr);

    if (now > 1700000000)
    {
        struct tm timeinfo;
        gmtime_r(&now, &timeinfo);

        char buffer[25];
        strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
        return String(buffer);
    }

    unsigned long totalSeconds = millis() / 1000;
    unsigned long hours = (totalSeconds / 3600) % 24;
    unsigned long minutes = (totalSeconds / 60) % 60;
    unsigned long seconds = totalSeconds % 60;

    char buffer[25];
    snprintf(buffer, sizeof(buffer), "2026-03-15T%02lu:%02lu:%02luZ", hours, minutes, seconds);
    return String(buffer);
}

bool sendSensorData(float temperature, float humidity)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi bagli degil, veri gonderilemedi.");
        return false;
    }

    HTTPClient http;
    http.begin(API_URL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", API_KEY);

    String payload = "{";
    payload += "\"macAddress\":\"" + WiFi.macAddress() + "\",";
    payload += "\"batchKey\":\"" + generateBatchKey() + "\",";
    payload += "\"triggeredByThreshold\":false,";
    payload += "\"recordedAtUtc\":\"" + getRecordedAtUtc() + "\",";
    payload += "\"readings\":[";
    payload += "{\"sensorType\":\"temperature\",\"value\":" + String(temperature, 2) + ",\"unit\":\"C\"},";
    payload += "{\"sensorType\":\"humidity\",\"value\":" + String(humidity, 2) + ",\"unit\":\"%\"}";
    payload += "]}";

    Serial.println(payload);

    int httpResponseCode = http.POST(payload);

    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println("Response:");
        Serial.println(response);
    }
    else
    {
        Serial.print("POST hatasi: ");
        Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();

    return httpResponseCode > 0 && httpResponseCode < 300;
}
