#pragma once
#include <array>

#include <SFML/Network.hpp>

#include "Constants.h"
#include "CellValue.h"
#include "BikeState.h"

/* Struct containing all of the data a simulation uses that can be 
 * synchronised across the server.
 *
 * Operator overloads are provided to allow the SimulationState to be
 * read into and out of a packet.
 */
struct SimulationState
{
    std::array<CellValue, GRID_AREA> cells {{ CellValue::NONE }};
    std::array<BikeState, MAX_PLAYERS> bikes;

    friend sf::Packet& operator<<(sf::Packet& _packet, 
        const SimulationState& _simulation_state);

    friend sf::Packet& operator>>(sf::Packet& _packet, 
        SimulationState& _simulation_state);
};
