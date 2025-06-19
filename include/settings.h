#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "LittleFS.h"

// ============================================================================
// DEVICE SETTINGS
// ============================================================================

struct DeviceSettings {
    float caliperMultiplier;
    String apSSID;
    String apPassword;
    String staSSID;
    String staPassword;
    String mdnsHostname;
};

// ============================================================================
// SETTINGS MANAGEMENT FUNCTIONS
// ============================================================================

// Initialize settings system
bool initSettings();

// Load settings from LittleFS
bool loadDeviceSettings(DeviceSettings& settings);

// Save settings to LittleFS
bool saveDeviceSettings(const DeviceSettings& settings);

// Get default settings
DeviceSettings getDefaultSettings();

// Validate settings
bool validateSettings(const DeviceSettings& settings);

// Convert settings to JSON
String settingsToJson(const DeviceSettings& settings);

// Convert JSON to settings
bool jsonToSettings(const String& json, DeviceSettings& settings);

// Global settings
extern DeviceSettings currentSettings;

#endif // SETTINGS_H
