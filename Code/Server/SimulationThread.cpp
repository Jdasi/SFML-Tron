#include <iostream>

#include "SimulationThread.h"
#include "IServerSimulation.h"

SimulationThread::SimulationThread(IServerSimulation& _server)
    : thread_running(true)
    , simulation_running(false)
    , bike_sync_needed(true)
    , full_sync_needed(true)
    , server(_server)
{
    simulation.attachListener(this);
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

        server.onSyncSimulation(simulation.getState());
    });
}

void SimulationThread::eventStartSimulation()
{
    postEvent([this]()
    {
        scheduler.invoke([this]()
        {
            simulation_running = true;
            server.onSimulationStarted();
        }, COUNTDOWN_TIME);
    });
}


void SimulationThread::eventStopSimulation()
{
    postEvent([this]()
    {
        simulation_running = false;
        server.onSimulationStopping();
        server.onSyncSimulation(simulation.getState());

        scheduler.invoke([this]()
        {
            resetSimulation();
            server.onSimulationEnded();
        }, COUNTDOWN_TIME);
    });
}



void SimulationThread::eventResetSimulation()
{
    postEvent([this]()
    {
        simulation_running = false;

        resetSimulation();
        server.onSimulationReset();

        server.onSyncSimulation(simulation.getState());
    });
}



void SimulationThread::eventDirectionChanged(const unsigned int _bike_id, const MoveDirection _dir)
{
    postEvent([this, _bike_id, _dir]()
    {
        simulation.changeBikeDirection(_bike_id, _dir);

        server.onSyncBike(simulation.getBike(_bike_id).getState());
    });
}



void SimulationThread::eventBoost(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        if (simulation.getBike(_bike_id).activateBoost())
        {
            server.onBikeBoost(_bike_id);
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
            server.onSyncBike(bike.getState());
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
            simulation.tick(dt);

            scheduleAllBikeSync(0.5);
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

            server.onSyncAllBikes(simulation.getBikes());
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

            server.onSyncSimulation(simulation.getState());
            full_sync_needed = true;

            std::cout << "Sync simulation event" << std::endl;
        }, _time);
    }
}



void SimulationThread::bikeRemoved(const unsigned int _bike_id)
{
    server.onBikeRemoved(_bike_id);
    server.onSyncBike(simulation.getBike(_bike_id).getState());
}



void SimulationThread::simulationVictor(const unsigned int _bike_id)
{
    server.onSimulationVictor(_bike_id);
    eventStopSimulation();
}



void SimulationThread::simulationEmpty()
{
    eventResetSimulation();
}
