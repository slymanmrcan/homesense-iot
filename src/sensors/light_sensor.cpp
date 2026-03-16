#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include "light_sensor.h"

static BH1750 lightMeter;

void initLightSensor()
{
    Wire.begin(21, 22);
    lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

LightData readLightSensor()
{
    LightData data;

    float lux = lightMeter.readLightLevel();

    data.lux = lux;
    data.valid = (lux >= 0);

    return data;
}
