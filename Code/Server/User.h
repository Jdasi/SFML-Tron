#pragma once
#include <memory>

#include <SFML/Network.hpp>

class User
{
public:
    User(sf::Uint8 _id);
    ~User() = default;

    const sf::Uint8& getID() const;

    const std::string& getName() const;
    void setName(const std::string& _name);

    sf::TcpSocket* getSocket() const;
    void resetSocket();

    const sf::Uint64& getLatency() const;
    void setLatency(sf::Uint64 _latency);

private:
    std::unique_ptr<sf::TcpSocket> socket;
    sf::Uint8 id;
    std::string name;
    sf::Uint64 latency;

};
