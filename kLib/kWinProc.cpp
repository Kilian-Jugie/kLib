#include "kWinProc.h"

kWinProc::kWinProc(kLPSTR cmdLine, DWORD creationFlags, STARTUPINFO * startInfo) : m_Cmd(cmdLine),
m_CreFlags(creationFlags) {
	if (!startInfo) {
		ZeroMemory(&m_StartInfo, sizeof(m_StartInfo));
		m_StartInfo.cb = sizeof(m_StartInfo);
	}
	else
		m_StartInfo = *startInfo;

	ZeroMemory(&m_ProcInfo, sizeof(m_ProcInfo));
}

kWinProc::~kWinProc() {
	finish();
}

void kWinProc::setRedirectHandle(bool redirect) {
	m_CustomSecurity = redirect;
	m_SecurityAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	m_SecurityAttr.bInheritHandle = redirect;
	m_SecurityAttr.lpSecurityDescriptor = NULL;
}

void kWinProc::setUseSize(bool useSize) {

}

bool kWinProc::start(bool bloquant) {
	bool r = CreateProcess(NULL, m_Cmd, m_CustomSecurity?&m_SecurityAttr:NULL,
		m_CustomSecurity ? &m_SecurityAttr : NULL, FALSE, m_CreFlags, NULL, NULL, &m_StartInfo, &m_ProcInfo);
	m_IsStarted = r;
	if (r&&bloquant)
		wait();
	return r;
}

void kWinProc::wait(DWORD time) {
	if (m_IsStarted)
		WaitForSingleObject(m_ProcInfo.hProcess, time);
}

void kWinProc::finish() {
	if (m_IsStarted) {
		CloseHandle(m_ProcInfo.hProcess);
		CloseHandle(m_ProcInfo.hThread);
	}
}
