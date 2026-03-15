#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

struct SensorData
{
    float temperature;
    float humidity;
    bool valid;
};

void initTemperatureSensor();
SensorData readTemperatureSensor();

#endif
