#include <algorithm>

#include <Game/Constants.h>
#include "PlayerManager.h"
#include "ClientData.h"

PlayerManager::PlayerManager(ClientData* _client_data)
    : client_data(_client_data)
{
    players.reserve(MAX_PLAYERS);
}

void PlayerManager::addPlayer(int _id)
{
    if (players.size() >= MAX_PLAYERS)
    {
        return;
    }

    Player player;
    player.setID(_id);

    if (player.getID() == client_data->client_id)
    {
        player.setClient(true);
    }

    players.push_back(player);
}

void PlayerManager::removePlayer(int _id)
{
    players.erase(std::remove_if(
        players.begin(),
        players.end(),
        [_id](Player& _player) { return _player.getID() == _id; }),
        players.end());
}

Player* PlayerManager::getPlayer(int _id)
{
    for (auto& player : players)
    {
        if (player.getID() == _id)
        {
            return &player;
        }
    }

    return nullptr;
}
