#include "caliper.h"
#include "settings.h"

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

Reading current_reading;
ConnectionMode current_mode = MODE_NONE;

// Operating modes (from user settings)
volatile bool debug_mode = USER_DEBUG_DEFAULT;
volatile bool stream_mode = USER_STREAM_DEFAULT;
volatile bool noise_filter_enabled = USER_NOISE_FILTER_DEFAULT;

// Statistics
volatile uint32_t total_packets = 0;
volatile uint32_t valid_packets = 0;
volatile uint32_t error_packets = 0;

// ============================================================================
// LOGGING FUNCTIONS
// ============================================================================

// Debug functions removed to save resources

// ============================================================================
// CALIPER READING FUNCTIONS
// ============================================================================

void initCaliper() {
    // Configure ADC
    analogReadResolution(ADC_RESOLUTION);
    analogSetAttenuation(ADC_ATTENUATION);

    // log("GPIO Configuration:");
    // log("  Data Pin (SPC Data):  GPIO%d", DATA_PIN);
    // log("  Clock Pin (SPC Clock): GPIO%d", CLOCK_PIN);
    // log("ADC Configuration:");
    // log("  Resolution: %d-bit (0-4095)", ADC_RESOLUTION);
    // log("  Attenuation: 6dB (max 2.2V)");
    // log("  Threshold: %d", ADC_THRESHOLD);

    // Check initial signal levels
    int dataLevel = analogRead(DATA_PIN);
    int clockLevel = analogRead(CLOCK_PIN);
    // log("Initial signal levels:");
    // log("  Data level: %d", dataLevel);
    // log("  Clock level: %d", clockLevel);
    
    // Initialize variables
    current_reading.microns = MIN_RANGE;
    current_reading.timestamp = millis();
    
    total_packets = 0;
    valid_packets = 0;
    error_packets = 0;
}

int getBit(int dataPin, int clockPin) {
    uint32_t startTime = millis();
    
    // Wait for clock signal fall (HIGH -> LOW) - optimized waiting
    while (analogRead(clockPin) > ADC_THRESHOLD) {
        if (millis() - startTime > BIT_READ_TIMEOUT) {
            // if (debug_mode) log("getBit: Timeout waiting for clock LOW");
            return -1;
        }
        // Removed delayMicroseconds for maximum speed
    }

    // Wait for clock signal rise (LOW -> HIGH) - optimized waiting
    while (analogRead(clockPin) < ADC_THRESHOLD) {
        if (millis() - startTime > BIT_READ_TIMEOUT) {
            // if (debug_mode) log("getBit: Timeout waiting for clock HIGH");
            return -1;
        }
        // Removed delayMicroseconds for maximum speed
    }

    // Read data on clock rise
    int data = (analogRead(dataPin) > ADC_THRESHOLD) ? 1 : 0;

    if (debug_mode) {
        // log("getBit: data=%d, dataLevel=%d, clockLevel=%d",
        //     data, analogRead(dataPin), analogRead(clockPin));
    }
    
    return data;
}

long getPacket(int dataPin, int clockPin) {
    long packet = 0;
    
    for (int i = 0; i < PACKET_BITS; i++) {
        int bit = getBit(dataPin, clockPin);
        if (bit < 0) {
            // if (debug_mode) log("getPacket: Failed to read bit %d", i);
            return -1;
        }
        packet |= ((long)bit << i);

        if (debug_mode && i < 5) {
            // log("getPacket: bit[%d] = %d, packet = 0x%08lX", i, bit, packet);
        }
    }

    if (debug_mode) {
        // log("getPacket: Complete packet = 0x%08lX (%ld)", packet, packet);
    }
    
    return packet;
}

long getMicrons(long packet) {
    if (packet < 0)
        return MIN_RANGE;

    // Extract 20-bit data (bits 0-19)
    long data = (packet & 0xFFFFF);

    // Check sign (bit 20)
    bool isNegative = (packet & (1L << 20)) != 0;

    // Apply sign
    long rawMicrons = isNegative ? -data : data;

    // Apply user settings
    long microns = (long)(rawMicrons * currentSettings.caliperMultiplier);

    // Sign inversion (if needed)
    if (CALIPER_INVERT_SIGN) {
        microns = -microns;
    }

    // Apply zero offset
    microns -= CALIPER_ZERO_OFFSET;

    if (debug_mode) {
        // log("getMicrons: packet=0x%08lX, data=0x%05lX, sign=%s, raw=%ld, multiplier=%.1f, final=%ld",
        //     packet, data, isNegative ? "NEG" : "POS", rawMicrons, currentSettings.caliperMultiplier, microns);
    }

    return microns;
}

// ============================================================================
// NOISE FILTERING
// ============================================================================

bool isValidMeasurement(int32_t microns) {
    if (!noise_filter_enabled) {
        return true;  // Filter disabled - pass everything
    }
    
    static int32_t lastRejectedValue = MIN_RANGE;
    static int rejectedCount = 0;
    
    // Compare with CURRENT value on display, not with last accepted
    int32_t currentDisplayValue = current_reading.microns;
    
    // If this is first measurement or no current value - accept
    if (currentDisplayValue == MIN_RANGE) {
        rejectedCount = 0;
        return true;
    }
    
    // Check difference with current displayed value
    int32_t diff = abs(microns - currentDisplayValue);
    
    if (diff > NOISE_FILTER_THRESHOLD) {  // Jump more than 10mm
        // Check - is this the same "new" value we already rejected?
        if (abs(microns - lastRejectedValue) < 1000) {  // Within 1mm of previous rejected
            rejectedCount++;
            if (rejectedCount >= 3) {  // If 3 times in a row the same - accept
                // log("Noise filter: Stable new value after %d rejections: %ld", rejectedCount, microns);
                rejectedCount = 0;
                return true;
            }
        } else {
            rejectedCount = 1;  // New rejected value
            lastRejectedValue = microns;
        }

        // log("Noise filter: Jump >10mm: %ld -> %ld (diff: %ld um, count: %d)",
        //     currentDisplayValue, microns, diff, rejectedCount);
        return false;  // Reject
    }
    
    // Value passed check - reset rejection counter
    rejectedCount = 0;
    return true;
}

// ============================================================================
// DEBUG FUNCTIONS
// ============================================================================

// Debug functions removed
