# CardCaliper - Indicatore per Calibri

**🌍 Lingue:** [English](README.md) | [Русский](README_RU.md) | [Italiano](README_IT.md) | [Español](README_ES.md)

![M5Stack Cardputer](https://img.shields.io/badge/Hardware-M5Stack%20Cardputer-blue)
![ESP32-S3](https://img.shields.io/badge/MCU-ESP32--S3-green)
![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-brightgreen)

Un indicatore digitale per calibri (o altri strumenti di misurazione) con un'uscita SPC basata su M5Stack Cardputer. Il progetto fornisce la lettura delle misurazioni con la possibilità di trasmettere dati tramite BLE, WiFi e un'interfaccia web.

Questo progetto è parzialmente basato sul progetto [EspDRO](https://github.com/MGX3D/EspDRO) di Marius G MGX3D.

## 🎯 Caratteristiche Principali

### 🔗 Modalità Operative
- **Pulsante 1** - **Tastiera BLE**: emula una tastiera Bluetooth per l'input diretto (premere **invio(ok)** per inviare i dati al campo di input)
- **Pulsante 2** - **WiFi AP**: crea il proprio punto di accesso
- **Pulsante 3** - **WiFi STA**: si connette a una rete domestica (passa automaticamente alla modalità AP se la connessione fallisce)
- **Pulsante 4** - **Modalità OFF**: disabilita i moduli wireless

### 🌐 Interfaccia Web
  SSID: `CardCaliper` (SSID predefinito in modalità AP)

  Password: `Caliper123` (password predefinita in modalità AP)

  URL: `http://cardcaliper.local`
- **PWA**: installa come applicazione (aggiungi alla schermata principale)
- **In tempo reale**: aggiornamenti WebSocket senza ritardi
- **Salvataggio dati**: fare clic su una misurazione o premere invio (desktop) per salvarla nella tabella
- **Copia negli appunti**: toccare o fare clic (desktop) su una misurazione per copiarla
- **Eliminazione dati**: scorrere su una misurazione nella tabella o utilizzare il pulsante con l'emoji del cestino (elimina tutte le misurazioni)
- **Esportazione CSV**: scarica le misurazioni salvate in formato tabella

  Pagina delle impostazioni (può essere modificata anche in user_config.h):
- **WiFi STA**: SSID e password della rete domestica
- **WiFi AP**: impostazioni del punto di accesso
- **Moltiplicatore di calibrazione**: correzione per calibri diversi
- **Nome mDNS**: nome del dispositivo sulla rete

## 🔧 Assemblaggio
### Requisiti
- M5Stack Cardputer
- Calibro digitale con uscita SPC
- PlatformIO IDE o PlatformIO Core
- Fili di collegamento
- In assenza di un connettore HY2.0-4P di riserva (porta grove) e di una stampante 3D,
 ho saldato dei connettori a pin DuPont. Un connettore a 3 pin si adatta perfettamente al connettore SPC del calibro, e un connettore a 4 pin sostituisce la porta grove del Cardputer.
 Ma se si dispone di un connettore adatto, utilizzarlo con il [modello di connettore per calibro stampato](https://github.com/MGX3D/EspDRO/blob/master/CAD/spc_connector.stl).

### Collegamento del Calibro al Cardputer
![alt text](images/Pinout.png)

```
| Grove (Cardputer) | SPC (calibro)        |
|-------------------|----------------------|
| GND               | GND                  |
| G2 (GPIO2)        | Data                 |
| G1 (GPIO1)        | Clock                |
```

## ⬇️ Installazione e Flashing

```bash
# Compila e flasha
pio run --target upload

# Carica l'interfaccia web
pio run --target uploadfs
```
## ⬇️ Flashing Rapido

Un firmware precompilato è disponibile anche su M5Burner

## 📄 Licenza

Licenza MIT - vedere il file [LICENSE](LICENSE)

## 🏆 Ringraziamenti

- Marius G (MGX3D) per aver implementato la lettura dei dati senza utilizzare convertitori di livello logico
- IA per aver implementato il codice tramite richieste idiote
- Tutti coloro che sono stati coinvolti nella scrittura delle librerie utilizzate
