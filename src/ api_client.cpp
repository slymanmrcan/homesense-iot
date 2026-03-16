#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_system.h>
#include <time.h>
#include "api_client.h"
#include "secrets.h"

namespace
{
    constexpr time_t MIN_VALID_EPOCH = 1700000000;
    constexpr unsigned long NTP_WAIT_TIMEOUT_MS = 10000;
    bool g_ntpConfigured = false;
}

static String escapeJson(const String& input)
{
    String output;
    output.reserve(input.length() + 8);

    for (size_t i = 0; i < input.length(); i++)
    {
        char c = input[i];

        switch (c)
        {
        case '\"':
            output += "\\\"";
            break;
        case '\\':
            output += "\\\\";
            break;
        case '\n':
            output += "\\n";
            break;
        case '\r':
            output += "\\r";
            break;
        case '\t':
            output += "\\t";
            break;
        default:
            output += c;
            break;
        }
    }

    return output;
}

static String generateBatchKey()
{
    char uuid[37];

    uint32_t p1 = esp_random();
    uint16_t p2 = static_cast<uint16_t>(esp_random());
    uint16_t p3 = (static_cast<uint16_t>(esp_random()) & 0x0FFF) | 0x4000;
    uint16_t p4 = (static_cast<uint16_t>(esp_random()) & 0x3FFF) | 0x8000;
    uint32_t p5a = esp_random();
    uint16_t p5b = static_cast<uint16_t>(esp_random());

    snprintf(
        uuid,
        sizeof(uuid),
        "%08lx-%04x-%04x-%04x-%08lx%04x",
        static_cast<unsigned long>(p1),
        p2,
        p3,
        p4,
        static_cast<unsigned long>(p5a),
        p5b);

    return String(uuid);
}

static bool ensureUtcTimeReady()
{
    if (!g_ntpConfigured)
    {
        configTime(0, 0, "pool.ntp.org", "time.google.com", "time.windows.com");
        g_ntpConfigured = true;
    }

    if (time(nullptr) > MIN_VALID_EPOCH)
    {
        return true;
    }

    unsigned long startedAt = millis();

    while (millis() - startedAt < NTP_WAIT_TIMEOUT_MS)
    {
        delay(250);

        if (time(nullptr) > MIN_VALID_EPOCH)
        {
            return true;
        }
    }

    return time(nullptr) > MIN_VALID_EPOCH;
}

static bool getRecordedAtUtc(String& out)
{
    if (!ensureUtcTimeReady())
    {
        return false;
    }

    time_t now = time(nullptr);
    struct tm timeinfo;

    gmtime_r(&now, &timeinfo);

    char buffer[25];
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);

    out = String(buffer);
    return true;
}

bool sendSensorSnapshot(const SensorSnapshot& snapshot)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi bagli degil, veri gonderilemedi.");
        return false;
    }

    String recordedAtUtc;
    if (!getRecordedAtUtc(recordedAtUtc))
    {
        Serial.println("UTC saat senkronize degil, veri gonderilmedi.");
        return false;
    }

    HTTPClient http;
    http.begin(API_URL);
    http.setTimeout(10000);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", API_KEY);

    String payload;
    payload.reserve(512);

    payload += "{";
    payload += "\"macAddress\":\"" + escapeJson(WiFi.macAddress()) + "\",";
    payload += "\"batchKey\":\"" + generateBatchKey() + "\",";
    payload += "\"triggeredByThreshold\":";
    payload += (snapshot.trigger.triggered ? "true" : "false");

    if (snapshot.trigger.triggered)
    {
        payload += ",";
        payload += "\"triggerSensorType\":\"" + escapeJson(String(snapshot.trigger.triggerSensorType)) + "\",";
        payload += "\"triggerValue\":" + String(snapshot.trigger.triggerValue, 2) + ",";
        payload += "\"triggerRule\":\"" + escapeJson(String(snapshot.trigger.triggerRule)) + "\"";
    }

    payload += ",";
    payload += "\"recordedAtUtc\":\"" + recordedAtUtc + "\",";
    payload += "\"readings\":[";

    bool first = true;

    for (int i = 0; i < snapshot.count; i++)
    {
        if (!snapshot.readings[i].valid)
        {
            continue;
        }

        if (!first)
        {
            payload += ",";
        }

        payload += "{";
        payload += "\"sensorType\":\"" + escapeJson(String(snapshot.readings[i].sensorType)) + "\",";
        payload += "\"value\":" + String(snapshot.readings[i].value, 2) + ",";
        payload += "\"unit\":\"" + escapeJson(String(snapshot.readings[i].unit)) + "\"";
        payload += "}";

        first = false;
    }

    payload += "]";
    payload += "}";

    Serial.println("Payload:");
    Serial.println(payload);

    int httpResponseCode = http.POST(payload);

    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println("Response:");

        if (response.length() == 0)
        {
            Serial.println("(empty body)");
        }
        else
        {
            Serial.println(response);
        }
    }
    else
    {
        Serial.print("POST hatasi: ");
        Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();

    return httpResponseCode >= 200 && httpResponseCode < 300;
}