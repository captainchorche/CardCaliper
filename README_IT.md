# CardCaliper - Lettore Digitale per Calibri

**🌍 Lingue:** [English](README.md) | [Русский](README_RU.md) | [Italiano](README_IT.md) | [Español](README_ES.md)

![M5Stack Cardputer](https://img.shields.io/badge/Hardware-M5Stack%20Cardputer-blue)
![ESP32-S3](https://img.shields.io/badge/MCU-ESP32--S3-green)
![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-brightgreen)

Lettore digitale per calibri (o altri strumenti di misura) con uscita SPC basato su M5Stack Cardputer. Il progetto fornisce la lettura delle misurazioni con capacità di trasmissione dati tramite BLE, WiFi e interfaccia web.

Questo progetto è parzialmente basato sul progetto [EspDRO](https://github.com/MGX3D/EspDRO) di Marius G MGX3D.

## 🎯 Caratteristiche Principali

### 🔗 Modalità Operative
- **Pulsante 1** - **BLE Keyboard**: Emulazione tastiera Bluetooth per input diretto (premere **enter(ok)** per inviare dati al campo di input)
- **Pulsante 2** - **WiFi AP**: Creare proprio punto di accesso
- **Pulsante 3** - **WiFi STA**: Connessione alla rete domestica (passa automaticamente alla modalità AP se la connessione fallisce)
- **Pulsante 4** - **Modalità OFF**: Disabilitare moduli wireless

### 🌐 Interfaccia Web
  SSID: `CardCaliper` (SSID predefinito modalità AP)

  Password: `Caliper123` (PASSWORD predefinita modalità AP)

  URL: `http://cardcaliper.local`
- **PWA**: Installare come app (aggiungi alla schermata home)
- **Tempo reale**: Aggiornamenti WebSocket senza ritardi
- **Salvataggio dati**: Cliccare sulla misurazione o enter (desktop) per salvare nella tabella
- **Copia negli appunti**: Toccare o cliccare sulla misurazione (desktop) per copiare negli appunti
- **Eliminazione dati**: Scorrere la misurazione nella tabella o pulsante emoji cestino (elimina tutte le misurazioni)
- **Esportazione CSV**: Scaricare le misurazioni salvate in formato tabella

  Pagina impostazioni (può anche essere modificata in user_config.h):
- **WiFi STA**: SSID e password della rete domestica
- **WiFi AP**: Impostazioni punto di accesso
- **Moltiplicatore calibrazione**: Correzione per diversi calibri
- **Nome mDNS**: Nome dispositivo sulla rete

## 🔧 Costruzione

### Requisiti
- M5Stack Cardputer
- Calibro digitale con uscita SPC
- PlatformIO IDE o PlatformIO Core
- Cavi di collegamento
- In assenza di connettore HY2.0-4P di ricambio (porta grove) e stampante 3D,
 ho saldato header pin (DuPont). Il 3-pin si adatta perfettamente al connettore SPC del calibro, e il 4-pin al posto della porta grove su cardputer.
 Ma se hai un connettore adatto, usalo con il [modello connettore calibro stampato](https://github.com/MGX3D/EspDRO/blob/master/CAD/spc_connector.stl).

### Collegamento Calibro a Cardputer

```
| Grove (Cardputer) | SPC (Calibro)        |
|-------------------|----------------------|
| GND               | GND                  |
| G2 (GPIO2)        | Data                 |
| G1 (GPIO1)        | Clock                |
```

## ⬇️ Installazione e Flashing

```bash
# Costruire e flashare
pio run --target upload

# Caricare interfaccia web
pio run --target uploadfs
```

## ⬇️ Flashing Rapido

Il firmware pronto è disponibile anche su M5Burner

## 📄 Licenza

MIT License - vedere file [LICENSE](LICENSE)

## 🏆 Riconoscimenti

- Marius G (MGX3D) per aver implementato la lettura dati senza usare convertitori di livello logico
- AI per l'implementazione del codice attraverso richieste idiote
- Tutti coloro coinvolti nella scrittura delle librerie utilizzate
