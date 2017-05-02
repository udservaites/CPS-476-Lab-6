// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#include "stdio.h"



typedef int(*__stdcall MainWndProc)(HWND, int, int, int);
typedef int(*__stdcall CleanUp)(HWND);
typedef int(*__stdcall MOVECARDS)(HWND);

/*IDR_ACCEL1 ACCELERATORS
{
	VK_F1, ID_UNKNOWN, VIRTKEY
	VK_F1, ID_UNKNOWN, SHIFT, VIRTKEY
	VK_F2, ID_FORCEWIN, CONTROL, SHIFT, VIRTKEY //Wins game, not sure if code is correct
	VK_F6, ID_CHEAT, CONTROL, SHIFT, VIRTKEY //F6 = Abort, Retry, Ignore
	VK_F3, ID_SELECTGAME, VIRTKEY
	VK_F4, ID_STASTICS, VIRTKEY
	VK_F5, ID_OPTIONS, VIRTKEY
	VK_F10, ID_UNDO, VIRTKEY
	VK_F10, ID_UNKOWN, VIRTKEY
}
*/
HHOOK handle_1_hook;
HINSTANCE* hinstance = (HINSTANCE*)0x1007860;
LRESULT __stdcall MessageProc(int nCode, WPARAM wParam, LPARAM lParam) {

}

HWND FindMyTopMostWindow()
{
	DWORD dwProcID = GetCurrentProcessId();
	HWND hWnd = GetTopWindow(GetDesktopWindow());
	while (hWnd)
	{
		DWORD dwWndProcID = 0;
		GetWindowThreadProcessId(hWnd, &dwWndProcID);
		if (dwWndProcID == dwProcID)
			return hWnd;
		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	}
	return NULL;
}

void handleFreecell_1() {
	
	DWORD old_message;
	DWORD message_address = (DWORD)GetModuleHandle(NULL) + (DWORD)0x00010c04;
	const size_t string_length = 36;
	wchar_t string_buffer[string_length] = L"Not in this game";

	wchar_t* string_buffer_ptr = (wchar_t*)(message_address);

	VirtualProtect(string_buffer_ptr, string_length, PAGE_EXECUTE_READWRITE, &old_message);
	memcpy(string_buffer_ptr, string_buffer, string_length);
	VirtualProtect(string_buffer_ptr, string_length, old_message, nullptr);

}

void handleFreecell_2() {
	// get key from registry
	HKEY* _hkey = (HKEY*)0x10079A4;
	// create pointer to point to registry path
	WCHAR* _pszRegPath = (WCHAR*)0x1001230;
	// creat pointer to the pszWon variable in Freecell
	WCHAR* _pszWon = (WCHAR*)0x10012A8;
	// create registry key - store in _hkey
	RegCreateKeyW((HKEY)2147483649, _pszRegPath, _hkey);
	// _pszWon registry set to data 
	DWORD new_value = 1000;
	RegSetValueExW(*_hkey, _pszWon, 0, 3, (LPBYTE)&new_value, 4);
	// close reg key
	RegCloseKey(*_hkey);
}

void handleFreecell_3() {
	//cheat variable = 2
	*(unsigned int *)0x1007130 = 2;
}
void handleFreecell_5() {
	HWND hWnd = FindMyTopMostWindow();
	//ensures moves continue to occur
	*(unsigned int *)0x01007864 = 1;
	//set cards to zero, so you can win
	//if cards not zero, there are cards left to play
	*(unsigned int *)0x01007800 = 0;

	//play the rest of the cards
	MOVECARDS mcFn = (MOVECARDS)0x01004FC7;
	mcFn(hWnd);
}


	

void handleFreecell_4and5() {

	while (1) {	

		if ((GetKeyState(VK_SHIFT) & GetKeyState(VK_CONTROL) & GetKeyState(VK_F6)) & 0x8000) {
			//MessageBoxA(0, "HI", "Hello", 1);
			HWND hwndmain = FindMyTopMostWindow();
			MainWndProc proc = (MainWndProc)0x1001AF9;
			proc(hwndmain, 0, 0x72, 0);
		
			break;
		}
		if ((GetKeyState(VK_SHIFT) & GetKeyState(VK_CONTROL) & GetKeyState(VK_F2)) & 0x8000) {
			handleFreecell_5();

			break;
		}
		Sleep(100);
	}

}



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//MessageBoxA(NULL, "Becca Rocks!", "Alert", 1);
		handleFreecell_1();
		handleFreecell_2();
		handleFreecell_3();
		handleFreecell_4and5();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

