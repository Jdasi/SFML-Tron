#include <Game/FileIO.h>
#include <Game/ServerSettings.h>
#include "TronServer.h"

int main()
{
    TronServer server;
    server.run(FileIO::retrieveServerSettings().tcp_port);

    return 0;
}

