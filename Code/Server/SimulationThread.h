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
    SimulationThread(ISimulationServer& _server);
    ~SimulationThread();

    void eventStartSimulation(const std::vector<int>& _bike_ids);
    void eventStopSimulation();
    void eventResetSimulation();
    void eventDirectionChanged(const unsigned int _bike_id, const MoveDirection _dir);
    void eventBoost(const unsigned int _bike_id);

    bool isSimulationRunning() const;

private:
    void stopSimulationThread();
    void simulationThreadLoop();
    void resetSimulation();

    void onSyncSimulation(const SimulationState& _simulation_state);
    void onSyncBike(const BikeState& _bike_state);
    void onSyncAllBikes(const std::array<BikeState, MAX_PLAYERS>& _bike_states);
    void onSimulationStarted();
    void onSimulationEnded();

    void scheduleAllBikeSync(double _time);
    void scheduleSimulationSync(double _time);

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