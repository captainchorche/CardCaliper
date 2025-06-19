#include "keyboard.h"
#include "caliper.h"
#include "connectivity.h"
#include "connectivity.h"

// ============================================================================
// ФУНКЦИИ ОБРАБОТКИ КЛАВИАТУРЫ
// ============================================================================

void handleKeyboard() {
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
            
            // Функциональные клавиши (Fn+) удалены
            if (status.fn) {
                // Fn клавиши отключены
            } else {
                // Обычные клавиши
                for (auto i : status.word) {
                    char key = i;
                    
                    if (key == '1') {
                        // Клавиша 1 - BLE режим
                        // log("Switching to BLE mode...");
                        setWirelessMode(WIRELESS_BLE);
                    } else if (key == '2') {
                        // Клавиша 2 - AP режим
                        // log("Switching to AP mode...");
                        setWirelessMode(WIRELESS_AP);
                    } else if (key == '3') {
                        // Клавиша 3 - STA режим
                        // log("Switching to STA mode...");
                        setWirelessMode(WIRELESS_STA);
                    } else if (key == '4') {
                        // Клавиша 4 - OFF режим (беспроводная связь выключена)
                        // log("Switching to OFF mode...");
                        setWirelessMode(WIRELESS_OFF);
                    } else {
                        // log("Key pressed: '%c'", key);
                    }
                }

                // Специальные клавиши
                if (status.enter) {
                    // Enter - отправить измерение по BLE (только в BLE режиме)
                    if (getCurrentWirelessMode() == WIRELESS_BLE) {
                        if (current_reading.microns != MIN_RANGE) {
                            sendMeasurementBLE(current_reading.microns);
                        } else {
                            // log("No measurement to send");
                        }
                    }
                }
            }
        }
    }
}

// Отладочные Serial команды удалены
