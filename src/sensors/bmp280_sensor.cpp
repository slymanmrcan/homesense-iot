#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "bmp280_sensor.h"

static Adafruit_BMP280 bmp;
static bool bmpInitialized = false;

void initBmp280Sensor()
{
    // BH1750 de aynı I2C hattını kullanıyor, tekrar begin etmek sorun değil
    Wire.begin(21, 22);

    if (bmp.begin(0x76))
    {
        bmpInitialized = true;
        Serial.println("BMP280 bulundu. I2C adresi: 0x76");
    }
    else if (bmp.begin(0x77))
    {
        bmpInitialized = true;
        Serial.println("BMP280 bulundu. I2C adresi: 0x77");
    }
    else
    {
        bmpInitialized = false;
        Serial.println("BMP280 bulunamadi.");
    }
}

Bmp280Data readBmp280Sensor()
{
    Bmp280Data data;

    if (!bmpInitialized)
    {
        data.valid = false;
        data.temperature = 0;
        data.pressureHpa = 0;
        return data;
    }

    data.temperature = bmp.readTemperature();
    data.pressureHpa = bmp.readPressure() / 100.0f;
    data.valid = true;

    return data;
}
