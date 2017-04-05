#pragma once

// The available actions the user can perform in the game.
enum class GameAction
{
    ACCEPT,
    DECLINE,
    PAUSE,
    QUIT,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    BOOST
};

// The state a GameAction can be in.
enum class ActionState
{
    PRESSED,
    RELEASED
};
