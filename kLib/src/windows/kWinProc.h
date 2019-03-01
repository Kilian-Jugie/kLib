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
	kWinProc(kLPSTR cmdLine, DWORD creationFlags = NULL, STARTUPINFO* startInfo=nullptr);
	~kWinProc();

	//Not working
	void setRedirectHandle(bool redirect = false);
	//Not implemented
	void setUseSize(bool useSize = false);
	

	bool start(bool bloquant = true);
	void wait(DWORD time=INFINITE);
	void finish();

//private:
	kLPSTR m_Cmd;
	STARTUPINFO m_StartInfo;
	PROCESS_INFORMATION m_ProcInfo;
	SECURITY_ATTRIBUTES m_SecurityAttr;
	DWORD m_CreFlags;
	bool m_IsStarted = false;
	bool m_CustomSecurity = false;
};