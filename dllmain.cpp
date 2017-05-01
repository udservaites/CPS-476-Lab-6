// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include "stdafx.h"
#include "resource.h"



typedef int(*__stdcall MainWndProc)(HWND, int, int, int);
typedef int(*__stdcall CleanUp)(HWND);

IDR_ACCEL1 ACCELERATORS //declaration not quite right
{
	VK_F1, 106, VIRTKEY
	VK_F1, 106, SHIFT, VIRTKEY
	VK_F2, 40027, CONTROL, SHIFT, VIRTKEY //Wins game, not sure if code is correct
	VK_F6, 114, CONTROL, SHIFT, VIRTKEY //F6 = Abort, Retry, Ignore 
	VK_F3, 103, VIRTKEY
	VK_F4, 105, VIRTKEY
	VK_F5, 109, VIRTKEY
	VK_F10, 115, VIRTKEY
	VK_F10, 106, VIRTKEY
}

HHOOK handle_1_hook;
HINSTANCE* hinstance = (HINSTANCE*)0x1007860;
LRESULT __stdcall MessageProc(int nCode, WPARAM wParam, LPARAM lParam) {

}
void handleFreecell_1() {
	//handle_1_hook = SetWindowsHookEx(WH_CALLWNDPROC, MessageProc, *hinstance, 0);

	


}

void handleFreecell_2() {
	// store key in freecell mem
	HKEY* _hkey = (HKEY*)0x10079A4;
	// pointer to registry path
	WCHAR* _pszRegPath = (WCHAR*)0x1001230;
	// pointer to pszWon variable
	WCHAR* _pszWon = (WCHAR*)0x10012A8;
	// create registry key - store in _hkey
	RegCreateKeyW((HKEY)2147483649, _pszRegPath, _hkey);
	// set registry at _pszWon to data
	DWORD data = 1000;
	RegSetValueExW(*_hkey, _pszWon, 0, 3, (LPBYTE)&data, 4);
	// close registry key
	RegCloseKey(*_hkey);
}

void handleFreecell_3() {
	//cheat variable = 2
	*(unsigned int *)0x1007130 = 2; 
}

void handleFreecell_4() {
	//accelerator table
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL1));


	//Matt's code of death
	//Trying the Accelerator Table here
	//LPACCEL myTable = new ACCEL[1];

	/*//For the Shift Control F6
	myTable[1].cmd = 106;
	myTable[1].key = VK_F6;
	myTable[1].fVirt = FSHIFT | FCONTROL;

	HACCEL hAccel = CreateAcceleratorTable(myTable, 1);
	HWND* hWnd = (HWND*)0xDFD1C;
	MSG lpMsg;
	TranslateAcceleratorW(*hWnd, hAccel, &lpMsg);
	/////////////////////////////////////////////*/
	/*while (1) {
		//UnregisterHotKey(NULL, VK_SHIFT & VK_CONTROL & VK_F10);
		if ((GetKeyState(VK_SHIFT) & GetKeyState(VK_CONTROL) & GetKeyState(VK_F6)) & 0x8000) {
			/*MessageBoxA(0, "HI", "Hello", 1);
			WPARAM* ptr = (WPARAM*)0x10072;
			HWND* hwnd = (HWND*)GetForegroundWindow();;
			MainWndProc funcCheat = (MainWndProc)0x1001AF9;
			funcCheat(*hwnd, 0, (int)ptr, 0);*/
		/*	break;
		}
		if ((GetKeyState(VK_SHIFT) & GetKeyState(VK_CONTROL) & GetKeyState(VK_F2)) & 0x8000) {
			*(unsigned int *)0x1007130 = 2;
			MessageBoxA(0, "HI1", "Hello", 1);
			//HWND* hwnd2 = (HWND*)0x7452BC;
			HWND hwnd2 = GetForegroundWindow();
			if (hwnd2 == NULL) {
				MessageBoxA(0, "HI@", "Hello", 1);
			}

			//MessageBoxA(0, "Done1", "Done", 1);
			CleanUp dc = (CleanUp)0x1004FC7;
			MessageBoxA(0, "HI3", "Hello", 1);
			//MessageBoxA(0, "Done2", "Done", 1);
		/*	dc(hwnd2);
			MessageBoxA(0, "Done3", "Done", 1);
			break;
		}
		Sleep(100);
	}*/
	





BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxA(NULL, "Becca Rocks!", "Alert", 1);
		handleFreecell_2();
		//handleFreecell_3();
		//handleFreecell_4();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

