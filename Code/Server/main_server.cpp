#include <Game/Constants.h>
#include "TronServer.h"

int main()
{
    TronServer server;
    server.run(SERVER_TCP_PORT);

    return 0;
}

