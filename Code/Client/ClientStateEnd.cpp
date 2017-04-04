#include <SFML/Graphics.hpp>

#include <Game/Constants.h>
#include <Game/JHelper.h>
#include "ClientStateEnd.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "NetworkManager.h"
#include "AssetManager.h"
#include "GameAudio.h"

ClientStateEnd::ClientStateEnd(ClientData* _client_data)
    : ClientState(_client_data)
{
    victor_text = std::make_unique<sf::Text>("",
        *client_data->asset_manager->loadFont(DEFAULT_FONT));

    victor_text->setCharacterSize(60);
    victor_text->setStyle(sf::Text::Bold);
    victor_text->setPosition({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 });
}

void ClientStateEnd::onStateEnter()
{
    std::string victory_string;
    
    if (client_data->client_id == client_data->victor_id)
    {
        victory_string.append("You win!");

        client_data->game_audio->playSound(WINNER_CUE);
    }
    else
    {
        victory_string.append("Player " + 
            std::to_string(client_data->victor_id + 1) + " Wins!");

        client_data->game_audio->playSound(LOSER_CUE);
    }

    victor_text->setFillColor(JHelper::evaluateSFColorFromPlayerID(client_data->victor_id));
    victor_text->setString(victory_string);

    JHelper::centerSFOrigin(*victor_text);
}

void ClientStateEnd::onStateLeave()
{
}

void ClientStateEnd::tick()
{
}

void ClientStateEnd::draw(sf::RenderWindow& _window)
{
    _window.draw(*victor_text);

    for (auto& drawable : drawables)
    {
        _window.draw(*drawable);
    }
}

void ClientStateEnd::onCommand(const GameAction _action, const ActionState _action_state)
{
    if (_action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->exit = true;
        }
    }

    if (_action == GameAction::ACCEPT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            client_data->network_manager->sendPlayerStateChange();
        }
    }
}
