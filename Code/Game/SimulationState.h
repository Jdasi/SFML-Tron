#pragma once
#include <array>

#include <SFML/Network.hpp>

#include "Constants.h"
#include "CellValue.h"
#include "BikeState.h"

struct SimulationState
{
    std::array<CellValue, GRID_AREA> cells {{ CellValue::NONE }};
    std::array<BikeState, MAX_PLAYERS> bikes;

    friend sf::Packet& operator<<(sf::Packet& _packet, SimulationState _simulation_state)
    {
        for (auto& cell_value : _simulation_state.cells)
        {
            _packet << static_cast<sf::Uint8>(cell_value);
        }

        for (auto& bike : _simulation_state.bikes)
        {
            _packet << bike;
        }

        return _packet;
    }

    friend sf::Packet& operator >> (sf::Packet& _packet, SimulationState& _simulation_state)
    {
        std::array<CellValue, GRID_AREA> cells;
        for (unsigned int i = 0; i < GRID_AREA; ++i)
        {
            sf::Uint8 cell_value;

            _packet >> cell_value;
            cells[i] = static_cast<CellValue>(cell_value);
        }
        _simulation_state.cells = cells;

        std::array<BikeState, MAX_PLAYERS> bikes;
        for (sf::Uint8 i = 0; i < MAX_PLAYERS; ++i)
        {
            _packet >> bikes[i];
        }
        _simulation_state.bikes = bikes;

        return _packet;
    }
};
