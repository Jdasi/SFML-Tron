#pragma once
#include <memory>

#include <SFML/Network.hpp>

#include <Game/PlayerState.h>

class Client
{
public:
    Client(sf::Uint8 _id);
    ~Client() = default;

    sf::TcpSocket* getSocket() const;
    void resetSocket();

    sf::Uint8 getID() const;
    void setID(sf::Uint8 _id);

    PlayerState getState() const;
    void setState(PlayerState _state);

    const std::string& getName() const;
    void setName(const std::string& _name);

    double getLatency() const;
    void setLatency(double _latency);

private:
    std::unique_ptr<sf::TcpSocket> socket;
    sf::Uint8 id;
    PlayerState state;
    std::string name;
    double latency;

};
