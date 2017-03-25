#include "Simulation.h"
#include "Player.h"
#include "Constants.h"


Simulation::Simulation()
    : colours_assigned(0)
{
    players.reserve(MAX_PLAYERS);
}

void Simulation::tick(double _dt)
{
    for (auto& player : players)
    {
        if (!player.isAlive())
        {
            continue;
        }

        player.modifyMoveTimer(_dt);

        if (player.getMoveTimer() > player.getMoveSpeed())
        {
            player.resetMoveTimer();

            movePlayer(player);
        }
    }
}

void Simulation::addPlayer()
{
    if (players.size() >= MAX_PLAYERS)
    {
        return;
    }

    Player player;

    player.setID(players.size());
    player.setColour(static_cast<CellColour>(colours_assigned++));

    player.setPosition({ 0, player.getID() * 20 }); // Need to change this to properly support multiple bikes...
    grid.setCell(player.getPosition(), { CellValue::HEAD, player.getColour() });

    players.push_back(player);
}

void Simulation::changePlayerDirection(unsigned int _player_id, MoveDirection _dir)
{
    if (_player_id > players.size() || players.empty())
    {
        return;
    }

    Player& player = players[_player_id];
    if (directionChangeValid(player, _dir))
    {
        player.setDirection(_dir);
    }
}

void Simulation::movePlayer(Player& _player)
{
    grid.setCell(_player.getPosition(), { CellValue::TRAIL, _player.getColour() } );

    for (auto& listener : listeners)
    {
        listener->updateCell(_player, CellValue::TRAIL);
    }

    MoveDirection dir = _player.getDirection();
    Vector2i pos = _player.getPosition();

    Vector2i adjustment = pos + generatePositionAdjustment(dir, pos);

    if (!adjustmentWithinBounds(adjustment))
    {
        // Bike hit the edge of the grid.
        _player.setAlive(false);
    }
    else if (adjustmentCollisionCheck(adjustment))
    {
        // Bike hit a trail.
        _player.setAlive(false);
    }
    else
    {
        // Path is clear.
        _player.setPosition(adjustment);
        grid.setCell(adjustment, { CellValue::HEAD, _player.getColour() });

        for (auto& listener : listeners)
        {
            listener->updateCell(_player, CellValue::HEAD);
        }
    }
}

Vector2i Simulation::generatePositionAdjustment(MoveDirection _dir, Vector2i _current_pos) const
{
    switch (_dir)
    {
        case MoveDirection::UP:     return {  0, -1 };
        case MoveDirection::DOWN:   return {  0,  1 };
        case MoveDirection::LEFT:   return { -1,  0 };
        case MoveDirection::RIGHT:  return {  1,  0 };

        default: return { 0, 0 };
    }
}

bool Simulation::adjustmentWithinBounds(Vector2i _adjustment) const
{
    if (_adjustment.x >= GRID_SIZE_X ||
        _adjustment.x < 0 ||
        _adjustment.y >= GRID_SIZE_Y ||
        _adjustment.y < 0)
    {
        return false;
    }

    return true;
}

bool Simulation::adjustmentCollisionCheck(Vector2i _adjustment) const
{
    if (grid.getCell(_adjustment).value == CellValue::NONE)
    {
        return false;
    }

    return true;
}

bool Simulation::directionChangeValid(Player& _player, MoveDirection _dir)
{
    if (_dir == MoveDirection::UP && _player.getDirection() == MoveDirection::DOWN ||
        _dir == MoveDirection::DOWN && _player.getDirection() == MoveDirection::UP ||
        _dir == MoveDirection::LEFT && _player.getDirection() == MoveDirection::RIGHT ||
        _dir == MoveDirection::RIGHT && _player.getDirection() == MoveDirection::LEFT ||
        _dir == _player.getDirection())
    {
        return false;
    }

    return true;
}
