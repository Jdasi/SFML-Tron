#include <Game/Simulation.h>
#include <Game/JHelper.h>
#include <Game/Constants.h>
#include "LobbyUI.h"
#include "AssetManager.h"
#include "GameManager.h"

LobbyUI::LobbyUI(AssetManager* _asset_manager, GameManager* _game_manager)
    : asset_manager(_asset_manager)
    , game_manager(_game_manager)
{
    initSlots();
}

void LobbyUI::refresh()
{
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        auto player = game_manager->getPlayer(i);
        auto slot = slots[i].get();

        if (!player)
        {
            slot->setOccupied(false);
            continue;
        }

        slot->setOccupied(true);
        slot->setPlayerStateText(playerStateToString(player->getState()));
    }
}

void LobbyUI::draw(sf::RenderWindow& _window)
{
    for (auto& slot : slots)
    {
        slot->draw(_window);
    }
}

void LobbyUI::initSlots()
{
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        slots[i] = std::make_unique<LobbySlot>(asset_manager->loadFont(DEFAULT_FONT));
        auto& slot = slots[i];

        slot->setRectangleSize({ 400.0f, 80.0f });
        slot->setPosition({ WINDOW_WIDTH / 2.0f, 150.0f + (100.0f * i) });

        slot->setPlayerIDText("Player " + std::to_string(i));
        slot->setOccupiedColor(JHelper::evaluateSFColor(JHelper::idToCellValue(i)));

        slot->setOccupied(false);
    }
}

std::string LobbyUI::playerStateToString(const PlayerState& _state) const
{
    switch (_state)
    {
        case PlayerState::NOTREADY:         return "Not Ready";
        case PlayerState::READY:            return "Ready";
        case PlayerState::PLAYING:          return "Playing";
        case PlayerState::VIEWING_RESULTS:   return "Viewing Results";

        default: return "Empty";
    }
}
