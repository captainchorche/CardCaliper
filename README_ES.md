# CardCaliper - Lector Digital para Calibres

**🌍 Idiomas:** [English](README.md) | [Русский](README_RU.md) | [Italiano](README_IT.md) | [Español](README_ES.md)

![M5Stack Cardputer](https://img.shields.io/badge/Hardware-M5Stack%20Cardputer-blue)
![ESP32-S3](https://img.shields.io/badge/MCU-ESP32--S3-green)
![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-brightgreen)

Lector digital para calibres (u otros instrumentos de medición) con salida SPC basado en M5Stack Cardputer. El proyecto proporciona lectura de mediciones con capacidades de transmisión de datos a través de BLE, WiFi e interfaz web.

Este proyecto está parcialmente basado en el proyecto [EspDRO](https://github.com/MGX3D/EspDRO) de Marius G MGX3D.

## 🎯 Características Principales

### 🔗 Modos de Operación
- **Botón 1** - **BLE Keyboard**: Emulación de teclado Bluetooth para entrada directa (presionar **enter(ok)** para enviar datos al campo de entrada)
- **Botón 2** - **WiFi AP**: Crear propio punto de acceso
- **Botón 3** - **WiFi STA**: Conectar a red doméstica (cambia automáticamente al modo AP si falla la conexión)
- **Botón 4** - **Modo OFF**: Deshabilitar módulos inalámbricos

### 🌐 Interfaz Web
  SSID: `CardCaliper` (SSID predeterminado modo AP)

  Contraseña: `Caliper123` (CONTRASEÑA predeterminada modo AP)

  URL: `http://cardcaliper.local`
- **PWA**: Instalar como aplicación (agregar a pantalla de inicio)
- **Tiempo real**: Actualizaciones WebSocket sin retrasos
- **Guardado de datos**: Hacer clic en medición o enter (escritorio) para guardar en tabla
- **Copiar al portapapeles**: Tocar o hacer clic en medición (escritorio) para copiar al portapapeles
- **Eliminación de datos**: Deslizar medición en tabla o botón emoji papelera (eliminar todas las mediciones)
- **Exportación CSV**: Descargar mediciones guardadas en formato tabla

  Página de configuración (también se puede cambiar en user_config.h):
- **WiFi STA**: SSID y contraseña de red doméstica
- **WiFi AP**: Configuración de punto de acceso
- **Multiplicador de calibración**: Corrección para diferentes calibres
- **Nombre mDNS**: Nombre del dispositivo en la red

## 🔧 Construcción

### Requisitos
- M5Stack Cardputer
- Calibre digital con salida SPC
- PlatformIO IDE o PlatformIO Core
- Cables de conexión
- En ausencia de conector HY2.0-4P de repuesto (puerto grove) e impresora 3D,
 soldé headers de pines (DuPont). El 3-pin encaja perfectamente en el conector SPC del calibre, y el 4-pin en lugar del puerto grove en cardputer.
 Pero si tienes un conector adecuado, úsalo con el [modelo de conector de calibre impreso](https://github.com/MGX3D/EspDRO/blob/master/CAD/spc_connector.stl).

### Conexión de Calibre a Cardputer

```
| Grove (Cardputer) | SPC (Calibre)        |
|-------------------|----------------------|
| GND               | GND                  |
| G2 (GPIO2)        | Data                 |
| G1 (GPIO1)        | Clock                |
```

## ⬇️ Instalación y Flasheo

```bash
# Construir y flashear
pio run --target upload

# Subir interfaz web
pio run --target uploadfs
```

## ⬇️ Flasheo Rápido

El firmware listo también está disponible en M5Burner

## 📄 Licencia

MIT License - ver archivo [LICENSE](LICENSE)

## 🏆 Agradecimientos

- Marius G (MGX3D) por implementar la lectura de datos sin usar convertidores de nivel lógico
- IA por la implementación del código a través de solicitudes idiotas
- Todos los involucrados en escribir las librerías utilizadas
