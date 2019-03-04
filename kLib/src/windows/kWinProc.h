#pragma once
#include <core/kDefines.h>
#include <string>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef KG_UNICODE
#error TODO: IMPLEMENT NON UNICODE (see constructor)
#endif // !KG_UNICODE


#include <windows/kWindows.h>

#define K_VER_WPROC 1,0,0

class kWinProc {
public:

	kWinProc(_In_ kLPCSTR cmdLine, _In_opt_ DWORD creationFlags = NULL) : m_CreFlags(creationFlags) {
		m_Cmd = new wchar_t[MAX_PATH];
		wcscpy_s((wchar_t*)m_Cmd, MAX_PATH, (const wchar_t*)cmdLine);
		wcscat_s((wchar_t*)m_Cmd, MAX_PATH, L"\0");
		ZeroMemory(&m_StartInfo, sizeof(m_StartInfo));
		m_StartInfo.cb = sizeof(m_StartInfo);

		ZeroMemory(&m_ProcInfo, sizeof(m_ProcInfo));
	}

	~kWinProc();

	//Not working
	void setRedirectHandle(_In_ bool redirect = false);
	//Not implemented
	void setUseSize(_In_ bool useSize = false);
	
	_Check_return_
	bool start(_In_ bool bloquant = true);
	void wait(_In_ DWORD time=INFINITE);
	void finish();

	int getReturnCode();
	
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