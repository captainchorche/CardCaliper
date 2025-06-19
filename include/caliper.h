#ifndef CALIPER_H
#define CALIPER_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// DATA STRUCTURES
// ============================================================================

// Structure for storing measurement
struct Reading {
    uint32_t timestamp;
    int32_t microns;
    
    Reading() : timestamp(millis()), microns(MIN_RANGE) {}
};

// Connection modes
enum ConnectionMode {
    MODE_NONE = 0,
    MODE_BLE = 1,
    MODE_AP = 2,
    MODE_STA = 3
};

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

extern Reading current_reading;
extern ConnectionMode current_mode;

// Operating modes
extern volatile bool debug_mode;
extern volatile bool stream_mode;
extern volatile bool noise_filter_enabled;

// Statistics
extern volatile uint32_t total_packets;
extern volatile uint32_t valid_packets;
extern volatile uint32_t error_packets;

// ============================================================================
// CALIPER READING FUNCTIONS
// ============================================================================

// Initialize data reading
void initCaliper();

// Read one bit
int getBit(int dataPin, int clockPin);

// Read data packet
long getPacket(int dataPin, int clockPin);

// Convert packet to microns
long getMicrons(long packet);

// Check measurement validity (noise filter)
bool isValidMeasurement(int32_t microns);

#endif // CALIPER_H
