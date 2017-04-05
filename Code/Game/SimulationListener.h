#pragma once
#include <vector>
#include <array>

#include "Constants.h"
#include "CellValue.h"
#include "Noncopyable.h"

struct BikeState;
struct Vector2i;

/* Class for other classes to derive from if they wish to listen in
 * on events from the simulation.
 *
 * Inherited functions are not pure virtual so derived classes must choose 
 * which events they listen in on.
 */
class SimulationListener : public Noncopyable
{
public:
    SimulationListener() = default;
    virtual ~SimulationListener() = default;

    virtual void clearCell(const Vector2i& _pos) {}
    virtual void clearCellRange(const std::vector<Vector2i>& _positions) {}
    virtual void simulationReset() {}

    virtual void overwriteCell(const Vector2i& _pos, const CellValue _value) {}
    virtual void overwriteCellRange(const std::vector<Vector2i>& _positions,
        const CellValue _value) {}

    virtual void overwriteAllCells(const std::array<CellValue, GRID_AREA>& _cells) {}

    virtual void updateBikePosition(const unsigned int _bike_id, 
        const Vector2i& _bike_pos, const bool _bike_alive = true) {}
    virtual void bikeBoosted(const unsigned int _bike_id) {}
    virtual void bikeNotBoosted(const unsigned int _bike_id) {}
    virtual void bikeRemoved(const unsigned int _bike_id) {}
    virtual void bikesReset() {}
    virtual void boostChargeGranted(const unsigned int _bike_id) {}

    virtual void simulationVictor(const unsigned int _bike_id) {}
    virtual void simulationEmpty() {}

};
