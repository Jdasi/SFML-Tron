#pragma once

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

enum class ActionState
{
    PRESSED,
    RELEASED
};
