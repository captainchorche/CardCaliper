# CardCaliper - Индикатор для штангенциркулей

**🌍 Языки:** [English](readme.md) | [Русский](README_RU.md) | [Italiano](README_IT.md) | [Español](README_ES.md)

![M5Stack Cardputer](https://img.shields.io/badge/Hardware-M5Stack%20Cardputer-blue)
![ESP32-S3](https://img.shields.io/badge/MCU-ESP32--S3-green)
![PlatformIO](https://img.shields.io/badge/Platform-PlatformIO-orange)
![Version](https://img.shields.io/badge/Version-2.0.0-brightgreen)

Цифровой индикатор для штангенциркулей (или других измерительных приборов) с SPC выходом на базе M5Stack Cardputer. Проект предоставляет считывание измерений с возможностью передачи данных через BLE, WiFi и веб-интерфейс.

Этот проект частично основан на проекте [EspDRO](https://github.com/MGX3D/EspDRO) Marius G MGX3D.

## 🎯 Основные возможности

[YouTubeLink](https://www.youtube.com/shorts/dYnquSnqz-o)

![alt text](images/CardCaliperGIF.gif)
### 🔗 Режимы работы
- кнопка **1** - **BLE Keyboard**: эмуляция bluetooth-клавиатуры для прямого ввода, (кнопка **enter(ok)** для отправки данных в поле ввода)
- кнопка **2** - **WiFi AP**: создание собственной точки доступа
- кнопка **3** - **WiFi STA**: подключение к домашней сети (при неудачной попытке подключения автоматический переход в режим AP)
- кнопка **4** - **Режим OFF**: отключение беспроводных модулей  

### 🌐 Веб-интерфейс
  SSID: `CardCaliper` (defaultSSID AP mode)

  Пароль: `Caliper123` (defaultPASSWORD AP mode)

  URL: `http://cardcaliper.local`
- **PWA**: установка как приложение (добавить на экран "домой")
- **Риалтайм**: WebSocket обновления без задержек
- **Сохранение данных**: клик по измерению или enter (desktop) для сохранения в таблицу
- **Копирование в клипборд**: тап или клик (desktop) по измерению для сохранения в таблицу
- **Удаление данных**: свайп по измерению в таблице или кнопкой с эмодзи корзины (удаление всех измерений)
- **Экспорт CSV**: загрузка сохраненных измерений в формате таблицы

  Страница настроек (также можно изменить в user_config.h):
- **WiFi STA**: SSID и пароль домашней сети
- **WiFi AP**: настройки точки доступа
- **Множитель калибровки**: коррекция для разных штангенциркулей
- **mDNS имя**: имя устройства в сети

## 🔧 Сборка
### Требования
- M5Stack Cardputer
- Цифровой штангенциркуль с SPC выходом
- PlatformIO IDE или PlatformIO Core
- Соединительные провода
- В отсутствии лишнего разъема HY2.0-4P(grove port) и 3D принтера,
 я припаял штырьковые разъемы(DuPont). 3pin идеально подходит в SPC разъем штангенциркуля, a 4pin вместо grove порта cardputer.
 Но если у вас есть подходящий разъем, то используйте его вместе с [напечатанной моделью разъема под штангенциркуль](https://github.com/MGX3D/EspDRO/blob/master/CAD/spc_connector.stl).

### Подключение штангенциркуля к Cardputer
![alt text](images/Pinout.png)

```
| Grove (Cardputer) | SPC (штангенциркуль) |
|-------------------|----------------------|
| GND               | GND                  |
| G2 (GPIO2)        | Data                 |
| G1 (GPIO1)        | Clock                |
```

## ⬇️ Установка и прошивка

```bash
# Собрать и прошить
pio run --target upload

# Загрузить веб-интерфейс
pio run --target uploadfs
```
## ⬇️ Быстрая прошивка

Готовая прошивка также доступна на M5Burner 

## 📄 Лицензия

MIT License - см. файл [LICENSE](LICENSE)

## 🏆 Благодарности

- Marius G (MGX3D) за то что реализовал считываение данных без использования преобразователей логических уровней
- ИИ за реализацию кода посредством идиотских запросов
- Всем причастным к написанию использованных библиотек
