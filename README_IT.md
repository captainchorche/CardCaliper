# CardCaliper - Indicatore per Calibri

**🌍 Lingue:** [English](README.md) | [Русский](README_RU.md) | [Italiano](README_IT.md) | [Español](README_ES.md)

![M5Stack Cardputer](https://img.shields.io/badge/Hardware-M5Stack%20Cardputer-blue)
![ESP32-S3](https://img.shields.io/badge/MCU-ESP32--S3-green)
![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-brightgreen)

Un indicatore digitale per calibri (o altri strumenti di misura) con uscita SPC basato su M5Stack Cardputer. Il progetto fornisce la lettura delle misurazioni con la possibilità di trasmettere i dati tramite BLE, WiFi e un'interfaccia web.

Questo progetto è parzialmente basato sul progetto [EspDRO](https://github.com/MGX3D/EspDRO) di Marius G MGX3D.

## 🎯 Caratteristiche Principali

[YouTubeLink](https://www.youtube.com/shorts/dYnquSnqz-o)

![alt text](images/CardCaliperGIF.gif)

### 🔗 Modalità Operative
- **Pulsante 1** - **Tastiera BLE**: emula una tastiera Bluetooth per l'input diretto (premere il pulsante **enter(ok)** per inviare i dati al campo di input).
- **Pulsante 2** - **WiFi AP**: crea un proprio punto di accesso.
- **Pulsante 3** - **WiFi STA**: si connette a una rete domestica (in caso di connessione fallita, passa automaticamente alla modalità AP).
- **Pulsante 4** - **Modalità OFF**: disabilita i moduli wireless.

### 🌐 Interfaccia Web
  SSID: `CardCaliper` (SSID predefinito in modalità AP)

  Password: `Caliper123` (password predefinita in modalità AP)

  URL: `http://cardcaliper.local`
- **PWA**: installa come applicazione (aggiungi alla schermata principale).
- **Tempo reale**: aggiornamenti tramite WebSocket senza ritardi.
- **Salvataggio Dati**: clicca sulla misurazione o premi Invio (su desktop) per salvarla nella tabella.
- **Copia negli Appunti**: tocca o clicca (su desktop) su una misurazione per copiarla.
- **Eliminazione Dati**: scorri su una misurazione nella tabella o usa il pulsante con l'emoji del cestino per eliminare tutte le misurazioni.
- **Esportazione CSV**: scarica le misurazioni salvate in formato tabella.

  Pagina delle impostazioni (modificabile anche in `user_config.h`):
- **WiFi STA**: SSID e password della rete domestica.
- **WiFi AP**: impostazioni del punto di accesso.
- **Moltiplicatore di Calibrazione**: fattore di correzione per diversi calibri.
- **Nome mDNS**: nome del dispositivo nella rete.

## 🔧 Assemblaggio
### Requisiti
- M5Stack Cardputer
- Calibro digitale con uscita SPC
- PlatformIO IDE o PlatformIO Core
- Fili di collegamento
- In assenza di un connettore HY2.0-4P (porta Grove) e di una stampante 3D, ho saldato dei connettori pin DuPont. Un connettore a 3 pin si adatta perfettamente al connettore SPC del calibro, e uno a 4 pin sostituisce la porta Grove del Cardputer. Se invece disponi di un connettore adatto, usalo insieme al [modello stampato del connettore per calibro](https://github.com/MGX3D/EspDRO/blob/master/CAD/spc_connector.stl).

### Collegamento del Calibro al Cardputer
![Pinout](images/Pinout.png)

```
| Grove (Cardputer) | SPC (calibro)        |
|-------------------|----------------------|
| GND               | GND                  |
| G2 (GPIO2)        | Data                 |
| G1 (GPIO1)        | Clock                |
```

## ⬇️ Installazione e Flashing

```bash
# Compila e carica il firmware
pio run --target upload

# Carica l'interfaccia web
pio run --target uploadfs
```
## ⬇️ Flashing Rapido

Un firmware precompilato è disponibile anche su M5Burner.

## 📄 Licenza

Licenza MIT - vedi il file [LICENSE](LICENSE).

## 🏆 Ringraziamenti

- Marius G (MGX3D) per aver implementato la lettura dei dati senza utilizzare convertitori di livello logico.
- L'IA per aver implementato il codice tramite richieste bizzarre.
- Tutti coloro che hanno contribuito alla scrittura delle librerie utilizzate.
