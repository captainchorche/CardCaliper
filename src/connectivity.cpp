#include <BleKeyboard.h>
#include <Preferences.h>
#include "LittleFS.h"
#include <ArduinoJson.h>
#include "connectivity.h"
#include "caliper.h"
#include "settings.h"
#include "display.h"

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================

BleKeyboard bleKeyboard(USER_BLE_DEVICE_NAME, USER_BLE_DEVICE_NAME, 100);

// Wireless modes
WirelessMode current_wireless_mode = WIRELESS_OFF;
Preferences preferences;
AsyncWebServer server(WEB_SERVER_PORT);
AsyncWebSocket ws(WEBSOCKET_PATH);

// ============================================================================
// WIRELESS MODE FUNCTIONS
// ============================================================================

void saveWirelessMode(WirelessMode mode) {
    preferences.begin("cardcaliper", false);
    preferences.putUChar("wireless_mode", (uint8_t)mode);
    preferences.end();
    // log("Wireless mode saved: %d", mode);
}

WirelessMode loadWirelessMode() {
    preferences.begin("cardcaliper", true);
    uint8_t mode = preferences.getUChar("wireless_mode", USER_WIRELESS_MODE_DEFAULT);
    preferences.end();
    // log("Wireless mode loaded: %d", mode);
    return (WirelessMode)mode;
}

void setWirelessMode(WirelessMode mode) {
    if (current_wireless_mode != mode) {
        // log("Switching wireless mode from %d to %d", current_wireless_mode, mode);
        saveWirelessMode(mode);
        showSwitchingScreen();
        ESP.restart();
    }
}

const char* getWirelessModeString(WirelessMode mode) {
    switch (mode) {
        case WIRELESS_OFF: return "OFF";
        case WIRELESS_BLE: return "BLE";
        case WIRELESS_AP:  return "AP";
        case WIRELESS_STA: return "STA";
        default: return "???";
    }
}

WirelessMode getCurrentWirelessMode() {
    return current_wireless_mode;
}

// ============================================================================
// MODE MANAGEMENT FUNCTIONS
// ============================================================================

void stopAllModes() {
    // log("Stopping all connection modes...");

    // Stop BLE
    if (bleKeyboard.isConnected()) {
        bleKeyboard.end();
    }

    // Stop WiFi
    if (WiFi.getMode() != WIFI_OFF) {
        server.end();
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
    }

    current_mode = MODE_NONE;
    // log("All modes stopped");
}

void startBLEMode() {
    stopAllModes();
    // log("Starting BLE mode...");

    bleKeyboard.begin();
    current_mode = MODE_BLE;

    // log("BLE mode started - device name: %s", PROJECT_NAME);
}

void startAPMode() {
    stopAllModes();
    // log("Starting AP mode...");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(currentSettings.apSSID.c_str(), currentSettings.apPassword.c_str());

    IPAddress ip = WiFi.softAPIP();
    // log("AP started - SSID: %s, IP: %s", currentSettings.apSSID.c_str(), ip.toString().c_str());

    // Setup mDNS
    if (MDNS.begin(currentSettings.mdnsHostname.c_str())) {
        // log("mDNS started - access via: http://%s.local", currentSettings.mdnsHostname.c_str());
    }

    setupWebServer();
    current_mode = MODE_AP;
}

void startSTAMode() {
    stopAllModes();
    // log("Starting STA mode...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(currentSettings.staSSID.c_str(), currentSettings.staPassword.c_str());

    // Показать экран подключения
    showConnectingScreen();

    // Ждем подключения с таймаутом из конфига
    uint32_t startTime = millis();
    uint32_t timeout = USER_STA_CONNECTION_TIMEOUT * 1000; // Конвертируем в миллисекунды

    while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < timeout) {
        delay(500);
        uint32_t elapsed = (millis() - startTime) / 1000;
        // log("Connecting... %s (%lu/%d sec)", currentSettings.staSSID.c_str(), elapsed, USER_STA_CONNECTION_TIMEOUT);
    }

    if (WiFi.status() == WL_CONNECTED) {
        IPAddress ip = WiFi.localIP();
        // log("STA connected - SSID: %s, IP: %s", currentSettings.staSSID.c_str(), ip.toString().c_str());

        // Настройка mDNS
        if (MDNS.begin(currentSettings.mdnsHostname.c_str())) {
            // log("mDNS started - access via: http://%s.local", currentSettings.mdnsHostname.c_str());
        }

        setupWebServer();
        current_mode = MODE_STA;
    } else {
        // log("Failed to connect to WiFi %s - falling back to AP mode", currentSettings.staSSID.c_str());
        // Fallback в AP режим через полный цикл переключения
        saveWirelessMode(WIRELESS_AP);
        showSwitchingScreen();
        ESP.restart();
    }
}

// ============================================================================
// BLE ФУНКЦИИ
// ============================================================================

void sendMeasurementBLE(int32_t microns) {
    // log("=== BLE SEND ===");
    WirelessMode mode = getCurrentWirelessMode();
    // log("BLE mode: %s", (mode == WIRELESS_BLE) ? "YES" : "NO");
    // log("BLE connected: %s", bleKeyboard.isConnected() ? "YES" : "NO");

    if (mode != WIRELESS_BLE) {
        // log("BLE mode is not active");
        return;
    }

    if (!bleKeyboard.isConnected()) {
        // log("BLE not connected to computer");
        return;
    }

    // Конвертируем в мм с 2 знаками после запятой (как на дисплее)
    float mm = microns / 1000.0;
    char measurement[32];
    sprintf(measurement, "%.2f", mm);

    // log("Sending measurement: '%s'", measurement);

    // Отправляем каждый символ через numpad клавиши (без блокирующих задержек)
    for (int i = 0; measurement[i] != '\0'; i++) {
        char c = measurement[i];

        if (c >= '0' && c <= '9') {
            // Цифры через numpad - используем правильные константы
            uint8_t numpadKey;
            switch (c) {
                case '0': numpadKey = KEY_NUM_0; break;  // 0xEA
                case '1': numpadKey = KEY_NUM_1; break;  // 0xE1
                case '2': numpadKey = KEY_NUM_2; break;  // 0xE2
                case '3': numpadKey = KEY_NUM_3; break;  // 0xE3
                case '4': numpadKey = KEY_NUM_4; break;  // 0xE4
                case '5': numpadKey = KEY_NUM_5; break;  // 0xE5
                case '6': numpadKey = KEY_NUM_6; break;  // 0xE6
                case '7': numpadKey = KEY_NUM_7; break;  // 0xE7
                case '8': numpadKey = KEY_NUM_8; break;  // 0xE8
                case '9': numpadKey = KEY_NUM_9; break;  // 0xE9
                default: numpadKey = KEY_NUM_0; break;
            }
            bleKeyboard.write(numpadKey);
            // log("Sent numpad digit: %c (key: 0x%02X)", c, numpadKey);
        } else if (c == '.') {
            // Точка через numpad - всегда точка независимо от раскладки
            bleKeyboard.write(KEY_NUM_PERIOD);  // 0xEB
            // log("Sent numpad period (key: 0x%02X)", KEY_NUM_PERIOD);
        }

        // Пользовательская задержка между символами (если настроена)
        if (USER_BLE_KEY_DELAY > 0) {
            delay(USER_BLE_KEY_DELAY);
        }
    }

    // log("BLE send completed");
}

// ============================================================================
// WEB SERVER FUNCTIONS
// ============================================================================

void setupWebServer() {
    // Initialize LittleFS
    if (!LittleFS.begin()) {
        // log("LittleFS mount failed");
        return;
    }
    // log("LittleFS mounted successfully");

    // WebSocket обработчик
    ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            // log("WebSocket client connected: %u", client->id());
        } else if (type == WS_EVT_DISCONNECT) {
            // log("WebSocket client disconnected: %u", client->id());
        }
    });

    server.addHandler(&ws);

    // API endpoints для настроек
    server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
        String json = settingsToJson(currentSettings);
        request->send(200, "application/json", json);
    });

    server.on("/api/settings", HTTP_POST, [](AsyncWebServerRequest *request) {
        // Обработка будет в onBody callback
    }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        // Обработка POST данных
        if (index == 0) {
            // Первый фрагмент данных
            request->_tempObject = malloc(total + 1);
            if (request->_tempObject == NULL) {
                request->send(500, "application/json", "{\"error\":\"Memory allocation failed\"}");
                return;
            }
        }

        if (request->_tempObject != NULL) {
            memcpy((uint8_t*)request->_tempObject + index, data, len);

            if (index + len == total) {
                // Все данные получены
                ((char*)request->_tempObject)[total] = '\0';
                String jsonData = String((char*)request->_tempObject);
                free(request->_tempObject);
                request->_tempObject = NULL;

                // Парсинг и сохранение настроек
                DeviceSettings newSettings;
                if (jsonToSettings(jsonData, newSettings)) {
                    if (saveDeviceSettings(newSettings)) {
                        currentSettings = newSettings;
                        request->send(200, "application/json", "{\"success\":true}");
                        // log("Settings updated successfully");
                    } else {
                        request->send(500, "application/json", "{\"error\":\"Failed to save settings\"}");
                    }
                } else {
                    request->send(400, "application/json", "{\"error\":\"Invalid settings data\"}");
                }
            }
        }
    });

    // API endpoint для перезагрузки устройства
    server.on("/api/restart", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "{\"success\":true,\"message\":\"Device will restart in 2 seconds\"}");
        // log("Restart requested via API");

        // Задержка перед перезагрузкой для отправки ответа
        delay(100);
        ESP.restart();
    });

    // Обслуживание статических файлов из LittleFS
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    server.begin();
    // log("Web server started");
}

void sendWebSocketUpdate(int32_t microns) {
    WirelessMode mode = getCurrentWirelessMode();
    if ((mode == WIRELESS_AP || mode == WIRELESS_STA) && ws.count() > 0) {
        float mm = microns / 1000.0;

        // Простая строка вместо JSON для экономии памяти
        char message[32];
        sprintf(message, "{\"measurement\":\"%.2f\"}", mm);

        ws.textAll(message);
    }
}

// ============================================================================
// CONNECTIVITY INITIALIZATION
// ============================================================================

void initConnectivity() {
    // log("Initializing connectivity...");

    // Загрузка сохраненного режима
    current_wireless_mode = loadWirelessMode();
    // log("Current wireless mode: %s", getWirelessModeString(current_wireless_mode));

    // Остановка всех режимов
    stopAllModes();

    // Запуск выбранного режима
    switch (current_wireless_mode) {
        case WIRELESS_BLE:
            startBLEMode();
            break;
        case WIRELESS_AP:
            startAPMode();
            break;
        case WIRELESS_STA:
            startSTAMode();
            break;
        case WIRELESS_OFF:
        default:
            // log("Wireless OFF mode - no connectivity started");
            break;
    }

    // log("Connectivity initialized in %s mode", getWirelessModeString(current_wireless_mode));
}
