#pragma once
#include <memory>

#include <SFML/Network.hpp>

#include <Game/PlayerState.h>

class Client
{
public:
    explicit Client(const sf::Uint8 _id);
    ~Client() = default;

    sf::TcpSocket* getSocket() const;
    void resetSocket();

    sf::Uint8 getID() const;
    void setID(const sf::Uint8 _id);

    PlayerState getState() const;
    void setState(const PlayerState _state);

    const std::string& getName() const;
    void setName(const std::string& _name);

    double getLatency() const;
    void setLatency(const double _latency);

private:
    std::unique_ptr<sf::TcpSocket> socket;
    sf::Uint8 id;
    PlayerState state;
    std::string name;
    double latency;

};
