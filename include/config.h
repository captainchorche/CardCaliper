#ifndef CONFIG_H
#define CONFIG_H

#include "user_config.h"

// ============================================================================
// PROJECT CONFIGURATION
// ============================================================================

// Firmware version
#define FIRMWARE_VERSION "1.0.1"
#define PROJECT_NAME "CardCaliper"

// GPIO Configuration
#define DATA_PIN 2      // SPC Data pin (Grove G2)
#define CLOCK_PIN 1     // SPC Clock pin (Grove G1)

// Wireless Modes
enum WirelessMode {
    WIRELESS_OFF = 0,
    WIRELESS_BLE = 1,
    WIRELESS_AP = 2,
    WIRELESS_STA = 3
};

// ADC Configuration
#define ADC_THRESHOLD 1800
#define ADC_RESOLUTION 12
#define ADC_ATTENUATION ADC_6db

// SPC Protocol Configuration
#define PACKET_BITS 24
#define BIT_READ_TIMEOUT 100  // milliseconds
#define MIN_RANGE -(1<<20)
#define MAX_RANGE (1<<20)

// WiFi Configuration (from user settings)
#define AP_SSID USER_AP_SSID
#define AP_PASSWORD USER_AP_PASSWORD
#define STA_SSID USER_STA_SSID
#define STA_PASSWORD USER_STA_PASSWORD
#define MDNS_HOSTNAME USER_MDNS_HOSTNAME

// Web Server Configuration
#define WEB_SERVER_PORT 80
#define WEBSOCKET_PATH "/ws"

// Display Configuration
#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 135
#define DISPLAY_UPDATE_MIN_CHANGE USER_DISPLAY_MIN_CHANGE  // microns
#define DISPLAY_UPDATE_MAX_INTERVAL USER_DISPLAY_MAX_INTERVAL  // ms

// Timing Configuration (from user settings)
#define MAIN_LOOP_INTERVAL USER_MAIN_LOOP_INTERVAL     // ms
#define WEBSOCKET_INTERVAL USER_WEBSOCKET_INTERVAL    // ms
#define READ_INTERVAL USER_READ_INTERVAL              // ms
#define NOISE_FILTER_THRESHOLD USER_NOISE_FILTER_THRESHOLD  // microns

// Colors (RGB565) - from user settings
#define COLOR_BACKGROUND USER_COLOR_BACKGROUND    // Blue
#define COLOR_BORDER USER_COLOR_BORDER        // Orange
#define COLOR_TEXT_WHITE USER_COLOR_TEXT_WHITE    // White
#define COLOR_TEXT_YELLOW USER_COLOR_TEXT_YELLOW   // Yellow
#define COLOR_TEXT_GREEN USER_COLOR_TEXT_GREEN    // Green
#define COLOR_TEXT_RED USER_COLOR_TEXT_RED      // Red
#define COLOR_TEXT_GRAY USER_COLOR_TEXT_GRAY     // Gray

#endif // CONFIG_H
