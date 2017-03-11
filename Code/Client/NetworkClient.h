#pragma once

class INetworkClient
{
public:
    virtual ~INetworkClient() = default;

    virtual void updatePingTime(const double ping) = 0;

};
