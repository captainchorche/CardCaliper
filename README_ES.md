# CardCaliper - Indicador para calibradores

**🌍 Idiomas:** [English](README.md) | [Русский](README_RU.md) | [Italiano](README_IT.md) | [Español](README_ES.md)

![M5Stack Cardputer](https://img.shields.io/badge/Hardware-M5Stack%20Cardputer-blue)
![ESP32-S3](https://img.shields.io/badge/MCU-ESP32--S3-green)
![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-brightgreen)

Indicador digital para calibradores (u otros instrumentos de medición) con salida SPC basado en M5Stack Cardputer. El proyecto permite la lectura de mediciones con la posibilidad de transferir datos vía BLE, WiFi y la interfaz web.

Este proyecto está parcialmente basado en [EspDRO](https://github.com/MGX3D/EspDRO) de Marius G MGX3D.

## 🎯 Características principales

[YouTubeLink](https://www.youtube.com/shorts/dYnquSnqz-o)

![alt text](images/CardCaliperGIF.gif)
### 🔗 Modos de funcionamiento
- botón **1** - **BLE Keyboard**: emulación de teclado bluetooth para entrada directa, (botón **enter(ok)** para enviar datos al campo de entrada)
- botón **2** - **WiFi AP**: crear un punto de acceso propio
- botón **3** - **WiFi STA**: conectarse a la red doméstica (si falla la conexión, cambia automáticamente al modo AP)
- botón **4** - **Modo OFF**: desactiva los módulos inalámbricos  

### 🌐 Interfaz web
  SSID: `CardCaliper` (defaultSSID modo AP)

  Contraseña: `Caliper123` (defaultPASSWORD modo AP)

  URL: `http://cardcaliper.local`
- **PWA**: instalar como aplicación (añadir a la pantalla de inicio)
- **Tiempo real**: actualizaciones WebSocket sin retraso
- **Guardar datos**: clic en la medición o enter (escritorio) para guardar en la tabla
- **Copiar al portapapeles**: tocar o hacer clic (escritorio) en la medición para guardar en la tabla
- **Eliminar datos**: deslizar la medición en la tabla o usar el botón de emoji de papelera (eliminar todas las mediciones)
- **Exportar CSV**: descargar las mediciones guardadas en formato tabla

  Página de configuración (también se puede cambiar en user_config.h):
- **WiFi STA**: SSID y contraseña de la red doméstica
- **WiFi AP**: configuración del punto de acceso
- **Multiplicador de calibración**: corrección para diferentes calibradores
- **Nombre mDNS**: nombre del dispositivo en la red

## 🔧 Montaje
### Requisitos
- M5Stack Cardputer
- Calibrador digital con salida SPC
- PlatformIO IDE o PlatformIO Core
- Cables de conexión
- Si no tienes un conector HY2.0-4P (puerto grove) y una impresora 3D,
  yo soldé pines (DuPont). El de 3 pines encaja perfectamente en el conector SPC del calibrador, y el de 4 pines en lugar del puerto grove del cardputer.
  Pero si tienes un conector adecuado, úsalo junto con el [modelo impreso de conector para calibrador](https://github.com/MGX3D/EspDRO/blob/master/CAD/spc_connector.stl).

### Conexión del calibrador al Cardputer
![alt text](images/Pinout.png)

```
| Grove (Cardputer) | SPC (calibrador) |
|-------------------|------------------|
| GND               | GND              |
| G2 (GPIO2)        | Data             |
| G1 (GPIO1)        | Clock            |
```

## ⬇️ Instalación y flasheo

```bash
# Compilar y flashear
pio run --target upload

# Subir la interfaz web
pio run --target uploadfs
```
## ⬇️ Flasheo rápido

El firmware listo también está disponible en M5Burner

## 📄 Licencia

Licencia MIT - ver [LICENSE](LICENSE)

## 🏆 Agradecimientos

- Marius G (MGX3D) por implementar la lectura de datos sin usar convertidores de nivel lógico
- IA por la implementación del código mediante solicitudes idiotas
- Todos los involucrados en la escritura de las bibliotecas utilizadas
