#include <Game/Simulation.h>
#include <Game/JHelper.h>
#include <Game/Constants.h>
#include "LobbyUI.h"
#include "ClientData.h"
#include "AssetManager.h"
#include "GameManager.h"

LobbyUI::LobbyUI(ClientData* _client_data)
    : client_data(_client_data)
{
    initSlots();
}



/* Refreshes the displayed information of the UI based on the 
 * current state of the game_manager's list of players.
 */
void LobbyUI::refresh()
{
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        auto player = client_data->game_manager->getPlayer(i);
        auto slot = slots[i].get();

        if (!player)
        {
            slot->setOccupied(false);
            continue;
        }

        if (i == client_data->client_id)
        {
            slot->setPlayerIDText("You");
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



// Configures each slot with (currently) hard-coded position/size values.
void LobbyUI::initSlots()
{
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        auto& slot = slots[i];

        slot = std::make_unique<LobbySlot>(
            client_data->asset_manager->loadFont(DEFAULT_FONT));

        slot->setRectangleSize({ 400.0f, 80.0f });
        slot->setPosition({ WINDOW_WIDTH / 2.0f, 150.0f + (100.0f * i) });

        slot->setPlayerIDText("Player " + std::to_string(i + 1));
        slot->setOccupiedColor(JHelper::evaluateSFColorFromPlayerID(i));

        slot->setOccupied(false);
    }
}



// Helper function to translate a PlayerState into a readable string.
std::string LobbyUI::playerStateToString(const PlayerState& _state) const
{
    switch (_state)
    {
        case PlayerState::NOTREADY:         { return "Not Ready";       }
        case PlayerState::READY:            { return "Ready";           }

        case PlayerState::STARTING_GAME:    { return "Starting Game";   }
        case PlayerState::PLAYING:          { return "Playing";         }

        case PlayerState::VIEWING_RESULTS:  { return "Viewing Results"; }

        default: { return "Empty"; }
    }
}
