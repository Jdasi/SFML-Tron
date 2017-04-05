#pragma once
#include <array>
#include <memory>

#include <Game/Simulation.h>
#include "Player.h"

struct ClientData;

/* Class for the client to manage the flow of the game and record the
 * list of players based on information from the server.
 *
 * The GameManager owns the client's version of the simulation, and thus
 * all network messages regarding the simulation must go through here.
 */
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
