#pragma once
#include <thread>
#include <vector>

#include <Game/ThreadDispatcher.h>
#include <Game/Simulation.h>
#include <Game/Scheduler.h>

class ISimulationServer;

class SimulationThread final : public ThreadDispatcher
{
public:
    explicit SimulationThread(ISimulationServer& _server);
    ~SimulationThread();

    void eventStartSimulation(const std::vector<int>& _bike_ids);
    void eventStopSimulation();
    void eventResetSimulation();
    void eventDirectionChanged(const unsigned int _bike_id, const MoveDirection _dir);
    void eventBoost(const unsigned int _bike_id);
    void eventPlayerLeft(const unsigned int _bike_id);

private:
    void stopSimulationThread();
    void simulationThreadLoop();
    void resetSimulation();

    void onSyncSimulation(const SimulationState& _simulation_state) const;
    void onSyncBike(const BikeState& _bike_state) const;
    void onSyncAllBikes(const std::array<BikeState, MAX_PLAYERS>& _bike_states) const;
    void onBikeBoost(const unsigned int _bike_id) const;
    void onSimulationStarted() const;
    void onSimulationEnded() const;

    void scheduleAllBikeSync(const double _time);
    void scheduleSimulationSync(const double _time);

    volatile bool thread_running;
    volatile bool simulation_running;
    std::thread simulation_thread;

    Simulation simulation;
    SimpleTimer simple_timer;
    Scheduler scheduler;

    bool bike_sync_needed;
    bool full_sync_needed;

    // TronServer simulation interface.
    ISimulationServer& server;

};