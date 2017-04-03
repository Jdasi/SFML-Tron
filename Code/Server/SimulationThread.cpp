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
    resetSimulation();

    simulation_thread = std::thread([this]()
    {
        simulationThreadLoop();
    });
}



SimulationThread::~SimulationThread()
{
    stopSimulationThread();
}



void SimulationThread::eventPrepareSimulation(const std::vector<int>& _bike_ids)
{
    postEvent([this, _bike_ids]()
    {
        for (auto& id : _bike_ids)
        {
            simulation.addBike(id);
        }

        onSyncSimulation(simulation.getState());
    });
}

void SimulationThread::eventStartSimulation()
{
    postEvent([this]()
    {
        scheduler.invoke([this]()
        {
            simulation_running = true;
        }, COUNTDOWN_TIME);
    });
}


void SimulationThread::eventStopSimulation()
{
    postEvent([this]()
    {
        simulation_running = false;
        onSimulationStopping();

        scheduler.invoke([this]()
        {
            resetSimulation();
            onSimulationEnded();
        }, COUNTDOWN_TIME);
    });
}



void SimulationThread::eventResetSimulation()
{
    postEvent([this]()
    {
        simulation_running = false;

        resetSimulation();
        onSimulationReset();

        onSyncSimulation(simulation.getState());
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
        if (simulation.getBike(_bike_id).activateBoost())
        {
            onBikeBoost(_bike_id);
        }
    });
}



void SimulationThread::eventPlayerLeft(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        auto& bike = simulation.getBike(_bike_id);

        if (bike.isAlive())
        {
            bike.setAlive(false);
            onSyncBike(bike.getState());
        }
    });
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
            // There is a victor.
            if (simulation.oneBikeLeft())
            {
                eventStopSimulation();
                continue;
            }

            // No one left in the game.
            if (simulation.allBikesDead())
            {
                eventResetSimulation();
                continue;
            }

            simulation.tick(dt);

            scheduleAllBikeSync(0.5);
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



void SimulationThread::scheduleAllBikeSync(const double _time)
{
    if (bike_sync_needed)
    {
        bike_sync_needed = false;

        scheduler.invoke([this]()
        {
            if (!simulation_running)
            {
                return;
            }

            onSyncAllBikes(simulation.getBikes());
            bike_sync_needed = true;

            std::cout << "Sync all bikes event" << std::endl;
        }, _time);
    }
}



void SimulationThread::scheduleSimulationSync(const double _time)
{
    if (full_sync_needed)
    {
        full_sync_needed = false;

        scheduler.invoke([this]()
        {
            if (!simulation_running)
            {
                return;
            }

            onSyncSimulation(simulation.getState());
            full_sync_needed = true;

            std::cout << "Sync simulation event" << std::endl;
        }, _time);
    }
}



void SimulationThread::onSyncSimulation(const SimulationState& _simulation) const
{
    server.onSyncSimulation(_simulation);
}



void SimulationThread::onSyncBike(const BikeState& _bike) const
{
    server.onSyncBike(_bike);
}



void SimulationThread::onSyncAllBikes(const std::array<BikeState, MAX_PLAYERS>& _bikes) const
{
    server.onSyncAllBikes(_bikes);
}



void SimulationThread::onBikeBoost(const unsigned int _bike_id) const
{
    server.onBikeBoost(_bike_id);
}



void SimulationThread::onSimulationReset() const
{
    server.onSimulationReset();
}



void SimulationThread::onSimulationStopping() const
{
    server.onSimulationStopping();
}



void SimulationThread::onSimulationEnded() const
{
    server.onSimulationEnded();
}
