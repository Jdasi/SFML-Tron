#include <Game/FileIO.h>
#include "TronClient.h"

int main()
{
    TronClient client(FileIO::retrieveServerSettings());
    client.run();

	return 0;
}
