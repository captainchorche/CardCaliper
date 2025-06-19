#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <M5Cardputer.h>
#include "config.h"

// ============================================================================
// DISPLAY FUNCTIONS
// ============================================================================

// Initialize sprite buffer
void initSprite();

// Update display
void updateDisplay(int32_t microns, uint32_t timestamp);

// Show startup screen
void showStartupScreen();

// Show mode switching screen
void showSwitchingScreen();

// Show connecting screen
void showConnectingScreen();

// ============================================================================
// DISPLAY GLOBAL VARIABLES
// ============================================================================

extern LGFX_Sprite sprite;
extern bool spriteInitialized;

// Variables for display update control
extern uint32_t lastDisplayUpdate;
extern int32_t lastDisplayedMicrons;

// Non-blocking timers
extern uint32_t lastMainLoopTime;
extern uint32_t lastWebSocketTime;
extern uint32_t lastBLESendTime;

#endif // DISPLAY_H
