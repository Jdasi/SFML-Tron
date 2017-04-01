#include <iostream>

#include "SimulationThread.h"
#include "ISimulationServer.h"

SimulationThread::SimulationThread(ISimulationServer& _server)
    : thread_running(true)
    , simulation_running(false)
    , bike_sync_needed(true)
    , full_sync_needed(true)
    , server(_server)
{
    simulation_thread = std::thread([this]()
    {
        simulationThreadLoop();
    });
}

SimulationThread::~SimulationThread()
{
    stopSimulationThread();
}

void SimulationThread::eventStartSimulation(const std::vector<int>& _bike_ids)
{
    postEvent([this, _bike_ids]()
    {
        simulation_running = true;
        
        for (auto& id : _bike_ids)
        {
            simulation.addBike(id);
        }

        onSyncSimulation(simulation.getState());
        onSimulationStarted();
    });
}

void SimulationThread::eventStopSimulation()
{
    postEvent([this]()
    {
        resetSimulation();
        onSimulationEnded();
    });
}

void SimulationThread::eventResetSimulation()
{
    postEvent([this]()
    {
        resetSimulation();
    });
}

void SimulationThread::eventDirectionChanged(const unsigned int _bike_id, const MoveDirection _dir)
{
    postEvent([this, _bike_id, _dir]()
    {
        simulation.changeBikeDirection(_bike_id, _dir);

        onSyncBike(simulation.getBike(_bike_id).getState());
    });
}

void SimulationThread::eventBoost(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        if (simulation.getBike(_bike_id).boost())
        {
            onSyncBike(simulation.getBike(_bike_id).getState());
        }
    });
}

bool SimulationThread::isSimulationRunning() const
{
    return simulation_running;
}

void SimulationThread::stopSimulationThread()
{
    simulation_thread.join();
}

void SimulationThread::simulationThreadLoop()
{
    while (thread_running)
    {
        double dt = simple_timer.getTimeDifference();
        simple_timer.reset();

        executeDispatchedMethods();
        scheduler.update();

        if (simulation_running)
        {
            simulation.tick(dt);

            if (simulation.allBikesDead())
            {
                eventStopSimulation();
            }

            scheduleAllBikeSync(0.2);
            scheduleSimulationSync(5.0);
        }
    }
}

void SimulationThread::resetSimulation()
{
    simulation_running = false;
    simulation.reset();

    bike_sync_needed = true;
    full_sync_needed = true;
}

void SimulationThread::onSyncSimulation(const SimulationState& _simulation)
{
    server.onSyncSimulation(_simulation);
}

void SimulationThread::onSyncBike(const BikeState& _bike)
{
    server.onSyncBike(_bike);
}

void SimulationThread::onSyncAllBikes(const std::array<BikeState, MAX_PLAYERS>& _bikes)
{
    server.onSyncAllBikes(_bikes);
}

void SimulationThread::onSimulationStarted()
{
    server.onSimulationStarted();
}

void SimulationThread::onSimulationEnded()
{
    server.onSimulationEnded();
}

void SimulationThread::scheduleAllBikeSync(double _time)
{
    if (bike_sync_needed)
    {
        bike_sync_needed = false;

        scheduler.invoke([this]()
        {
            onSyncAllBikes(simulation.getBikes());
            bike_sync_needed = true;

            std::cout << "Sync all bikes event" << std::endl;
        }, 0.2);
    }
}

void SimulationThread::scheduleSimulationSync(double _time)
{
    if (full_sync_needed)
    {
        full_sync_needed = false;

        scheduler.invoke([this]()
        {
            onSyncSimulation(simulation.getState());
            full_sync_needed = true;

            std::cout << "Sync simulation event" << std::endl;
        }, 5.0);
    }
}
