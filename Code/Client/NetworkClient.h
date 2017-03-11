#pragma once

/* Interface class used by TronNetworkManager to decouple networking from
 * the TronClient.
 *
 * This provides a simplified interface inside TronNetworkManager for dealing
 * with events that should be passed to the client for processing.
 */
class INetworkClient
{
public:
    virtual ~INetworkClient() = default;

    virtual void onConnected() = 0;
    virtual void onDisconnected() = 0;

    virtual void onUpdatePingTime(const sf::Uint32 _ping) = 0;

};
