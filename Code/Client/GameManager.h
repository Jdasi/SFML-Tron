#pragma once
#include <map>

#include <Game/Simulation.h>
#include "Player.h"

struct ClientData;

class GameManager
{
public:
    explicit GameManager(ClientData* _client_data);
    ~GameManager() = default;

    void tick();

    void startSimulation();
    void stopSimulation();

    Simulation* getSimulation();
    INetworkSimulation* getNetworkSimulation();

    void addPlayer(int _id, PlayerState _state = PlayerState::NOTREADY);
    void removePlayer(int _id);

    Player* getPlayer(int _id);

private:
    ClientData* client_data;
    Simulation simulation;
    bool simulation_running;

    std::map<int, Player> players;

};
