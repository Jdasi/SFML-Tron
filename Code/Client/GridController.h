#pragma once
#include <vector>

#include "Bike.h"

class GameGrid;
struct ClientData;

class GridController
{
public:
    explicit GridController(GameGrid& _game_grid);
    ~GridController() = default;

    void tick(ClientData* _client_data);

    void addBike();
    void changeBikeDirection(unsigned int _bike_id, MoveDirection _dir);

private:
    bool directionChangeValid(Bike& _bike, MoveDirection _dir) const;
    void moveBike(Bike& _bike) const;
    sf::Vector2<int> generatePositionAdjustment(MoveDirection _dir, 
        sf::Vector2<int> _current_position) const;
    bool adjustmentWithinBounds(sf::Vector2<int> _adjustment) const;
    bool adjustmentCollisionCheck(sf::Vector2<int> _adjustment) const;

    GameGrid& game_grid;
    std::vector<Bike> bikes;
    std::vector<sf::Color> colors;

};
