#pragma once
#include <vector>
#include <array>

#include "Constants.h"
#include "CellValue.h"

struct BikeState;
struct Vector2i;

class SimulationListener
{
public:
    SimulationListener() = default;
    virtual ~SimulationListener() = default;

    virtual void clearCell(const Vector2i& _pos) = 0;
    virtual void clearCellRange(const std::vector<Vector2i>& _positions) = 0;
    virtual void clearAllCells() = 0;

    virtual void overwriteCell(const Vector2i& _pos, const CellValue _value) = 0;
    virtual void overwriteCellRange(const std::vector<Vector2i>& _positions,
        const CellValue _value) = 0;

    virtual void overwriteAllCells(const std::array<CellValue, GRID_AREA>& _cells) = 0;

    virtual void updatePlayerMarker(const BikeState& _bike_state) = 0;
    virtual void removeAllPlayerMarkers() = 0 ;

    virtual void updateBikePosition(const Vector2i& _pos, const unsigned int _bike_id) = 0;

};
