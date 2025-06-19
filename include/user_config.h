#ifndef USER_CONFIG_H
#define USER_CONFIG_H

// ============================================================================
// CARDCALIPER USER SETTINGS
// ============================================================================
// This file contains settings that users can modify
// to suit their needs without changing the main code
// ============================================================================

// ============================================================================
// CALIPER SETTINGS
// ============================================================================

// Multiplier for converting raw data to microns
// Different calipers may have different multipliers:
// - If values are 10 times smaller: use 10.0
// - If values are correct: use 1.0
// - If values are 2 times larger: use 0.5
#define CALIPER_MULTIPLIER 10.0

// Sign inversion (if positive and negative values are swapped)
// true - invert sign, false - do not invert
#define CALIPER_INVERT_SIGN false

// Zero offset (if caliper shows offset when jaws are closed)
// Value in microns that will be subtracted from all measurements
#define CALIPER_ZERO_OFFSET 0

// ============================================================================
// WIFI SETTINGS
// ============================================================================

// Access Point settings (AP mode)
#define USER_AP_SSID "CardCaliper"
#define USER_AP_PASSWORD "Caliper123"

// Home network connection settings (STA mode)
#define USER_STA_SSID "yourSSID"
#define USER_STA_PASSWORD "yourPassword"

// Device name for mDNS (will be available as http://cardcaliper.local)
#define USER_MDNS_HOSTNAME "cardcaliper"

// ============================================================================
// DISPLAY SETTINGS
// ============================================================================

// Minimum change to update display (in microns)
// Lower value = more sensitive display, but more flickering
#define USER_DISPLAY_MIN_CHANGE 10

// Maximum display update interval (in milliseconds)
// Lower value = faster updates, but more CPU load
#define USER_DISPLAY_MAX_INTERVAL 200

// ============================================================================
// PERFORMANCE SETTINGS
// ============================================================================

// Caliper data reading interval (in milliseconds)
// Lower value = faster response, but more CPU load
#define USER_READ_INTERVAL 50

// WebSocket update interval (in milliseconds)
// Lower value = smoother updates in browser
#define USER_WEBSOCKET_INTERVAL 50

// Main loop interval (in milliseconds)
// Recommended to keep at 1 for maximum responsiveness
#define USER_MAIN_LOOP_INTERVAL 1

// ============================================================================
// NOISE FILTER SETTINGS
// ============================================================================

// Enable noise filter by default
// true = enable, false = disable
#define USER_NOISE_FILTER_DEFAULT true

// Noise filter threshold (in microns)
// Values differing by more than this threshold will be filtered
// For calipers with WiFi/BLE interference, 10000 (10mm) is recommended
#define USER_NOISE_FILTER_THRESHOLD 10000

// ============================================================================
// WIRELESS MODE SETTINGS
// ============================================================================

// Default wireless mode (after firmware update)
// 0 = OFF (disabled), 1 = BLE, 2 = AP, 3 = STA
#define USER_WIRELESS_MODE_DEFAULT 0

// WiFi connection timeout in STA mode (seconds)
#define USER_STA_CONNECTION_TIMEOUT 10

// ============================================================================
// BLE SETTINGS
// ============================================================================

// BLE device name (will be visible when connecting)
#define USER_BLE_DEVICE_NAME "CardCaliper"

// Delay between sending characters via BLE (in milliseconds)
// 0 = no delay (maximum speed)
// 20-50 = for compatibility with slow devices
#define USER_BLE_KEY_DELAY 0

// Enable debug messages by default
// true = enable, false = disable
#define USER_DEBUG_DEFAULT false

// Enable stream mode by default
// true = enable, false = disable
#define USER_STREAM_DEFAULT true

// Serial port speed for debugging
#define USER_SERIAL_SPEED 115200

// ============================================================================
// COLOR SCHEME (RGB565)
// ============================================================================

// Main interface colors (can be changed to your preference)
#define USER_COLOR_BACKGROUND 0x001F    // Dark blue
#define USER_COLOR_BORDER 0xFD20        // Orange
#define USER_COLOR_TEXT_WHITE 0xFFFF    // White
#define USER_COLOR_TEXT_YELLOW 0xFFE0   // Yellow
#define USER_COLOR_TEXT_GREEN 0x07E0    // Green
#define USER_COLOR_TEXT_RED 0xF800      // Red
#define USER_COLOR_TEXT_GRAY 0x8410     // Gray

// ============================================================================
// ADDITIONAL SETTINGS
// ============================================================================

// Bit reading timeout (in milliseconds)
#define USER_BIT_READ_TIMEOUT 100

// Number of WiFi reconnection attempts
#define USER_WIFI_RETRY_COUNT 20

// Delay between WiFi reconnection attempts (in milliseconds)
#define USER_WIFI_RETRY_DELAY 500

#endif // USER_CONFIG_H
