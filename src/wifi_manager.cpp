#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"
#include "wifi_manager.h"

static unsigned long lastWifiAttemptMs = 0;
static const unsigned long WIFI_RETRY_INTERVAL_MS = 10000;
static bool wifiStarted = false;

bool isWifiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void initWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);

    Serial.print("[WiFi] Ilk baglanti denemesi: ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    wifiStarted = true;
    lastWifiAttemptMs = millis();
}

void ensureWifiConnection() {
    if (isWifiConnected()) {
        static bool wasConnectedLogged = false;

        if (!wasConnectedLogged) {
            Serial.println("[WiFi] Baglandi.");
            Serial.print("[WiFi] IP: ");
            Serial.println(WiFi.localIP());
            Serial.print("[WiFi] RSSI: ");
            Serial.println(WiFi.RSSI());
            wasConnectedLogged = true;
        }

        return;
    }

    static bool wasConnectedLogged = false;
    wasConnectedLogged = false;

    unsigned long now = millis();

    if (!wifiStarted || (now - lastWifiAttemptMs >= WIFI_RETRY_INTERVAL_MS)) {
        Serial.print("[WiFi] Baglanma denemesi: ");
        Serial.println(WIFI_SSID);

        WiFi.disconnect(false, false);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        wifiStarted = true;
        lastWifiAttemptMs = now;
    }
}