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
const int WINDOW_MARGIN_X               = 100; // Margin from horizontal screen edges.
const int WINDOW_MARGIN_Y               = 50; // Margin from vertical screen edges.

constexpr float WINDOW_LEFT_BOUNDARY      = WINDOW_MARGIN_X;
constexpr float WINDOW_RIGHT_BOUNDARY     = WINDOW_WIDTH - WINDOW_LEFT_BOUNDARY;
constexpr float WINDOW_TOP_BOUNDARY       = WINDOW_MARGIN_Y;
constexpr float WINDOW_BOTTOM_BOUNDARY    = WINDOW_HEIGHT - WINDOW_MARGIN_Y;

// ======================================================================================
// NETWORK SETTINGS
// ======================================================================================
const std::string SERVER_IP             { "127.0.0.1" };
const unsigned int SERVER_TCP_PORT      = 1337;

// ======================================================================================
// GAMEPLAY SETTINGS
// ======================================================================================
const int MAX_PLAYERS                   = 4; // Max players supported.

const int GRID_SIZE_X                   = 125; // Width (in tiles) of the playing grid.
const int GRID_SIZE_Y                   = 125; // Height (in tiles) of the playing grid.

const float BIKE_MOVE_SPEED             = 0.025f; // Rate at which the bike moves.

// ======================================================================================
// COMMON PATH STRINGS
// ======================================================================================


// ======================================================================================
// TEXTURE STRINGS
// ======================================================================================


// ======================================================================================
// SOUND STRINGS
// ======================================================================================

