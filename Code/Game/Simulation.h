#pragma once
#include <vector>

#include "MoveDirection.h"
#include "Vector2i.h"
#include "Grid.h"
#include "ListenerSubject.h"
#include "Player.h"
#include "SimulationListener.h"

class Simulation : public ListenerSubject<SimulationListener>
{
public:
    Simulation();
    ~Simulation() = default;

    void tick(double _dt);

    void addPlayer();
    void changePlayerDirection(unsigned int _player_id, MoveDirection _dir);

private:
    void movePlayer(Player& _player);
    Vector2i generatePositionAdjustment(MoveDirection _dir, Vector2i _current_pos) const;
    bool adjustmentWithinBounds(Vector2i _adjustment) const;
    bool adjustmentCollisionCheck(Vector2i _adjustment) const;
    bool directionChangeValid(Player& _player, MoveDirection _dir);

    Grid grid;
    std::vector<Player> players;
    int colours_assigned;

};
