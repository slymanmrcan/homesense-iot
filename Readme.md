# 🌡️ ESP32 IoT Sensor Network

ESP32 tabanlı dağıtık sensör ağı. Birden fazla cihaz ortam verisi toplar ve merkezi bir sunucuya iletir.

![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![Framework](https://img.shields.io/badge/framework-Arduino-teal)
![Build](https://img.shields.io/badge/build-PlatformIO-orange)

---

## Ne Yapar?

Birden fazla ESP32 cihazı sıcaklık, nem, mesafe ve hareket gibi sensörlerden veri toplar. Her cihaz bu veriyi belirli aralıklarla Wi-Fi üzerinden merkezi bir sunucuya HTTP POST ile gönderir.

```
[ESP32 #1] ──┐
[ESP32 #2] ──┼──▶ REST API ──▶ Sunucu ──▶ İzleme
[ESP32 #3] ──┘
```

---

## Özellikler

- 📡 Wi-Fi üzerinden otomatik bağlantı
- 🌡️ DHT11 ile sıcaklık ve nem ölçümü
- 📏 HC-SR04 ile mesafe ölçümü
- 🚶 PIR ile hareket algılama
- 🔁 Periyodik REST API veri gönderimi
- 🔌 Çoklu cihaz desteği (`device_id` bazlı)

---

## Donanım

| Sensör | Bağlantı Pini |
|--------|--------------|
| DHT11 (sıcaklık/nem) | GPIO4 |
| HC-SR04 (mesafe) | GPIO18 / GPIO19 |
| PIR (hareket) | GPIO5 |

> Detaylı bağlantı şeması için [`docs/wiring.md`](docs/wiring.md)

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

## Proje Yapısı

```
include/
├── api_client.h
├── wifi_manager.h
├── secrets.h            ← git'e eklenmez
└── secrets.h.example

src/
├── main.cpp
├── api_client.cpp
├── wifi_manager.cpp
└── sensors/
    └── temperature_sensor.cpp
```

---

## Gönderilen Veri Formatı

```json
{
  "device_id": "esp32-01",
  "temperature": 24.5,
  "humidity": 60,
  "distance_cm": 42,
  "motion": false,
  "timestamp": 1710000000
}
```

---
Bağlantılar 

<svg width="100%" viewBox="0 0 680 620" xmlns="http://www.w3.org/2000/svg">
<defs>
  <marker id="arrow" viewBox="0 0 10 10" refX="8" refY="5" markerWidth="6" markerHeight="6" orient="auto-start-reverse">
    <path d="M2 1L8 5L2 9" fill="none" stroke="context-stroke" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"/>
  </marker>
</defs>

<!-- ESP32 Board -->
<g>
  <rect x="250" y="140" width="180" height="340" rx="10" fill="none" stroke="#888780" stroke-width="1.5"/>
  <rect x="258" y="148" width="164" height="324" rx="8" fill="#D3D1C7" stroke="#B4B2A9" stroke-width="0.5"/>
  <text font-family="sans-serif" font-weight="500" font-size="14" x="340" y="175" text-anchor="middle" fill="#2C2C2A">ESP32</text>
  <text font-family="sans-serif" font-size="12" x="340" y="191" text-anchor="middle" fill="#5F5E5A">esp32dev</text>
  <rect x="322" y="142" width="36" height="8" rx="3" fill="#F1EFE8" stroke="#B4B2A9" stroke-width="0.5"/>
  <text font-family="sans-serif" font-size="7" x="340" y="148" text-anchor="middle" fill="#5F5E5A">USB</text>

  <!-- Left side pins -->
  <rect x="238" y="215" width="22" height="14" rx="2" fill="#2C2C2A" stroke="none"/>
  <text x="232" y="225" text-anchor="end" font-size="9" font-family="monospace" fill="#5F5E5A">GND</text>
  <rect x="238" y="240" width="22" height="14" rx="2" fill="#851111" stroke="none"/>
  <text x="232" y="250" text-anchor="end" font-size="9" font-family="monospace" fill="#5F5E5A">3V3</text>
  <rect x="238" y="265" width="22" height="14" rx="2" fill="#185FA5" stroke="none"/>
  <text x="232" y="275" text-anchor="end" font-size="9" font-family="monospace" fill="#5F5E5A">GPIO4</text>
  <rect x="238" y="290" width="22" height="14" rx="2" fill="#3B6D11" stroke="none"/>
  <text x="232" y="300" text-anchor="end" font-size="9" font-family="monospace" fill="#5F5E5A">GPIO5</text>
  <rect x="238" y="315" width="22" height="14" rx="2" fill="#634806" stroke="none"/>
  <text x="232" y="325" text-anchor="end" font-size="9" font-family="monospace" fill="#5F5E5A">GPIO18</text>
  <rect x="238" y="340" width="22" height="14" rx="2" fill="#634806" stroke="none"/>
  <text x="232" y="350" text-anchor="end" font-size="9" font-family="monospace" fill="#5F5E5A">GPIO19</text>

  <!-- Right side pins -->
  <rect x="420" y="215" width="22" height="14" rx="2" fill="#851111" stroke="none"/>
  <text x="450" y="225" font-size="9" font-family="monospace" fill="#5F5E5A">VIN (5V)</text>
  <rect x="420" y="240" width="22" height="14" rx="2" fill="#2C2C2A" stroke="none"/>
  <text x="450" y="250" font-size="9" font-family="monospace" fill="#5F5E5A">GND</text>
</g>

<!-- DHT11 -->
<g>
  <rect x="30" y="210" width="90" height="80" rx="8" fill="#E6F1FB" stroke="#185FA5" stroke-width="1"/>
  <text font-family="sans-serif" font-weight="500" font-size="14" x="75" y="232" text-anchor="middle" fill="#042C53">DHT11</text>
  <text font-family="sans-serif" font-size="12" x="75" y="248" text-anchor="middle" fill="#185FA5">Sıcaklık/Nem</text>
  <rect x="28" y="258" width="8" height="8" rx="1" fill="#2C2C2A"/>
  <text x="40" y="265" font-size="8" font-family="sans-serif" fill="#5F5E5A">–</text>
  <rect x="28" y="272" width="8" height="8" rx="1" fill="#851111"/>
  <text x="40" y="279" font-size="8" font-family="sans-serif" fill="#5F5E5A">+</text>
  <rect x="118" y="258" width="8" height="8" rx="1" fill="#185FA5"/>
  <text x="105" y="265" font-size="8" font-family="sans-serif" fill="#5F5E5A">S</text>
</g>

<!-- DHT11 wires -->
<path d="M36 262 L36 580 L340 580 L340 229" fill="none" stroke="#2C2C2A" stroke-width="1.5" stroke-dasharray="4 2"/>
<circle cx="340" cy="222" r="3" fill="#2C2C2A"/>
<path d="M36 276 L36 575 L335 575 L335 247" fill="none" stroke="#851111" stroke-width="1.5" stroke-dasharray="4 2" opacity="0.8"/>
<circle cx="335" cy="247" r="3" fill="#851111"/>
<path d="M126 262 L238 272" fill="none" stroke="#185FA5" stroke-width="1.5" marker-end="url(#arrow)"/>

<!-- PIR -->
<g>
  <rect x="30" y="330" width="90" height="70" rx="8" fill="#EAF3DE" stroke="#3B6D11" stroke-width="1"/>
  <text font-family="sans-serif" font-weight="500" font-size="14" x="75" y="352" text-anchor="middle" fill="#173404">PIR</text>
  <text font-family="sans-serif" font-size="12" x="75" y="368" text-anchor="middle" fill="#3B6D11">Hareket algılama</text>
  <rect x="28" y="375" width="8" height="8" rx="1" fill="#2C2C2A"/>
  <text x="40" y="382" font-size="8" font-family="sans-serif" fill="#5F5E5A">GND</text>
  <rect x="28" y="389" width="8" height="8" rx="1" fill="#851111"/>
  <text x="40" y="396" font-size="8" font-family="sans-serif" fill="#5F5E5A">VCC</text>
  <rect x="118" y="375" width="8" height="8" rx="1" fill="#3B6D11"/>
  <text x="103" y="382" font-size="8" font-family="sans-serif" fill="#5F5E5A">OUT</text>
</g>

<!-- PIR wires -->
<path d="M36 379 L26 379 L26 570 L344 570 L344 229" fill="none" stroke="#2C2C2A" stroke-width="1.5" stroke-dasharray="4 2"/>
<circle cx="344" cy="222" r="3" fill="#2C2C2A"/>
<path d="M36 393 L30 393 L30 565 L430 565 L430 247" fill="none" stroke="#851111" stroke-width="1.5" stroke-dasharray="4 2" opacity="0.8"/>
<circle cx="430" cy="247" r="3" fill="#851111"/>
<path d="M126 379 L238 297" fill="none" stroke="#3B6D11" stroke-width="1.5" marker-end="url(#arrow)"/>

<!-- HC-SR04 -->
<g>
  <rect x="480" y="270" width="100" height="100" rx="8" fill="#FAEEDA" stroke="#634806" stroke-width="1"/>
  <text font-family="sans-serif" font-weight="500" font-size="14" x="530" y="294" text-anchor="middle" fill="#412402">HC-SR04</text>
  <text font-family="sans-serif" font-size="12" x="530" y="310" text-anchor="middle" fill="#634806">Mesafe</text>
  <rect x="476" y="318" width="8" height="8" rx="1" fill="#851111"/>
  <text x="488" y="325" font-size="8" font-family="sans-serif" fill="#5F5E5A">VCC</text>
  <rect x="476" y="332" width="8" height="8" rx="1" fill="#2C2C2A"/>
  <text x="488" y="339" font-size="8" font-family="sans-serif" fill="#5F5E5A">GND</text>
  <rect x="476" y="346" width="8" height="8" rx="1" fill="#634806"/>
  <text x="488" y="353" font-size="8" font-family="sans-serif" fill="#5F5E5A">TRIG</text>
  <rect x="476" y="360" width="8" height="8" rx="1" fill="#634806"/>
  <text x="488" y="367" font-size="8" font-family="sans-serif" fill="#5F5E5A">ECHO</text>
</g>

<!-- HC-SR04 wires -->
<path d="M484 322 L465 322 L465 222 L442 222" fill="none" stroke="#851111" stroke-width="1.5" stroke-dasharray="4 2" opacity="0.8" marker-end="url(#arrow)"/>
<path d="M484 336 L470 336 L470 247 L442 247" fill="none" stroke="#2C2C2A" stroke-width="1.5" stroke-dasharray="4 2" marker-end="url(#arrow)"/>
<path d="M484 350 L460 350 L460 322 L442 322" fill="none" stroke="#634806" stroke-width="1.5" marker-end="url(#arrow)"/>
<path d="M484 364 L455 364 L455 347 L442 347" fill="none" stroke="#634806" stroke-width="1.5" marker-end="url(#arrow)"/>

<!-- Legend -->
<rect x="30" y="460" width="540" height="60" rx="8" fill="#F1EFE8" stroke="#B4B2A9" stroke-width="0.5"/>
<text font-family="sans-serif" font-size="12" x="50" y="480" fill="#5F5E5A">Kablo renkleri:</text>
<rect x="50"  y="490" width="12" height="6" rx="2" fill="#851111"/>
<text font-family="sans-serif" font-size="12" x="68" y="497" fill="#5F5E5A">3.3V / VCC</text>
<rect x="160" y="490" width="12" height="6" rx="2" fill="#2C2C2A"/>
<text font-family="sans-serif" font-size="12" x="178" y="497" fill="#5F5E5A">GND</text>
<rect x="240" y="490" width="12" height="6" rx="2" fill="#185FA5"/>
<text font-family="sans-serif" font-size="12" x="258" y="497" fill="#5F5E5A">DATA (DHT11)</text>
<rect x="370" y="490" width="12" height="6" rx="2" fill="#3B6D11"/>
<text font-family="sans-serif" font-size="12" x="388" y="497" fill="#5F5E5A">OUT (PIR)</text>
<rect x="460" y="490" width="12" height="6" rx="2" fill="#634806"/>
<text font-family="sans-serif" font-size="12" x="478" y="497" fill="#5F5E5A">TRIG/ECHO</text>

<!-- Note -->
<text font-family="sans-serif" font-size="11" x="340" y="545" text-anchor="middle" fill="#888780">HC-SR04 → 5V ile besle  |  DHT11 ve PIR → 3.3V yeterli  |  Bağlantı öncesi USB'yi çıkar</text>
</svg>

---


## Yol Haritası

- [x] Wi-Fi bağlantısı
- [x] DHT11 sıcaklık/nem okuma
- [ ] REST API POST entegrasyonu
- [ ] HC-SR04 mesafe sensörü
- [ ] PIR hareket sensörü
- [ ] Gerçek backend bağlantısı
- [ ] OTA (kablosuz güncelleme)

---

## Gereksinimler

- VS Code + PlatformIO
- ESP32 geliştirme kartı (`esp32dev`)
- DHT11 / HC-SR04 / PIR sensörleri





