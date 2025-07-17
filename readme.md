# CardCaliper - Indicator for Calipers

**🌍 Languages:** [English](README.md) | [Русский](README_RU.md) | [Italiano](README_IT.md) | [Español](README_ES.md)

![M5Stack Cardputer](https://img.shields.io/badge/Hardware-M5Stack%20Cardputer-blue)
![ESP32-S3](https://img.shields.io/badge/MCU-ESP32--S3-green)
![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-brightgreen)

A digital indicator for calipers (or other measuring instruments) with an SPC output, based on the M5Stack Cardputer. The project provides measurement readings with the ability to transmit data via BLE, WiFi, and a web interface.

This project is partially based on the [EspDRO](https://github.com/MGX3D/EspDRO) project by Marius G MGX3D.

## 🎯 Key Features

[YouTubeLink](https://www.youtube.com/shorts/dYnquSnqz-o)

![alt text](images/CardCaliperGIF.gif)

### 🔗 Operating Modes
- **Button 1** - **BLE Keyboard**: emulates a Bluetooth keyboard for direct input (press the **enter(ok)** button to send data to the input field).
- **Button 2** - **WiFi AP**: creates its own access point.
- **Button 3** - **WiFi STA**: connects to a home network (if the connection fails, it automatically switches to AP mode).
- **Button 4** - **OFF Mode**: disables wireless modules.

### 🌐 Web Interface
  SSID: `CardCaliper` (defaultSSID AP mode)

  Password: `Caliper123` (defaultPASSWORD AP mode)

  URL: `http://cardcaliper.local`
- **PWA**: install as an application (add to home screen).
- **Real-time**: WebSocket updates without delays.
- **Data Saving**: click on a measurement or press enter (desktop) to save it to the table.
- **Copy to Clipboard**: tap or click (desktop) on a measurement to copy it.
- **Data Deletion**: swipe on a measurement in the table or use the trash can emoji button (deletes all measurements).
- **CSV Export**: download saved measurements in table format.

  Settings page (can also be changed in user_config.h):
- **WiFi STA**: home network SSID and password.
- **WiFi AP**: access point settings.
- **Calibration Multiplier**: correction for different calipers.
- **mDNS Name**: device name on the network.

## 🔧 Assembly
### Requirements
- M5Stack Cardputer
- Digital caliper with SPC output
- PlatformIO IDE or PlatformIO Core
- Connecting wires
- In the absence of a spare HY2.0-4P connector (grove port) and a 3D printer, I soldered DuPont pin headers. A 3-pin header fits perfectly into the caliper's SPC connector, and a 4-pin header replaces the Cardputer's grove port.
- But if you have a suitable connector, use it with the [printed caliper connector model](https://github.com/MGX3D/EspDRO/blob/master/CAD/spc_connector.stl).

### Connecting the Caliper to the Cardputer
![alt text](images/Pinout.png)

```
| Grove (Cardputer) | SPC (caliper) |
|-------------------|---------------|
| GND               | GND           |
| G2 (GPIO2)        | Data          |
| G1 (GPIO1)        | Clock         |
```

## ⬇️ Installation and Flashing

```bash
# Build and flash
pio run --target upload

# Upload web interface
pio run --target uploadfs
```
## ⬇️ Quick Flashing

A ready-made firmware is also available on M5Burner.

## 📄 License

MIT License - see the [LICENSE](LICENSE) file.

## 🏆 Acknowledgements

- Marius G (MGX3D) for implementing data reading without using logic level shifters.
- AI for implementing the code through idiotic requests.
- Everyone involved in writing the libraries used.
