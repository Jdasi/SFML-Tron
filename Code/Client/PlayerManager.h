#pragma once
#include <vector>

#include "Player.h"

struct ClientData;

class PlayerManager
{
public:
    explicit PlayerManager(ClientData* _client_data);
    ~PlayerManager() = default;

    void addPlayer(int _id);
    void removePlayer(int _id);

    Player* getPlayer(int _id);

private:
    ClientData* client_data;
    std::vector<Player> players;

};
