#pragma once
#include <memory>

#include <SFML/Network.hpp>

class User
{
public:
    User();

    const std::string& getName() const;
    void setName(const std::string& _name);

    sf::TcpSocket* getSocket() const;
    void resetSocket();

private:
    std::string name;
    std::unique_ptr<sf::TcpSocket> socket;

};
