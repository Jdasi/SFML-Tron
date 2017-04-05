#pragma once

// The states a Player/User/Client can be in.
enum class PlayerState
{
    NOTREADY,
    READY,
    STARTING_GAME,
    PLAYING,
    VIEWING_RESULTS
};
