#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>

#include <Game/TronGame.h>

int main()
{
    // Debug console.
    AllocConsole();
    freopen("CONOUT$", "wt", stdout);
    SetConsoleTitle(L"Debug Console");

    TronGame game;
    game.run();

	return 0;
}