#pragma once
#include <string>

/*! \file Constants.h
@brief   Constants that are used throughout the game. 
@details Add any new constants to this file. 
*/

// ======================================================================================
// WINDOW SETTINGS
// ======================================================================================
const int WINDOW_WIDTH                  = 800; // Width of the screen.
const int WINDOW_HEIGHT                 = 600; // Height of the screen.
const int WINDOW_MARGIN                 = 20;  // For determining playable area.

constexpr int WINDOW_LEFT_BOUNDARY      = WINDOW_MARGIN;
constexpr int WINDOW_RIGHT_BOUNDARY     = WINDOW_WIDTH - WINDOW_LEFT_BOUNDARY;
constexpr int WINDOW_TOP_BOUNDARY       = WINDOW_MARGIN;
constexpr int WINDOW_BOTTOM_BOUNDARY    = WINDOW_HEIGHT - WINDOW_MARGIN;

// ======================================================================================
// NETWORK SETTINGS
// ======================================================================================
const std::string SERVER_IP             { "127.0.0.1" };
const unsigned int SERVER_TCP_PORT      = 72500;

// ======================================================================================
// GAMEPLAY SETTINGS
// ======================================================================================


// ======================================================================================
// COMMON PATH STRINGS
// ======================================================================================


// ======================================================================================
// TEXTURE STRINGS
// ======================================================================================


// ======================================================================================
// SOUND STRINGS
// ======================================================================================

