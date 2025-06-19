#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "config.h"

// BLE Keyboard forward declaration
class BleKeyboard;

// ============================================================================
// WIRELESS MODE FUNCTIONS
// ============================================================================

// Save mode to memory
void saveWirelessMode(WirelessMode mode);

// Load mode from memory
WirelessMode loadWirelessMode();

// Set new mode (with restart)
void setWirelessMode(WirelessMode mode);

// Get string representation of mode
const char* getWirelessModeString(WirelessMode mode);

// Get current mode
WirelessMode getCurrentWirelessMode();

// Initialize connectivity
void initConnectivity();

// ============================================================================
// MODE MANAGEMENT FUNCTIONS
// ============================================================================

// Stop all modes
void stopAllModes();

// Start BLE mode
void startBLEMode();

// Start AP mode
void startAPMode();

// Start STA mode
void startSTAMode();

// ============================================================================
// BLE FUNCTIONS
// ============================================================================

// Send measurement via BLE HID
void sendMeasurementBLE(int32_t microns);

// ============================================================================
// WEB SERVER FUNCTIONS
// ============================================================================

// Setup web server and PWA
void setupWebServer();

// Send update via WebSocket
void sendWebSocketUpdate(int32_t microns);

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================

extern BleKeyboard bleKeyboard;
extern AsyncWebServer server;
extern AsyncWebSocket ws;

#endif // CONNECTIVITY_H
