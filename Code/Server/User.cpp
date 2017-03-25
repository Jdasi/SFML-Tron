#include "User.h"

User::User(sf::Uint8 _id)
    : socket(std::make_unique<sf::TcpSocket>())
    , id(_id)
    , latency(0)
{
}

sf::Uint8 User::getID() const
{
    return id;
}

const std::string& User::getName() const
{
    return name;
}

void User::setName(const std::string& _name)
{
    name = _name;
}

sf::TcpSocket* User::getSocket() const
{
    return socket.get();
}

void User::resetSocket()
{
    socket.reset();
}

double User::getLatency() const
{
    return latency;
}

void User::setLatency(double _latency)
{
    latency = _latency;
}
