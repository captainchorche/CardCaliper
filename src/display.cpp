#include "display.h"
#include "caliper.h"
#include "connectivity.h"

// ============================================================================
// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ДИСПЛЕЯ
// ============================================================================

LGFX_Sprite sprite(&M5Cardputer.Display);
bool spriteInitialized = false;

// Переменные для контроля обновления дисплея
uint32_t lastDisplayUpdate = 0;
int32_t lastDisplayedMicrons = MIN_RANGE - 1;

// Неблокирующие таймеры
uint32_t lastMainLoopTime = 0;
uint32_t lastWebSocketTime = 0;
uint32_t lastBLESendTime = 0;

// ============================================================================
// ФУНКЦИИ ДИСПЛЕЯ
// ============================================================================

void initSprite() {
    if (!spriteInitialized) {
        sprite.createSprite(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        spriteInitialized = true;
        // log("Display sprite initialized (%dx%d)", DISPLAY_WIDTH, DISPLAY_HEIGHT);
    }
}

void updateDisplay(int32_t microns, uint32_t timestamp) {
    uint32_t now = millis();

    // Обновляем только при значительном изменении или чаще для быстрого отклика
    bool shouldUpdate = (now - lastDisplayUpdate > DISPLAY_UPDATE_MAX_INTERVAL) ||
                       (abs(microns - lastDisplayedMicrons) > DISPLAY_UPDATE_MIN_CHANGE);

    if (!shouldUpdate) return;

    lastDisplayUpdate = now;
    lastDisplayedMicrons = microns;

    // Очистка экрана с синим фоном (blueprint стиль)
    sprite.fillSprite(COLOR_BACKGROUND);

    // Оранжевая рамка (комплементарный к синему)
    sprite.drawRoundRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 4, COLOR_BORDER);
    sprite.drawRoundRect(1, 1, DISPLAY_WIDTH-2, DISPLAY_HEIGHT-2, 4, COLOR_BORDER);

    if (microns != MIN_RANGE) {
        float mm = microns / 1000.0;

        // Главное значение - белый цвет (blueprint стиль)
        sprite.setTextColor(COLOR_TEXT_WHITE);
        sprite.setTextSize(6);
        sprite.setTextDatum(middle_center);
        sprite.drawString(String(mm, 2), DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2 - 20);

        // Единицы измерения на отдельной строке
        sprite.setTextColor(COLOR_TEXT_YELLOW);
        sprite.setTextSize(3);
        sprite.setTextDatum(middle_center);
        sprite.drawString("mm", DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2 + 18);

        // Микроны в правом верхнем углу (мелким шрифтом)
        // sprite.setTextColor(COLOR_TEXT_GRAY);
        // sprite.setTextSize(1);
        // sprite.setTextDatum(top_right);
        // sprite.drawString(String(microns) + " um", DISPLAY_WIDTH-5, 5);
    } else {
        // Нет данных
        sprite.setTextColor(COLOR_TEXT_GRAY);
        sprite.setTextSize(4);
        sprite.setTextDatum(middle_center);
        sprite.drawString("No Data", DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2);
    }

    // Показать строку режимов в нижней части экрана
    WirelessMode currentMode = getCurrentWirelessMode();
    sprite.setTextDatum(bottom_center);

    // Формируем строку с режимами, активный - зеленый размер 2, остальные - серые размер 1

    // 1-BLE
    if (currentMode == WIRELESS_BLE) {
        sprite.setTextSize(2);
        sprite.setTextColor(COLOR_TEXT_GREEN);
        sprite.drawString("1-BLE", DISPLAY_WIDTH/2 - 83, DISPLAY_HEIGHT-5);
    } else {
        sprite.setTextSize(1);
        sprite.setTextColor(COLOR_TEXT_GRAY);
        sprite.drawString("1-BLE", DISPLAY_WIDTH/2 - 80, DISPLAY_HEIGHT-5);
    }

    // 2-AP
    if (currentMode == WIRELESS_AP) {
        sprite.setTextSize(2);
        sprite.setTextColor(COLOR_TEXT_GREEN);
        sprite.drawString("2-AP", DISPLAY_WIDTH/2 - 25, DISPLAY_HEIGHT-5);
    } else {
        sprite.setTextSize(1);
        sprite.setTextColor(COLOR_TEXT_GRAY);
        sprite.drawString("2-AP", DISPLAY_WIDTH/2 - 25, DISPLAY_HEIGHT-5);
    }

    // 3-STA
    if (currentMode == WIRELESS_STA) {
        sprite.setTextSize(2);
        sprite.setTextColor(COLOR_TEXT_GREEN);
        sprite.drawString("3-STA", DISPLAY_WIDTH/2 + 30, DISPLAY_HEIGHT-5);
    } else {
        sprite.setTextSize(1);
        sprite.setTextColor(COLOR_TEXT_GRAY);
        sprite.drawString("3-STA", DISPLAY_WIDTH/2 + 30, DISPLAY_HEIGHT-5);
    }

    // 4-OFF
    if (currentMode == WIRELESS_OFF) {
        sprite.setTextSize(2);
        sprite.setTextColor(COLOR_TEXT_GREEN);
        sprite.drawString("4-OFF", DISPLAY_WIDTH/2 + 85, DISPLAY_HEIGHT-5);
    } else {
        sprite.setTextSize(1);
        sprite.setTextColor(COLOR_TEXT_GRAY);
        sprite.drawString("4-OFF", DISPLAY_WIDTH/2 + 85, DISPLAY_HEIGHT-5);
    }
    
    // Показать индикатор фильтра помех в левом нижнем углу
    // if (noise_filter_enabled) {
    //     sprite.setTextDatum(bottom_left);
    //     sprite.setTextColor(COLOR_TEXT_YELLOW);
    //     sprite.drawString("FILTER", 5, DISPLAY_HEIGHT-5);
    // }

    // Отправка на дисплей
    sprite.pushSprite(0, 0);
}

void showStartupScreen() {
    if (!spriteInitialized) {
        initSprite();
    }

    // Очистка экрана с синим фоном
    sprite.fillSprite(COLOR_BACKGROUND);

    // Оранжевая рамка
    sprite.drawRoundRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 4, COLOR_BORDER);
    sprite.drawRoundRect(1, 1, DISPLAY_WIDTH-2, DISPLAY_HEIGHT-2, 4, COLOR_BORDER);

    // Заголовок
    sprite.setTextColor(COLOR_BORDER);
    sprite.setTextSize(4);
    sprite.setTextDatum(middle_center);
    sprite.drawString("CardCaliper", DISPLAY_WIDTH/2, 40);

    // Версия
    sprite.setTextColor(COLOR_TEXT_WHITE);
    sprite.setTextSize(2);
    sprite.setTextDatum(middle_center);
    sprite.drawString("v" FIRMWARE_VERSION, DISPLAY_WIDTH/2, 70);

    // Статус
    sprite.setTextColor(COLOR_TEXT_GREEN);
    sprite.setTextSize(2);
    sprite.setTextDatum(middle_center);
    sprite.drawString("Ready", DISPLAY_WIDTH/2, 95);

    // Инструкции
    sprite.setTextColor(COLOR_TEXT_WHITE);
    sprite.setTextSize(1);
    sprite.drawString("1=BLE, 2=AP, 3=STA, 4=OFF", DISPLAY_WIDTH/2, DISPLAY_HEIGHT-15);

    // Отправка на дисплей
    sprite.pushSprite(0, 0);
}

void showSwitchingScreen() {
    if (!spriteInitialized) {
        initSprite();
    }

    // Очистка экрана с синим фоном
    sprite.fillSprite(COLOR_BACKGROUND);

    // Оранжевая рамка
    sprite.drawRoundRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 4, COLOR_BORDER);
    sprite.drawRoundRect(1, 1, DISPLAY_WIDTH-2, DISPLAY_HEIGHT-2, 4, COLOR_BORDER);

    // Надпись на двух строках
    sprite.setTextColor(COLOR_TEXT_WHITE);
    sprite.setTextSize(3);
    sprite.setTextDatum(middle_center);
    sprite.drawString("switching", DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2 - 15);
    sprite.drawString("mode", DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2 + 15);

    // Отправка на дисплей
    sprite.pushSprite(0, 0);

    // Показать 1 секунду
    delay(1000);

    // Черный экран
    sprite.fillSprite(TFT_BLACK);
    sprite.pushSprite(0, 0);
}

void showConnectingScreen() {
    if (!spriteInitialized) {
        initSprite();
    }

    // Очистка экрана с синим фоном
    sprite.fillSprite(COLOR_BACKGROUND);

    // Оранжевая рамка
    sprite.drawRoundRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 4, COLOR_BORDER);
    sprite.drawRoundRect(1, 1, DISPLAY_WIDTH-2, DISPLAY_HEIGHT-2, 4, COLOR_BORDER);

    // Надпись
    sprite.setTextColor(COLOR_TEXT_WHITE);
    sprite.setTextSize(3);
    sprite.setTextDatum(middle_center);
    sprite.drawString("connecting...", DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2);

    // Отправка на дисплей
    sprite.pushSprite(0, 0);
}
