#pragma once
#include <queue>
#include <mutex>
#include <thread>
#include <vector>

#include "ClientState.h"

namespace sf
{
    class Text;
    class Drawable;
}

class ClientStateGame final : public ClientState
{
public:
    ClientStateGame(ClientData* _client_data);
    virtual ~ClientStateGame();

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void onCommand(const GameAction _action, const ActionState _action_state) override;

private:
    bool connect();
    void receive();
    void ping();
    void shutdown();
    void handlePacket(sf::Packet& _packet);

    bool networking;
    sf::Text* latency_text;

    std::thread receive_thread;
    std::thread ping_thread;

    sf::IpAddress ip_address;
    unsigned int tcp_port;
    sf::TcpSocket socket;

    std::queue<sf::Packet> pong_queue;
    std::mutex pong_queue_mutex;
    sf::Int64 latency;

    std::vector<std::unique_ptr<sf::Drawable>> objects;

};
