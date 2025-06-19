# CardCaliper - Digital Caliper Reader

**🌍 Languages:** [English](README.md) | [Русский](README_RU.md) | [Italiano](README_IT.md) | [Español](README_ES.md)

![M5Stack Cardputer](https://img.shields.io/badge/Hardware-M5Stack%20Cardputer-blue)
![ESP32-S3](https://img.shields.io/badge/MCU-ESP32--S3-green)
![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-brightgreen)

Digital reader for calipers (or other measuring instruments) with SPC output based on M5Stack Cardputer. The project provides measurement reading with data transmission capabilities via BLE, WiFi, and web interface.

This project is partially based on [EspDRO](https://github.com/MGX3D/EspDRO) project by Marius G MGX3D.

## 🎯 Key Features

### 🔗 Operating Modes
- **Button 1** - **BLE Keyboard**: Bluetooth keyboard emulation for direct input (press **enter(ok)** button to send data to input field)
- **Button 2** - **WiFi AP**: Create own access point
- **Button 3** - **WiFi STA**: Connect to home network (automatically switches to AP mode if connection fails)
- **Button 4** - **OFF Mode**: Disable wireless modules

### 🌐 Web Interface
  SSID: `CardCaliper` (default SSID AP mode)

  Password: `Caliper123` (default PASSWORD AP mode)

  URL: `http://cardcaliper.local`
- **PWA**: Install as app (add to home screen)
- **Real-time**: WebSocket updates without delays
- **Data saving**: Click on measurement or enter (desktop) to save to table
- **Copy to clipboard**: Tap or click on measurement (desktop) to copy to clipboard
- **Data deletion**: Swipe measurement in table or trash emoji button (delete all measurements)
- **CSV export**: Download saved measurements in table format

  Settings page (can also be changed in user_config.h):
- **WiFi STA**: Home network SSID and password
- **WiFi AP**: Access point settings
- **Calibration multiplier**: Correction for different calipers
- **mDNS name**: Device name on network

## 🔧 Build

### Requirements
- M5Stack Cardputer
- Digital caliper with SPC output
- PlatformIO IDE or PlatformIO Core
- Connecting wires
- In absence of spare HY2.0-4P connector (grove port) and 3D printer,
 I soldered pin headers (DuPont). 3-pin fits perfectly into SPC connector of caliper, and 4-pin instead of grove port on cardputer.
 But if you have suitable connector, use it with [printed caliper connector model](https://github.com/MGX3D/EspDRO/blob/master/CAD/spc_connector.stl).

### Connecting Caliper to Cardputer

```
| Grove (Cardputer) | SPC (Caliper)        |
|-------------------|----------------------|
| GND               | GND                  |
| G2 (GPIO2)        | Data                 |
| G1 (GPIO1)        | Clock                |
```

## ⬇️ Installation and Flashing

```bash
# Build and flash
pio run --target upload

# Upload web interface
pio run --target uploadfs
```

## ⬇️ Quick Flashing

Ready firmware is also available on M5Burner

## 📄 License

MIT License - see [LICENSE](LICENSE) file

## 🏆 Acknowledgments

- Marius G (MGX3D) for implementing data reading without using logic level converters
- AI for code implementation through idiotic requests
- Everyone involved in writing the used libraries
