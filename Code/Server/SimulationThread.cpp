#include <iostream>

#include "SimulationThread.h"
#include "IServerSimulation.h"

// Initialises member data and launches the simulation thread.
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



// The thread must be joined on destruction of the SimulationThread.
SimulationThread::~SimulationThread()
{
    stopSimulationThread();
}



/* Called when the simulation needs to be prepared for use.
 * E.g. when all clients have readied up in the lobby.
 */
void SimulationThread::eventPrepareSimulation(const std::vector<int>& _bike_ids)
{
    postEvent([this, _bike_ids]()
    {
        for (auto& id : _bike_ids)
        {
            simulation.addBike(id);
        }

        server.onSyncAllBikes(simulation.getBikeStates());
    });
}



/* Called when the simulation needs to be started 
 * E.g. when all clients are loaded in to the game state.
 */
void SimulationThread::eventStartSimulation()
{
    postEvent([this]()
    {
        scheduler.invoke([this]()
        {
            simulation_running = true;
            server.onSimulationStarted();
        }, COUNTDOWN_BEGIN);
    });
}



// Called when the simulation needs to be stopped.
void SimulationThread::eventStopSimulation()
{
    postEvent([this]()
    {
        simulation_running = false;
        server.onSimulationStopping();
        server.onSyncAllBikes(simulation.getBikeStates());

        scheduler.invoke([this]()
        {
            resetSimulation();
            server.onSimulationEnded();
        }, COUNTDOWN_END);
    });
}



// Called when the simulation needs to be reset.
void SimulationThread::eventResetSimulation()
{
    postEvent([this]()
    {
        resetSimulation();
        server.onSimulationReset();
    });
}



// Called when a bike needs to change direction.
void SimulationThread::eventDirectionChanged(const unsigned int _bike_id, 
    const MoveDirection _dir)
{
    postEvent([this, _bike_id, _dir]()
    {
        simulation.changeBikeDirection(_bike_id, _dir);

        server.onSyncBike(simulation.getBikeState(_bike_id));
    });
}



// Called when a bike needs to boost.
void SimulationThread::eventBoost(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        if (simulation.activateBikeBoost(_bike_id))
        {
            server.onBikeBoost(_bike_id);
        }
    });
}



// Called when a player has left the game.
void SimulationThread::eventPlayerLeft(const unsigned int _bike_id)
{
    postEvent([this, _bike_id]()
    {
        simulation.removeBike(_bike_id);
    });
}



void SimulationThread::stopSimulationThread()
{
    simulation_thread.join();
}



/* Main loop for the simulation thread.
 * Here the dispatched methods are executed, and a scheduled
 * synchronisation of all the simulation's bikes is handled.
 */
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



/* Schedules a synchronisation event of all the simulation's bikes
 * if one is needed.
 */
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

            server.onSyncAllBikes(simulation.getBikeStates());
            bike_sync_needed = true;

            std::cout << "Sync all bikes event" << std::endl;
        }, _time);
    }
}



/* Schedules a synchronisation event for the server to send the simulation
 * through to all clients. Note: this is currently unused by the program.
 */
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



// Tell the server that a bike was just destroyed.
void SimulationThread::bikeRemoved(const unsigned int _bike_id)
{
    server.onBikeRemoved(_bike_id);
    server.onSyncBike(simulation.getBikeState(_bike_id));
}



// Tell the server that a bike has just boosted.
void SimulationThread::boostChargeGranted(const unsigned int _bike_id)
{
    server.onBoostChargeGranted(_bike_id);
}



// Tell the server that the simulation has ended and there is a victor.
void SimulationThread::simulationVictor(const unsigned int _bike_id)
{
    server.onSimulationVictor(_bike_id);
    eventStopSimulation();
}



// Tell the server that the simulation has reset.
void SimulationThread::simulationEmpty()
{
    eventResetSimulation();
}
