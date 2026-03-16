#include <Arduino.h>
#include "proximity_sensor.h"

#define PROXIMITY_SENSOR_PIN 27
#define PROXIMITY_HIT_THRESHOLD 3

static int hitCount = 0;

void initProximitySensor()
{
    pinMode(PROXIMITY_SENSOR_PIN, INPUT);
}

ProximityData readProximitySensor()
{
    ProximityData data;

    int rawValue = digitalRead(PROXIMITY_SENSOR_PIN);
    bool nearObject = (rawValue == 0);

    if (nearObject)
    {
        hitCount++;
    }
    else
    {
        hitCount = 0;
    }

    data.rawValue = rawValue;
    data.hitCount = hitCount;
    data.isNear = (hitCount >= PROXIMITY_HIT_THRESHOLD);
    data.valid = true;

    return data;
}
