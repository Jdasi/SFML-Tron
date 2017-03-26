#include "Client.h"

Client::Client(sf::Uint8 _id)
    : socket(std::make_unique<sf::TcpSocket>())
    , id(_id)
    , state(PlayerState::NOTREADY)
    , latency(0)
{
}

sf::TcpSocket* Client::getSocket() const
{
    return socket.get();
}

void Client::resetSocket()
{
    socket.reset();
}

sf::Uint8 Client::getID() const
{
    return id;
}

void Client::setID(sf::Uint8 _id)
{
    id = _id;
}

PlayerState Client::getState() const
{
    return state;
}

void Client::setState(PlayerState _state)
{
    state = _state;
}

const std::string& Client::getName() const
{
    return name;
}

void Client::setName(const std::string& _name)
{
    name = _name;
}

double Client::getLatency() const
{
    return latency;
}

void Client::setLatency(double _latency)
{
    latency = _latency;
}
