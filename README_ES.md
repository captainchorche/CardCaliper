# CardCaliper - Indicador para Calibradores

**🌍 Idiomas:** [English](README.md) | [Русский](README_RU.md) | [Italiano](README_IT.md) | [Español](README_ES.md)

![M5Stack Cardputer](https://img.shields.io/badge/Hardware-M5Stack%20Cardputer-blue)
![ESP32-S3](https://img.shields.io/badge/MCU-ESP32--S3-green)
![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-brightgreen)

Un indicador digital para calibradores (u otros instrumentos de medición) con salida SPC basado en el M5Stack Cardputer. El proyecto proporciona lectura de mediciones con la capacidad de transmitir datos a través de BLE, WiFi y una interfaz web.

Este proyecto se basa parcialmente en el proyecto [EspDRO](https://github.com/MGX3D/EspDRO) de Marius G MGX3D.

## 🎯 Características Principales

### 🔗 Modos de Operación
- **Botón 1** - **Teclado BLE**: emula un teclado Bluetooth para entrada directa (presione **enter(ok)** para enviar datos al campo de entrada)
- **Botón 2** - **WiFi AP**: crea su propio punto de acceso
- **Botón 3** - **WiFi STA**: se conecta a una red doméstica (cambia automáticamente al modo AP si la conexión falla)
- **Botón 4** - **Modo OFF**: deshabilita los módulos inalámbricos

### 🌐 Interfaz Web
  SSID: `CardCaliper` (SSID predeterminado en modo AP)

  Contraseña: `Caliper123` (contraseña predeterminada en modo AP)

  URL: `http://cardcaliper.local`
- **PWA**: instalar como una aplicación (agregar a la pantalla de inicio)
- **Tiempo real**: actualizaciones de WebSocket sin demora
- **Guardado de Datos**: haga clic en una medición o presione enter (escritorio) para guardarla en la tabla
- **Copiar al Portapapeles**: toque o haga clic (escritorio) en una medición para copiarla
- **Eliminación de Datos**: deslice el dedo sobre una medición en la tabla o use el botón con el emoji de la papelera (elimina todas las mediciones)
- **Exportar a CSV**: descargue las mediciones guardadas en formato de tabla

  Página de configuración (también se puede cambiar en user_config.h):
- **WiFi STA**: SSID y contraseña de la red doméstica
- **WiFi AP**: configuración del punto de acceso
- **Multiplicador de Calibración**: corrección para diferentes calibradores
- **Nombre mDNS**: nombre del dispositivo en la red

## 🔧 Ensamblaje
### Requisitos
- M5Stack Cardputer
- Calibrador digital con salida SPC
- PlatformIO IDE o PlatformIO Core
- Cables de conexión
- A falta de un conector HY2.0-4P de repuesto (puerto grove) y una impresora 3D,
 soldé conectores de pines DuPont. Un conector de 3 pines encaja perfectamente en el conector SPC del calibrador, y un conector de 4 pines reemplaza el puerto grove del Cardputer.
 Pero si tiene un conector adecuado, úselo con el [modelo de conector de calibrador impreso](https://github.com/MGX3D/EspDRO/blob/master/CAD/spc_connector.stl).

### Conexión del Calibrador al Cardputer
![alt text](images/Pinout.png)

```
| Grove (Cardputer) | SPC (calibrador)     |
|-------------------|----------------------|
| GND               | GND                  |
| G2 (GPIO2)        | Data                 |
| G1 (GPIO1)        | Clock                |
```

## ⬇️ Instalación y Flasheo

```bash
# Compilar y flashear
pio run --target upload

# Subir interfaz web
pio run --target uploadfs
```
## ⬇️ Flasheo Rápido

Un firmware precompilado también está disponible en M5Burner

## 📄 Licencia

Licencia MIT - consulte el archivo [LICENSE](LICENSE)

## 🏆 Agradecimientos

- Marius G (MGX3D) por implementar la lectura de datos sin usar convertidores de nivel lógico
- IA por implementar el código a través de solicitudes idiotas
- Todos los involucrados en la escritura de las bibliotecas utilizadas
