#pragma once
#include <array>
#include <memory>

#include <Game/Constants.h>
#include "LobbySlot.h"

struct ClientData;
enum class PlayerState;
class AssetManager;
class GameManager;

class LobbyUI final
{
public:
    explicit LobbyUI(ClientData* _client_data);
    ~LobbyUI() = default;

    void refresh();
    void draw(sf::RenderWindow& _window);

private:
    void initSlots();
    std::string playerStateToString(const PlayerState& _state) const;

    ClientData* client_data;

    std::array<std::unique_ptr<LobbySlot>, MAX_PLAYERS> slots;

};
