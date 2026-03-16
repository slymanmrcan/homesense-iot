# 🌡️ ESP32 IoT Sensor Network

ESP32 tabanlı dağıtık sensör ağı. Birden fazla cihaz ortam verisi toplar ve merkezi bir sunucuya iletir.

![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![Framework](https://img.shields.io/badge/framework-Arduino-teal)
![Build](https://img.shields.io/badge/build-PlatformIO-orange)

---

## Ne Yapar?

Birden fazla ESP32 cihazı sıcaklık, nem, basınç, ışık ve yakınlık gibi sensörlerden veri toplar. Her cihaz bu veriyi belirli aralıklarla Wi-Fi üzerinden merkezi bir sunucuya HTTP POST ile gönderir.

```
[ESP32 #1] ──┐
[ESP32 #2] ──┼──▶ REST API ──▶ Sunucu ──▶ İzleme
[ESP32 #3] ──┘
```

---

## Özellikler

- 📡 Wi-Fi üzerinden otomatik bağlantı
- 🌡️ DHT11 ile sıcaklık ve nem ölçümü
- 🔬 BMP280 ile sıcaklık ve basınç ölçümü
- 💡 BH1750 ile ışık yoğunluğu ölçümü
- 🚨 LM393 ile yakınlık algılama
- 🔁 Periyodik REST API veri gönderimi
- 🔌 Çoklu cihaz desteği (`device_id` bazlı)

---

## Donanım

| Sensör | Protokol | Pin |
|--------|----------|-----|
| DHT11 (sıcaklık / nem) | Single-wire | GPIO4 |
| BMP280 (sıcaklık / basınç) | I²C | GPIO21 (SDA) / GPIO22 (SCL) |
| BH1750 (ışık) | I²C | GPIO21 (SDA) / GPIO22 (SCL) |
| LM393 (yakınlık) | Digital | GPIO5 |

> **I²C Adresleri:** BH1750 → `0x23` · BMP280 → `0x76`

### Devre Şeması

![Devre Şeması](image.svg)

---

## Proje Yapısı

```
include/
├── api_client.h
├── bmp280_sensor.h
├── light_sensor.h
├── proximity_sensor.h
├── reading_models.h
├── temperature_sensor.h
├── wifi_manager.h
├── secrets.h              ← git'e ekleme!
└── secrets.h.example

src/
├── main.cpp
├── api_client.cpp
├── wifi_manager.cpp
└── sensors/
    ├── bmp280_sensor.cpp
    ├── light_sensor.cpp
    ├── proximity_sensor.cpp
    └── temperature_sensor.cpp

docs/
└── schematic.svg
```

---

## Kurulum

**1. Repoyu klonla**
```bash
git clone https://github.com/kullanici/esp32-iot-sensor.git
cd esp32-iot-sensor
```

**2. Secrets dosyasını oluştur**
```bash
cp include/secrets.h.example include/secrets.h
```

```cpp
// include/secrets.h
#define WIFI_SSID     "WIFI_ADI"
#define WIFI_PASSWORD "WIFI_SIFRESI"
#define API_ENDPOINT  "http://sunucu_ip/api/data"
```

**3. Yükle**

PlatformIO üzerinden `Upload` veya:
```bash
pio run --target upload
```

---

## Gönderilen Veri Formatı

```json
{
  "device_id": "esp32-01",
  "temperature_dht": 24.6,
  "humidity": 17.0,
  "temperature_bmp": 25.1,
  "pressure_hpa": 893.45,
  "light_lux": 1.67,
  "proximity": false,
  "timestamp": 1710000000
}
```

---

## Yol Haritası

- [x] Wi-Fi bağlantısı
- [x] DHT11 sıcaklık / nem okuma
- [x] BMP280 basınç / sıcaklık okuma
- [x] BH1750 ışık okuma
- [x] LM393 yakınlık algılama
- [ ] REST API POST entegrasyonu
- [ ] Gerçek backend bağlantısı
- [ ] OTA (kablosuz güncelleme)

---

## Gereksinimler

- VS Code + PlatformIO
- ESP32 geliştirme kartı (`esp32dev`)
- DHT11 · BMP280 · BH1750 · LM393 sensörleri