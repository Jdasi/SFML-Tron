#include "User.h"

User::User()
    : socket(std::make_unique<sf::TcpSocket>())
{
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
