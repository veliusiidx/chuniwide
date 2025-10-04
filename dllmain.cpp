#include "pch.h"
#include <windows.h>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <TlHelp32.h>

#define RESIZE_25_INCH

uintptr_t GetModuleBaseAddress(DWORD64, const wchar_t*);

DWORD WINAPI Search(LPVOID)
{
    HWND hwnd;
    wchar_t title[500];
    do {
        Sleep(3000);
        hwnd = GetForegroundWindow();
        GetWindowTextW(hwnd, title, 500);
    } while (wcscmp(title, L"teaGfx DirectX Release") != 0);

    auto gameplayflag = 0;
    auto procID = GetCurrentProcessId();
    auto addr = GetModuleBaseAddress(procID, L"chusanApp.exe") + 0x185BFA0;

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    while (1) {
        auto gameplayread = *((std::uint32_t*)addr);

        if (gameplayread > 0 && gameplayflag == 0)
        {
#ifdef RESIZE_25_INCH
            SetWindowPos(hwnd, 0, rect.left - 288, rect.top - 324, 2496, 1404, SWP_SHOWWINDOW);
#else
            SetWindowPos(hwnd, 0, rect.left - 144, rect.top - 162, 2208, 1242, SWP_SHOWWINDOW);
#endif
            gameplayflag = 1;
        }
        if (gameplayread == 0 && gameplayflag == 1)
        {
            SetWindowPos(hwnd, 0, rect.left, rect.top, 1920, 1080, SWP_SHOWWINDOW);
            gameplayflag = 0;
        }

        Sleep(500);
    }
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DWORD tid;
        auto thread = CreateThread(
            NULL,
            0,
            Search,
            NULL,
            0,
            &tid
        );

    }
    return TRUE;
}

uintptr_t GetModuleBaseAddress(DWORD64 procId, const wchar_t* modName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, (DWORD)procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}