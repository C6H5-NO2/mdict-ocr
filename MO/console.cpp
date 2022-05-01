#include "console.h"

#include <Windows.h>

#include <cstdio>


namespace MO {
    void RedirectOutput() {
        AllocConsole();
        SetConsoleTitle(L"Debug Log (DO NOT CLOSE)");
        // prevent closing console accidentally
        DeleteMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);

        FILE* fp = nullptr;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);

        printf_s("!!! DO NOT CLOSE CONSOLE WINDOW DIRECTLY !!!\n");
    }


    void RestoreOutput() {
        FreeConsole();

        FILE* fp = nullptr;
        freopen_s(&fp, "NUL:", "w", stdout);
        freopen_s(&fp, "NUL:", "w", stderr);
    }
}
