#pragma once
#include <array>
#include <memory>

#include <Game/Constants.h>
#include "LobbySlot.h"

enum class PlayerState;
class AssetManager;
class GameManager;

class LobbyUI final
{
public:
    explicit LobbyUI(AssetManager* _asset_manager, GameManager* _game_manager);
    ~LobbyUI() = default;

    void refresh();
    void draw(sf::RenderWindow& _window);

private:
    void initSlots();
    std::string playerStateToString(const PlayerState& _state) const;

    AssetManager* asset_manager;
    GameManager* game_manager;

    std::array<std::unique_ptr<LobbySlot>, MAX_PLAYERS> slots;

};
