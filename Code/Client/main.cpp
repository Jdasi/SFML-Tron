#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>

#include "Client.h"

int main()
{
    // Debug console.
    AllocConsole();
    freopen("CONOUT$", "wt", stdout);
    SetConsoleTitle(L"Debug Console");

    Client client;
    client.run();

	return 0;
}