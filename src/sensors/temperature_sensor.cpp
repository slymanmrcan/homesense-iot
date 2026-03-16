#include <Arduino.h>
#include <DHT.h>
#include "temperature_sensor.h"

#define DHTPIN 4
#define DHTTYPE DHT11

static DHT dht(DHTPIN, DHTTYPE);

void initTemperatureSensor()
{
    dht.begin();
}

TemperatureHumidityData readTemperatureSensor()
{
    TemperatureHumidityData data;

    data.humidity = dht.readHumidity();
    data.temperature = dht.readTemperature();
    data.valid = !(isnan(data.humidity) || isnan(data.temperature));

    return data;
}
