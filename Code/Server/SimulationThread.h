#pragma once
#include <thread>
#include <vector>

#include <Game/ThreadDispatcher.h>
#include <Game/Simulation.h>
#include <Game/Scheduler.h>

class IServerSimulation;

class SimulationThread final : public ThreadDispatcher, public SimulationListener
{
public:
    explicit SimulationThread(IServerSimulation& _server);
    ~SimulationThread();

    void eventPrepareSimulation(const std::vector<int>& _bike_ids);
    void eventStartSimulation();
    void eventStopSimulation();
    void eventResetSimulation();
    void eventDirectionChanged(const unsigned int _bike_id, const MoveDirection _dir);
    void eventBoost(const unsigned int _bike_id);
    void eventPlayerLeft(const unsigned int _bike_id);

private:
    void stopSimulationThread();
    void simulationThreadLoop();
    void resetSimulation();

    void scheduleAllBikeSync(const double _time);
    void scheduleSimulationSync(const double _time);

    // SimulationListener events.
    void bikeRemoved(const unsigned int _bike_id) override;
    void simulationVictor(const unsigned int _bike_id) override;
    void simulationEmpty() override;

    volatile bool thread_running;
    volatile bool simulation_running;
    std::thread simulation_thread;

    Simulation simulation;
    SimpleTimer simple_timer;
    Scheduler scheduler;

    bool bike_sync_needed;
    bool full_sync_needed;
    bool counting_down;

    // TronServer simulation interface.
    IServerSimulation& server;

};