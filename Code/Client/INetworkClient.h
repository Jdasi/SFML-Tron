#pragma once
#include <vector>
#include <array>

#include <Game/PlayerState.h>

class Player;
struct SimulationState;
struct BikeState;

/* Interface class used by TronNetworkManager to decouple networking from
 * the TronClient.
 *
 * This provides a simplified interface inside TronNetworkManager for dealing
 * with events that should be passed to the client for processing.
 */
class INetworkClient
{
public:
    INetworkClient() = default;
    virtual ~INetworkClient() = default;

    virtual void onConnected() = 0;
    virtual void onDisconnected() = 0;

    virtual void onUpdatePingTime(double _ping) = 0;
    virtual void onIdentity(int _id) = 0;
    virtual void onPlayerList(const std::vector<Player>& _players) = 0;
    virtual void onPlayerJoined(int _id) = 0;
    virtual void onPlayerStateChange(int _player_id, const PlayerState _state) = 0;
    virtual void onGameStateChange(int _state) = 0;
    virtual void onBikeSync(const BikeState& _bike_state) = 0;
    virtual void onFullBikeSync(const std::array<BikeState, MAX_PLAYERS>& _bike_states) = 0;
    virtual void onFullSync(const SimulationState& _simulation_state) = 0;

};
