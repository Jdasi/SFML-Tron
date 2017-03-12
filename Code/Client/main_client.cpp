#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#if (defined(_MSC_VER))
#include <windows.h>
#endif

#include "TronClient.h"

int main()
{
#if (defined(_MSC_VER))
    // Debug console.
    AllocConsole();
    freopen("CONOUT$", "wt", stdout);
    SetConsoleTitle(L"Debug Console");
#endif

    TronClient client;
    client.run();

	return 0;
}
