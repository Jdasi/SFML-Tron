#pragma once
#include <array>
#include <memory>

#include <Game/Simulation.h>
#include "Player.h"

struct ClientData;

class GameManager final : public Noncopyable
{
public:
    explicit GameManager(ClientData* _client_data);
    ~GameManager() = default;

    void tick();

    void startCountdown();
    void startSimulation();
    void stopSimulation();
    void resetSimulation();

    int getCountdownDigit() const;
    bool simulationRunning() const;

    void attachSimulationListener(SimulationListener* _listener);
    ISimulation* getNetworkSimulation();

    Player* getPlayer(const unsigned int _id);
    void addPlayer(const unsigned int _id, const PlayerState _state = PlayerState::NOTREADY);
    void removePlayer(const unsigned int _id);

private:
    ClientData* client_data;
    Simulation simulation;
    bool simulation_running;
    bool countdown_started;
    double countdown_timer;

    std::array<std::unique_ptr<Player>, MAX_PLAYERS> players;

};
