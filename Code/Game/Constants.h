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
// GAMEPLAY SETTINGS
// ======================================================================================
const int MIN_PLAYERS                   = 2; // Min players needed for game to start.
const int MAX_PLAYERS                   = 4; // Max players supported.
const int INITIAL_MOVES                 = 5; // Players start having made this many moves.
const double COUNTDOWN_BEGIN            = 3.0; // Time before game starts.
const double COUNTDOWN_END              = 3.0;  // Time before the game ends.

const int GRID_SIZE_X                   = 125; // Width (in tiles) of the playing grid.
const int GRID_SIZE_Y                   = 125; // Height (in tiles) of the playing grid.
constexpr int GRID_AREA                 = GRID_SIZE_X * GRID_SIZE_Y;

const float BIKE_MOVE_SPEED             = 0.037f; // Rate at which the bike moves.
const float BIKE_BOOST_DURATION         = 1.5f; // Duration of boost.
const int BIKE_BOOST_DIVISOR            = 2; // Boost speed divisor. Larger = faster.
const int STARTING_BOOST_CHARGES        = 3; // Starting number of boost charges.
const float EXTRA_BOOST_INTERVAL        = 6.0f; // Seconds before boost charge replenish.

const float MARKER_ROTATE_SPEED         = 40.0f; // Speed of the player marker rotation.
const float MARKER_ROTATE_MODIFIER      = 2.0f; // rotation modifier of enlarged marker.

// ======================================================================================
// CONTROLLER SETTINGS
// ======================================================================================
const float AXIS_DEADZONE               = 80.0f; // Min input axis required by controller.

// ======================================================================================
// COMMON PATH STRINGS
// ======================================================================================
const std::string FONTS_PATH            { "Resources/Fonts/" };
const std::string TEXTURES_PATH         { "Resources/Images/" };
const std::string AUDIO_PATH            { "Resources/Audio/" };

// ======================================================================================
// FONT STRINGS
// ======================================================================================
const std::string DEFAULT_FONT          { "arial.ttf" };

// ======================================================================================
// TEXTURE STRINGS
// ======================================================================================
const std::string BACKDROP              { "backdrop.png" };
const std::string PLAYER_MARKER         { "player_marker.png" };

// ======================================================================================
// AUDIO STRINGS
// ======================================================================================
const std::string COUNTDOWN_TICK_CUE    { "countdown_tick.ogg" };
const std::string COUNTDOWN_FIN_CUE     { "countdown_fin.ogg" };

const std::string BOOST_CUE             { "bike_boost.ogg" };
const std::string EXTRA_BOOST_CUE       { "extra_boost.ogg" };
const std::string DEATH_CUE             { "bike_death.ogg" };
const std::string LAST_BIKE_CUE         { "last_bike.ogg" };
const std::string GAME_OVER_CUE         { "game_over.ogg" };
const std::string CONNECT_CUE           { "connect.ogg" };
const std::string DISCONNECT_CUE        { "disconnect.ogg" };

const std::string GAME_MUSIC            { "game_music.ogg" };
