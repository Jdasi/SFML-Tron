#include <SFML/Graphics.hpp>

#include <Game/Constants.h>
#include <Game/JHelper.h>
#include <Game/GameStateIDs.h>
#include "ClientStateEnd.h"
#include "ClientStateHandler.h"
#include "ClientData.h"
#include "NetworkManager.h"
#include "AssetManager.h"
#include "GameAudio.h"

ClientStateEnd::ClientStateEnd(ClientData* _client_data)
    : ClientState(_client_data)
{
    auto backdrop = std::make_unique<sf::Sprite>(
        *client_data->asset_manager->loadTexture(BACKDROP));
    drawables.push_back(std::move(backdrop));

    initVictorText();
    initBlackBar();
}



// Display victory message and play victory sound on entry.
void ClientStateEnd::onStateEnter()
{
    client_data->game_audio->playSound(GAME_OVER_CUE);

    std::string victory_string;
    
    if (client_data->client_id == client_data->victor_id)
    {
        victory_string.append("You win!");
    }
    else
    {
        victory_string.append("Player " + 
            std::to_string(client_data->victor_id + 1) + " Wins!");
    }

    victor_text.setFillColor(JHelper::evaluateSFColorFromPlayerID(client_data->victor_id));
    victor_text.setString(victory_string);

    JHelper::centerSFOrigin(victor_text);
}



void ClientStateEnd::onStateLeave()
{
}



void ClientStateEnd::tick()
{
}



void ClientStateEnd::draw(sf::RenderWindow& _window)
{
    for (auto& drawable : drawables)
    {
        _window.draw(*drawable);
    }

    _window.draw(black_bar);
    _window.draw(victor_text);
}



void ClientStateEnd::onCommand(const GameAction _action, const ActionState _action_state)
{
    if (_action == GameAction::ACCEPT ||
        _action == GameAction::BOOST ||
        _action == GameAction::QUIT)
    {
        if (_action_state == ActionState::PRESSED)
        {
            // Inform the server the client has gone back to lobby.
            client_data->network_manager->sendPlayerStateChange();
            getHandler()->queueState(STATE_LOBBY);
        }
    }
}



void ClientStateEnd::initVictorText()
{
    victor_text.setFont(*client_data->asset_manager->loadFont(DEFAULT_FONT));

    victor_text.setCharacterSize(60);
    victor_text.setStyle(sf::Text::Bold);
    victor_text.setPosition({ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 });
    victor_text.setOutlineColor(sf::Color::Black);
    victor_text.setOutlineThickness(2.0f);
}



void ClientStateEnd::initBlackBar()
{
    black_bar.setFillColor(sf::Color::Black);
    black_bar.setSize({ static_cast<float>(WINDOW_WIDTH), 100 });
    black_bar.setPosition(victor_text.getPosition());

    JHelper::centerSFOrigin(black_bar);
}
