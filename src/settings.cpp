#include "settings.h"
#include "user_config.h"
#include "caliper.h"

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

DeviceSettings currentSettings;
const char* SETTINGS_FILE = "/settings.json";

// ============================================================================
// SETTINGS MANAGEMENT FUNCTIONS
// ============================================================================

bool initSettings() {
    // log("Initializing settings system...");

    // Initialize LittleFS if not already initialized
    if (!LittleFS.begin()) {
        // log("Failed to mount LittleFS");
        return false;
    }

    // Load settings
    if (!loadDeviceSettings(currentSettings)) {
        // log("Failed to load settings, using defaults");
        currentSettings = getDefaultSettings();

        // Save default settings
        if (!saveDeviceSettings(currentSettings)) {
            // log("Failed to save default settings");
            return false;
        }
    }

    // log("Settings initialized successfully");
    return true;
}

DeviceSettings getDefaultSettings() {
    DeviceSettings settings;
    settings.caliperMultiplier = CALIPER_MULTIPLIER;
    settings.apSSID = USER_AP_SSID;
    settings.apPassword = USER_AP_PASSWORD;
    settings.staSSID = USER_STA_SSID;
    settings.staPassword = USER_STA_PASSWORD;
    settings.mdnsHostname = USER_MDNS_HOSTNAME;
    return settings;
}

bool validateSettings(const DeviceSettings& settings) {
    // Check caliper multiplier
    if (settings.caliperMultiplier <= 0 || settings.caliperMultiplier > 100) {
        // log("Invalid caliper multiplier: %f", settings.caliperMultiplier);
        return false;
    }

    // Check AP SSID
    if (settings.apSSID.length() == 0 || settings.apSSID.length() > 32) {
        // log("Invalid AP SSID length: %d", settings.apSSID.length());
        return false;
    }

    // Check AP password
    if (settings.apPassword.length() < 8 || settings.apPassword.length() > 64) {
        // log("Invalid AP password length: %d", settings.apPassword.length());
        return false;
    }

    // Check STA SSID (can be empty)
    if (settings.staSSID.length() > 32) {
        // log("Invalid STA SSID length: %d", settings.staSSID.length());
        return false;
    }

    // Check STA password (can be empty)
    if (settings.staPassword.length() > 64) {
        // log("Invalid STA password length: %d", settings.staPassword.length());
        return false;
    }

    // Check mDNS hostname
    if (settings.mdnsHostname.length() == 0 || settings.mdnsHostname.length() > 32) {
        // log("Invalid mDNS hostname length: %d", settings.mdnsHostname.length());
        return false;
    }

    // Check characters in hostname
    for (char c : settings.mdnsHostname) {
        if (!isalnum(c) && c != '-') {
            // log("Invalid character in mDNS hostname: %c", c);
            return false;
        }
    }

    return true;
}

bool loadDeviceSettings(DeviceSettings& settings) {
    if (!LittleFS.exists(SETTINGS_FILE)) {
        // log("Settings file does not exist");
        return false;
    }

    File file = LittleFS.open(SETTINGS_FILE, "r");
    if (!file) {
        // log("Failed to open settings file for reading");
        return false;
    }

    String content = file.readString();
    file.close();

    if (content.length() == 0) {
        // log("Settings file is empty");
        return false;
    }

    return jsonToSettings(content, settings);
}

bool saveDeviceSettings(const DeviceSettings& settings) {
    if (!validateSettings(settings)) {
        // log("Settings validation failed");
        return false;
    }

    String json = settingsToJson(settings);
    if (json.length() == 0) {
        // log("Failed to serialize settings to JSON");
        return false;
    }

    File file = LittleFS.open(SETTINGS_FILE, "w");
    if (!file) {
        // log("Failed to open settings file for writing");
        return false;
    }

    size_t written = file.print(json);
    file.close();

    if (written != json.length()) {
        // log("Failed to write complete settings file");
        return false;
    }

    // log("Settings saved successfully");
    return true;
}

String settingsToJson(const DeviceSettings& settings) {
    JsonDocument doc;

    doc["caliperMultiplier"] = settings.caliperMultiplier;
    doc["apSSID"] = settings.apSSID;
    doc["apPassword"] = settings.apPassword;
    doc["staSSID"] = settings.staSSID;
    doc["staPassword"] = settings.staPassword;
    doc["mdnsHostname"] = settings.mdnsHostname;

    String json;
    if (serializeJson(doc, json) == 0) {
        // log("Failed to serialize settings to JSON");
        return "";
    }

    return json;
}

bool jsonToSettings(const String& json, DeviceSettings& settings) {
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        // log("Failed to parse settings JSON: %s", error.c_str());
        return false;
    }

    // Load with type checking and default values
    settings.caliperMultiplier = doc["caliperMultiplier"] | CALIPER_MULTIPLIER;
    settings.apSSID = doc["apSSID"] | USER_AP_SSID;
    settings.apPassword = doc["apPassword"] | USER_AP_PASSWORD;
    settings.staSSID = doc["staSSID"] | USER_STA_SSID;
    settings.staPassword = doc["staPassword"] | USER_STA_PASSWORD;
    settings.mdnsHostname = doc["mdnsHostname"] | USER_MDNS_HOSTNAME;

    return validateSettings(settings);
}
