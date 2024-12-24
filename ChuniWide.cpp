#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <cstring>
#include <TlHelp32.h>
#include <stdlib.h>

using namespace std;

uintptr_t GetModuleBaseAddress(DWORD64, const wchar_t*);
DWORD64 GetProcId(const wchar_t*);

int main()
{
	cout << "Chunithm Luminous Plus positioner By VELIUS" << endl;
	cout << "Please start Chunithm now" << endl;
	Sleep(20000);
	HWND hwnd = FindWindowA(NULL, "teaGfx DirectX Release");
	HWND foreground = GetForegroundWindow();
	if (hwnd == NULL)
	{
		cout << "Cannot find window. Make sure Chuni has been started." << endl;
		Sleep(3000);
		exit(-1);
	}
	else
	{
		//DWORD64 procID = GetProcId(L"spice64.exe");
		DWORD64 procID = GetProcId(L"chusanApp.exe");

		if (procID == NULL) {
			cout << "Cannot obtain process." << endl;
			Sleep(3000);
			exit(-1);
		}
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);
		if (handle == NULL || handle == INVALID_HANDLE_VALUE)
		{
			cout << "Handle not found. Try running as Admin and try again." << endl;
			cout << "Error code: " << GetLastError();
			Sleep(3000);
			exit(-1);
		}
		else
		{
			uintptr_t Base = GetModuleBaseAddress(procID, L"chusanApp.exe");
			cout << Base << endl;
			unsigned int gameplayread;
			DWORD64 gameplayaddress = Base + 0x1E63B1C;
			int gameplayflag = 0;
			RECT rect;
			GetWindowRect(hwnd, &rect);
			rect.left = 0;
			rect.top = 0;
			for (;;) {
				foreground = GetForegroundWindow();
				if (hwnd == foreground)
				{
					break;
				}
				Sleep(1000);
			}
			//SetWindowLong(hwnd, -16, 335544320);
			//SetWindowPos(hwnd, 0, rect.left, rect.top, 2560, 1440, SWP_NOMOVE);
			//SetWindowPos(hwnd, 0, rect.left, rect.top, 2560, 1440, SWP_NOMOVE);
			//SetWindowPos(hwnd, 0, rect.left, rect.top, 2400, 1350, SWP_NOMOVE);
			for (;;)
			{

				ReadProcessMemory(handle, (BYTE*)gameplayaddress, &gameplayread, sizeof(gameplayread), 0);
				//cout << gameplayread << endl;
				//if LDJ, set to 800 otherwise 1024
				if (gameplayread == 16 && gameplayflag == 0)
				{
					cout << "Trying to SET POSITION" << endl;
					SetWindowPos(hwnd, 0, rect.left - 288, rect.top - 324, 2496, 1404, SWP_SHOWWINDOW);
					//SetWindowPos(hwnd, 0, rect.left - 144, rect.top - 162, 2208, 1242, SWP_SHOWWINDOW); 27 in
					//SetWindowPos(hwnd, 0, rect.left + 480, rect.top - 200, 2400, 1350, SWP_SHOWWINDOW); Use for P2
					//SetWindowPos(hwnd, 0, rect.left + 240, rect.top - 200, 2400, 1350, SWP_SHOWWINDOW); Use for DP
					gameplayflag = 1;
				}
				//cout << resultscreenread << endl;
				if (gameplayread == 24 && gameplayflag == 1)
				{
					SetWindowPos(hwnd, 0, rect.left, rect.top, 1920, 1080, SWP_SHOWWINDOW);
					gameplayflag = 0;
				}

				Sleep(500);
			}
			
		}
	}
}

uintptr_t GetModuleBaseAddress(DWORD64 procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
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

DWORD64 GetProcId(const wchar_t* procName)
{
	DWORD procId;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));

		}
	}
	CloseHandle(hSnap);
	return procId;
}
// R
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
