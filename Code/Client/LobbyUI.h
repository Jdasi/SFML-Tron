#pragma once
#include <array>
#include <memory>

#include <Game/Constants.h>
#include <Game/PlayerState.h>
#include "LobbySlot.h"

struct ClientData;
class AssetManager;
class GameManager;

/* Class for visualising the list of players on the server.
 * LobbyUI owns and arranges an array of LobbySlots, which display the
 * actual information.
 *
 * LobbyUI needs to be occassionally refreshed to ensure the displayed
 * information is up to date.
 */
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
