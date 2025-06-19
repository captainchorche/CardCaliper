// Main header files
#include <M5Cardputer.h>
#include "config.h"
#include "caliper.h"
#include "display.h"
#include "connectivity.h"
#include "keyboard.h"
#include "settings.h"

// ============================================================================
// DATA READING FUNCTIONS (now in main loop for stability)
// ============================================================================

// ============================================================================
// SETUP
// ============================================================================

void setup() {
    // Initialize Serial (speed from user settings)
    Serial.begin(USER_SERIAL_SPEED);
    while (!Serial && millis() < 3000) {
        delay(10);
    }
    
    // log("=== CardCaliper v%s ===", FIRMWARE_VERSION);
    // log("Starting setup...");
    
    // Initialize M5Cardputer
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    
    // log("M5Cardputer initialized");

    // Initialize settings system
    if (!initSettings()) {
        // log("Failed to initialize settings system");
        // Continue with default settings
        currentSettings = getDefaultSettings();
    }

    // Initialize display
    initSprite();
    // showStartupScreen();  // Commented out - screen works poorly and is not needed

    // Initialize caliper
    initCaliper();

    // Initialize wireless modes
    initConnectivity();

    // log("SPC reading in main loop (optimized for stability)");

    // log("=== Setup Complete ===");
    // log("Wireless modes (saved & auto-restart):");
    // log("  1: BLE Keyboard mode");
    // log("  2: WiFi AP mode (SSID: %s)", currentSettings.apSSID.c_str());
    // log("  3: WiFi STA mode (connect to %s)", currentSettings.staSSID.c_str());
    // log("  4: OFF mode (wireless disabled)");
    // log("Current mode: %s", getWirelessModeString(getCurrentWirelessMode()));
    // log("Other commands:");
    // log("  Fn+S: Stop streaming");
    // log("  Fn+R: Resume streaming");
    // log("  Fn+D: Toggle debug mode");
    // log("  Fn+T: Show statistics");
    // log("  Fn+C: Clear statistics");
    // log("  SPACE: Send measurement (BLE mode only)");
    // log("Web interface: http://%s.local", currentSettings.mdnsHostname.c_str());
    // log("Noise filter: %s (config: user_config.h)", noise_filter_enabled ? "ENABLED" : "DISABLED");
    // log("Caliper multiplier: %.1f", currentSettings.caliperMultiplier);
    // log("========================");

    // Show initial measurement screen
    updateDisplay(MIN_RANGE, millis());
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
    uint32_t currentTime = millis();

    // Main loop runs every 1ms (1000 times per second)
    if (currentTime - lastMainLoopTime >= MAIN_LOOP_INTERVAL) {
        lastMainLoopTime = currentTime;

        // Update M5Cardputer
        M5Cardputer.update();

        // Handle keyboard
        handleKeyboard();
    }

    // Read caliper data in main loop (configurable in user_config.h)
    static uint32_t lastReadTime = 0;
    if (currentTime - lastReadTime >= READ_INTERVAL) {
        lastReadTime = currentTime;

        if (stream_mode) {
            // Read data packet
            long packet = getPacket(DATA_PIN, CLOCK_PIN);
            total_packets++;

            if (packet >= 0) {
                // Process packet
                int32_t microns = getMicrons(packet);
                uint32_t timestamp = millis();

                // Apply noise filter (if enabled)
                bool shouldUpdate = true;
                if (noise_filter_enabled) {
                    if (microns >= MIN_RANGE && microns <= MAX_RANGE) {
                        shouldUpdate = isValidMeasurement(microns);
                    } else {
                        shouldUpdate = false;
                        // log("Value out of range: %ld", microns);
                    }
                }

                if (shouldUpdate) {
                    // Update current measurement
                    current_reading.microns = microns;
                    current_reading.timestamp = timestamp;

                    // Output to Serial
                    float mm = microns / 1000.0;
                    // log("CardCaliper: %.3f mm (%ld um) [0x%08lX] Valid: %lu/%lu",
                    //     mm, microns, packet, valid_packets, total_packets);

                    // Update display
                    updateDisplay(microns, timestamp);
                }

                valid_packets++;
            } else {
                error_packets++;
                if (debug_mode) {
                    // log("Error reading packet. Errors: %lu/%lu", error_packets, total_packets);
                }
            }
        }
    }
    
    // WebSocket updates every 50ms (only when changed)
    WirelessMode mode = getCurrentWirelessMode();
    if (mode == WIRELESS_AP || mode == WIRELESS_STA) {
        static int32_t lastSentValue = MIN_RANGE;

        if (currentTime - lastWebSocketTime >= WEBSOCKET_INTERVAL) {
            if (current_reading.microns != lastSentValue) {
                sendWebSocketUpdate(current_reading.microns);
                lastSentValue = current_reading.microns;
            }
            lastWebSocketTime = currentTime;
        }
    }
    
    // No delay() - maximum performance!
}
