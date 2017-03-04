#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>

#include "TronClient.h"

int main()
{
    // Debug console.
    AllocConsole();
    freopen("CONOUT$", "wt", stdout);
    SetConsoleTitle(L"Debug Console");

    TronClient client("127.0.0.1", 53000);
    client.run();

	return 0;
}