#pragma once
#include <memory>

#include <SFML/Network.hpp>

class User
{
public:
    User(sf::Uint8 _id);
    ~User() = default;

    sf::Uint8 getID() const;

    const std::string& getName() const;
    void setName(const std::string& _name);

    sf::TcpSocket* getSocket() const;
    void resetSocket();

    double getLatency() const;
    void setLatency(double _latency);

private:
    std::unique_ptr<sf::TcpSocket> socket;
    sf::Uint8 id;
    std::string name;
    double latency;

};
