#include "pch.h"
#include <fcntl.h>
#include <io.h>

void fixUpLibDir();
void loadDlls();

void openConsole() {
    AllocConsole();
    SetConsoleTitleA("Debug Console");

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    DWORD mode = 0;
    GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mode);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    INT hCrt = _open_osfhandle((INT)hCon, _O_TEXT);
    FILE* hf = _fdopen(hCrt, "w");
    setvbuf(hf, NULL, _IONBF, 0);
    *stdout = *hf;                              
    freopen("CONOUT$", "w+t", stdout);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        openConsole();
        // For #683, Change CWD to current module path
        WCHAR cpath[MAX_PATH];
        GetModuleFileName(hModule, cpath, MAX_PATH);
        std::wstring path(cpath);
        auto cwd = path.substr(0, path.find_last_of('\\'));
        _wchdir(cwd.c_str());
		
        fixUpLibDir();
        loadDlls();
    }
    return TRUE;
}

