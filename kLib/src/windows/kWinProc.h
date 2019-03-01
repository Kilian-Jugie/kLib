#pragma once
#include <core/kDefines.h>
#include <string>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows/kWindows.h>

#define K_VER_WPROC 1,0,0

class kWinProc {
public:
	kWinProc(_In_ kLPSTR cmdLine, _In_opt_ DWORD creationFlags = NULL, _In_opt_ STARTUPINFO* startInfo=nullptr);
	~kWinProc();

	//Not working
	void setRedirectHandle(_In_ bool redirect = false);
	//Not implemented
	void setUseSize(_In_ bool useSize = false);
	
	_Check_return_
	bool start(_In_ bool bloquant = true);
	void wait(_In_ DWORD time=INFINITE);
	void finish();
	
	_Check_return_
	PROCESS_INFORMATION getProcInfo() { return m_ProcInfo; }
private:
	kLPSTR m_Cmd;
	STARTUPINFO m_StartInfo;
	PROCESS_INFORMATION m_ProcInfo;
	SECURITY_ATTRIBUTES m_SecurityAttr;
	DWORD m_CreFlags;
	bool m_IsStarted = false;
	bool m_CustomSecurity = false;
};