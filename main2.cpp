#include <Arduino.h>
#include "temperature_sensor.h"
#include "proximity_sensor.h"
#include "light_sensor.h"
#include "api_client.h"
#include "wifi_manager.h"
#include "reading_models.h"

unsigned long lastTemperatureReadTime = 0;
unsigned long lastProximityReadTime = 0;
unsigned long lastLightReadTime = 0;
unsigned long lastPeriodicSendTime = 0;

const unsigned long TEMPERATURE_READ_INTERVAL = 5000;
const unsigned long PROXIMITY_READ_INTERVAL = 100;
const unsigned long LIGHT_READ_INTERVAL = 5000;
const unsigned long PERIODIC_SEND_INTERVAL = 600000;

TemperatureHumidityData latestTemperatureData = {0, 0, false};
ProximityData latestProximityData = {false, 1, 0, true};
LightData latestLightData = {0, false};

bool lastAlarmState = false;

static SensorSnapshot buildSnapshot(bool triggeredByThreshold)
{
    SensorSnapshot snapshot;
    snapshot.count = 0;

    snapshot.trigger.triggered = triggeredByThreshold;
    snapshot.trigger.triggerSensorType = "";
    snapshot.trigger.triggerValue = 0;
    snapshot.trigger.triggerRule = "";

    if (latestTemperatureData.valid)
    {
        snapshot.readings[snapshot.count++] = {
            "temperature",
            latestTemperatureData.temperature,
            "C",
            true};

        snapshot.readings[snapshot.count++] = {
            "humidity",
            latestTemperatureData.humidity,
            "%",
            true};
    }

    if (latestLightData.valid)
    {
        snapshot.readings[snapshot.count++] = {
            "light",
            latestLightData.lux,
            "lux",
            true};
    }

    if (latestProximityData.valid)
    {
        snapshot.readings[snapshot.count++] = {
            "proximity",
            latestProximityData.isNear ? 1.0f : 0.0f,
            "state",
            true};
    }

    if (triggeredByThreshold)
    {
        snapshot.trigger.triggered = true;
        snapshot.trigger.triggerSensorType = "proximity";
        snapshot.trigger.triggerValue = latestProximityData.isNear ? 1.0f : 0.0f;
        snapshot.trigger.triggerRule = "isNear == true";
    }

    return snapshot;
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("System starting...");

    initTemperatureSensor();
    initProximitySensor();
    initLightSensor();
    connectWifi();
}

void loop()
{
    unsigned long now = millis();

    if (now - lastTemperatureReadTime >= TEMPERATURE_READ_INTERVAL)
    {
        lastTemperatureReadTime = now;
        latestTemperatureData = readTemperatureSensor();

        if (latestTemperatureData.valid)
        {
            Serial.print("Temperature: ");
            Serial.print(latestTemperatureData.temperature);
            Serial.print(" C | Humidity: ");
            Serial.print(latestTemperatureData.humidity);
            Serial.println(" %");
        }
        else
        {
            Serial.println("Temperature/Humidity read failed.");
        }
    }

    if (now - lastLightReadTime >= LIGHT_READ_INTERVAL)
    {
        lastLightReadTime = now;
        latestLightData = readLightSensor();

        if (latestLightData.valid)
        {
            Serial.print("Light: ");
            Serial.print(latestLightData.lux);
            Serial.println(" lux");
        }
        else
        {
            Serial.println("Light read failed.");
        }
    }

    if (now - lastProximityReadTime >= PROXIMITY_READ_INTERVAL)
    {
        lastProximityReadTime = now;
        latestProximityData = readProximitySensor();

        if (latestProximityData.isNear && !lastAlarmState)
        {
            Serial.println("Proximity threshold triggered.");

            SensorSnapshot snapshot = buildSnapshot(true);
            sendSensorSnapshot(snapshot);
        }

        lastAlarmState = latestProximityData.isNear;
    }

    if (now - lastPeriodicSendTime >= PERIODIC_SEND_INTERVAL)
    {
        lastPeriodicSendTime = now;

        Serial.println("Sending periodic sensor snapshot...");
        SensorSnapshot snapshot = buildSnapshot(false);
        sendSensorSnapshot(snapshot);
    }
}
