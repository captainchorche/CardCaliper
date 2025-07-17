
# CardCaliper - Indicatore per calibri

**🌍 Lingue:** [English](README.md) | [Русский](README_RU.md) | [Italiano](README_IT.md) | [Español](README_ES.md)

![M5Stack Cardputer](https://img.shields.io/badge/Hardware-M5Stack%20Cardputer-blue)
![ESP32-S3](https://img.shields.io/badge/MCU-ESP32--S3-green)
![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-brightgreen)

Indicatore digitale per calibri (o altri strumenti di misura) con uscita SPC basato su M5Stack Cardputer. Il progetto consente la lettura delle misurazioni con la possibilità di trasferire i dati tramite BLE, WiFi e interfaccia web.

Questo progetto è parzialmente basato su [EspDRO](https://github.com/MGX3D/EspDRO) di Marius G MGX3D.

## 🎯 Caratteristiche principali

[YouTubeLink](https://www.youtube.com/shorts/dYnquSnqz-o)

![alt text](images/CardCaliperGIF.gif)
### 🔗 Modalità operative
- pulsante **1** - **BLE Keyboard**: emulazione tastiera bluetooth per input diretto, (pulsante **enter(ok)** per inviare i dati al campo di input)
- pulsante **2** - **WiFi AP**: crea un proprio punto di accesso
- pulsante **3** - **WiFi STA**: connessione alla rete domestica (in caso di fallimento della connessione, passa automaticamente alla modalità AP)
- pulsante **4** - **Modalità OFF**: disattiva i moduli wireless  

### 🌐 Interfaccia web
  SSID: `CardCaliper` (defaultSSID modalità AP)

  Password: `Caliper123` (defaultPASSWORD modalità AP)

  URL: `http://cardcaliper.local`
- **PWA**: installa come app (aggiungi alla schermata home)
- **Realtime**: aggiornamenti WebSocket senza ritardi
- **Salvataggio dati**: clic sulla misurazione o enter (desktop) per salvare nella tabella
- **Copia negli appunti**: tocca o clicca (desktop) sulla misurazione per salvare nella tabella
- **Elimina dati**: swipe sulla misurazione nella tabella o usa il pulsante emoji cestino (elimina tutte le misurazioni)
- **Esporta CSV**: scarica le misurazioni salvate in formato tabella

  Pagina delle impostazioni (può essere modificata anche in user_config.h):
- **WiFi STA**: SSID e password della rete domestica
- **WiFi AP**: impostazioni punto di accesso
- **Moltiplicatore di calibrazione**: correzione per diversi calibri
- **Nome mDNS**: nome del dispositivo in rete

## 🔧 Assemblaggio
### Requisiti
- M5Stack Cardputer
- Calibro digitale con uscita SPC
- PlatformIO IDE o PlatformIO Core
- Fili di collegamento
- Se non hai un connettore HY2.0-4P (porta grove) e una stampante 3D,
  ho saldato pin DuPont. Il 3 pin si adatta perfettamente al connettore SPC del calibro, e il 4 pin invece della porta grove del cardputer.
  Ma se hai un connettore adatto, usalo insieme al [modello stampato del connettore per calibro](https://github.com/MGX3D/EspDRO/blob/master/CAD/spc_connector.stl).

### Collegamento del calibro al Cardputer
![alt text](images/Pinout.png)

```
| Grove (Cardputer) | SPC (calibro) |
|-------------------|---------------|
| GND               | GND           |
| G2 (GPIO2)        | Data          |
| G1 (GPIO1)        | Clock         |
```

## ⬇️ Installazione e flash

```bash
# Compila e flasha
pio run --target upload

# Carica interfaccia web
pio run --target uploadfs
```
## ⬇️ Flash veloce

Il firmware pronto è disponibile anche su M5Burner

## 📄 Licenza

Licenza MIT - vedi [LICENSE](LICENSE)

## 🏆 Ringraziamenti

- Marius G (MGX3D) per aver implementato la lettura dei dati senza convertitori di livello logico
- IA per l'implementazione del codice tramite richieste idiote
- Tutti coloro che hanno contribuito alla scrittura delle librerie utilizzate
