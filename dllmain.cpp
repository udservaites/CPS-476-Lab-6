// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#include "stdio.h"



typedef int(*__stdcall MainWndProc)(HWND, int, int, int);
typedef int(*__stdcall CleanUp)(HWND);
typedef int(*__stdcall MOVECARDS)(HWND);

HHOOK hkb;
KBDLLHOOKSTRUCT kbdStruct;

HACCEL newFreeMenu;
HACCEL origFreeMenu;



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
	HWND hWnd = *(HWND *)0x01008374;
	//ensures moves continue to occur
	*(unsigned int *)0x01007864 = 1;
	//set cards to zero, so you can win
	//if cards not zero, there are cards left to play
	*(unsigned int *)0x01007800 = 0;

	//play the rest of the cards
	MOVECARDS mcFn = (MOVECARDS)0x01004FC7;
	mcFn(hWnd);
}

//for problem 4
void MessageHandler(HWND hWnd, LPMSG uMsg, WPARAM wParam, LPARAM lParam) {

	if (!TranslateAccelerator(hWnd, newFreeMenu, uMsg))
	{
		TranslateMessage(uMsg);
		DispatchMessage(uMsg);
	}
}

LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	char buf[200];

	LPMSG lMsg = (LPMSG)lParam;
	HWND hWnd;
	hWnd = *(HWND *)0x01008374;

	if (TranslateAccelerator(hWnd, newFreeMenu, lMsg) == 0)
	{
		TranslateMessage(lMsg);
		DispatchMessage(lMsg);
	}
	else {

		if (lMsg->lParam == 0x3C0001) {
			handleFreecell_5();
			return 0;
		}
	}

	return CallNextHookEx(hkb, nCode, wParam, lParam);
}

void handleFreecell_4and5(HMODULE hmod)
{
	newFreeMenu = LoadAccelerators(hmod, L"FreeMenu");

	char buf[200];

	
	HWND hWnd = *(HWND *)0x01008374;

	DWORD dwProcID = GetCurrentProcessId();
	

	DWORD dwThreadID = GetWindowThreadProcessId(hWnd, &dwProcID);
	

	HWND pgmHwnd;
	pgmHwnd = *(HWND *)0x01008374;
	
	hkb = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)HookCallback, NULL, dwThreadID);

	MSG msg;
	BOOL bRet;

	while (bRet = GetMessage(&msg, NULL, 0, 0));
		
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
		handleFreecell_4and5(hModule);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

