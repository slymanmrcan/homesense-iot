#ifndef READING_MODELS_H
#define READING_MODELS_H

#define MAX_SENSOR_READINGS 16

struct SensorReading
{
    const char* sensorType;
    float value;
    const char* unit;
    bool valid;
};

struct TriggerInfo
{
    bool triggered;
    const char* triggerSensorType;
    float triggerValue;
    const char* triggerRule;
};

struct SensorSnapshot
{
    SensorReading readings[MAX_SENSOR_READINGS];
    int count;
    TriggerInfo trigger;
};

struct TemperatureHumidityData
{
    float temperature;
    float humidity;
    bool valid;
};

struct ProximityData
{
    bool isNear;
    int rawValue;
    int hitCount;
    bool valid;
};
struct LightData
{
    float lux;
    bool valid;
};
struct Bmp280Data
{
    float temperature;
    float pressureHpa;
    bool valid;
};
#endif
