#include <klib.h>
#include <tests.h>
#include <iostream>

int main() {

	//wslExample();

	/*
	kWinProc proc((kLPSTR)L"cmd.exe");
	proc.start();

	std::cout << GetLastError() << " : " << proc.getReturnCode() << "\n";
	
	proc.finish();*/

	
	kWinProc proc(L"cmd /C echo test");
	proc.start(true);
	std::cout << GetLastError() << " : " << proc.getReturnCode() << "\n";
	proc.finish();
	
	wchar_t* cmd = new wchar_t[MAX_PATH];
	wcscpy_s(cmd, MAX_PATH, L"cmd /C echo test");
	wcscat_s(cmd, MAX_PATH, L"\0");
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	ZeroMemory(&pi, sizeof(pi));
	
	if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		std::cout << "ntr\n";
	}
	std::cout << "ok\n";

	WaitForSingleObject(pi.hProcess, 0xffffffff);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	delete[] cmd;

	system("pause");
	
	return 0;
}