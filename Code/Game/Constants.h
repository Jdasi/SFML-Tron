#pragma once
#include <string>

/*! \file Constants.h
@brief   Constants that are used throughout the game. 
@details Add any new constants to this file. 
*/

// ======================================================================================
// WINDOW SETTINGS
// ======================================================================================
const std::string WINDOW_NAME           { "Tron Game" };

const int WINDOW_WIDTH                  = 800; // Width of the screen.
const int WINDOW_HEIGHT                 = 600; // Height of the screen.
const int WINDOW_MARGIN_X               = 100; // Margin from horizontal screen edges.
const int WINDOW_MARGIN_Y               = 50; // Margin from vertical screen edges.

constexpr float WINDOW_LEFT_BOUNDARY    = WINDOW_MARGIN_X;
constexpr float WINDOW_RIGHT_BOUNDARY   = WINDOW_WIDTH - WINDOW_LEFT_BOUNDARY;
constexpr float WINDOW_TOP_BOUNDARY     = WINDOW_MARGIN_Y;
constexpr float WINDOW_BOTTOM_BOUNDARY  = WINDOW_HEIGHT - WINDOW_MARGIN_Y;

// ======================================================================================
// NETWORK SETTINGS
// ======================================================================================
//const std::string SERVER_IP             { "dgm.no-ip.org" };
const std::string SERVER_IP             { "127.0.0.1" };
//const std::string SERVER_IP             { "86.31.186.180" };
const unsigned int SERVER_TCP_PORT      = 1337;

// ======================================================================================
// GAMEPLAY SETTINGS
// ======================================================================================
const int MAX_PLAYERS                   = 4; // Max players supported.
const int INITIAL_MOVES                 = 5; // Players start having made this many moves.
const double COUNTDOWN_TIME             = 3.5; // Time before game/sim starts & ends.

const int GRID_SIZE_X                   = 125; // Width (in tiles) of the playing grid.
const int GRID_SIZE_Y                   = 125; // Height (in tiles) of the playing grid.
constexpr int GRID_AREA                 = GRID_SIZE_X * GRID_SIZE_Y;

const float BIKE_MOVE_SPEED             = 0.04f; // Rate at which the bike moves.
const float BIKE_BOOST_DURATION         = 1.5f; // Duration of boost.
const int BIKE_BOOST_DIVISOR            = 2; // Boost speed divisor. Larger = faster.
const int STARTING_BOOST_CHARGES        = 3; // Starting number of boost charges.
const float MARKER_ROTATE_SPEED         = 30.0f; // Speed of the player marker rotation.
const float MARKER_ROTATE_MODIFIER      = 2.0f; // rotation modifier of enlarged marker.

// ======================================================================================
// CONTROLLER SETTINGS
// ======================================================================================
const float AXIS_DEADZONE               = 80.0f; // Min input axis required by controller.


// ======================================================================================
// COMMON PATH STRINGS
// ======================================================================================



// ======================================================================================
// TEXTURE STRINGS
// ======================================================================================
const std::string DEFAULT_FONT          { "arial.ttf" };


// ======================================================================================
// TEXTURE STRINGS
// ======================================================================================
const std::string BACKDROP              { "backdrop.png" };
const std::string PLAYER_MARKER         { "player_marker.png" };


// ======================================================================================
// SOUND STRINGS
// ======================================================================================

