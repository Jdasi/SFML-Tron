#pragma once
#include <vector>
#include <array>

#include <Game/PlayerState.h>
#include <Game/FlowControl.h>

class Player;
struct SimulationState;
struct BikeState;

/* Interface class used by NetworkManager to decouple networking from
 * the TronClient.
 *
 * This provides a simplified interface inside NetworkManager for dealing
 * with events that should be passed to the client for processing.
 */
class INetworkClient
{
public:
    INetworkClient() = default;
    virtual ~INetworkClient() = default;

    virtual bool isExiting() const = 0;

    virtual void onConnected() = 0;
    virtual void onDisconnected() = 0;

    virtual void onUpdatePingTime(const double _ping) = 0;
    virtual void onIdentity(const unsigned int _player_id) = 0;
    virtual void onPlayerList(const std::vector<Player>& _players) = 0;
    virtual void onPlayerJoined(const unsigned int _player_id) = 0;
    virtual void onPlayerLeft(const unsigned int _player_id) = 0;
    virtual void onPlayerStateChange(const unsigned int _player_id, const PlayerState _state) = 0;
    virtual void onGameStateChange(const int _state) = 0;
    virtual void onFlowControl(const FlowControl _control) = 0;
    virtual void onVictor(const unsigned int _player_id) = 0;
    virtual void onBikeSync(const BikeState& _bike_state) = 0;
    virtual void onFullBikeSync(const std::array<BikeState, MAX_PLAYERS>& _bike_states) = 0;
    virtual void onFullSync(const SimulationState& _simulation_state) = 0;
    virtual void onBikeRemoved(const unsigned _bike_id) = 0;
    virtual void onBikeBoost(const unsigned int _bike_id) = 0;
    virtual void onBoostChargeGranted(const unsigned int _bike_id) = 0;
    virtual void onBulletinUpdate(const std::string& _bulletin) = 0;

};
