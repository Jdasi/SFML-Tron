#pragma once
#include <vector>
#include <Game/PlayerState.h>

class Player;
class Bike;
class Simulation;

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
    virtual void onBikeDirectionChange(int _id, MoveDirection _dir) = 0;
    virtual void onIdentity(int _id) = 0;
    virtual void onPlayerList(std::vector<Player> _players) = 0;
    virtual void onPlayerJoined(int _id) = 0;
    virtual void onPlayerStateChange(int _player_id, PlayerState _state) = 0;
    virtual void onGameStateChange(int _state) = 0;
    virtual void onBikeSync(Bike& _bike) = 0;
    virtual void onFullSync(Simulation& _simulation) = 0;

};
