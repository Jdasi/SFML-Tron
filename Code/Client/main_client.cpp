#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#include <Game/Constants.h>
#include "TronClient.h"

int main()
{
    // Debug console.
    AllocConsole();
    freopen("CONOUT$", "wt", stdout);
    SetConsoleTitle(L"Debug Console");

    TronClient client(SERVER_IP, SERVER_TCP_PORT);
    client.run();

	return 0;
}
