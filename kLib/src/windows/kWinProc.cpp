#include <windows/kWinProc.h>

kWinProc::~kWinProc() {
	finish();
	delete[] m_Cmd;
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
	//bool r = CreateProcess(NULL, m_Cmd, m_CustomSecurity?&m_SecurityAttr:NULL,
		//m_CustomSecurity ? &m_SecurityAttr : NULL, FALSE, m_CreFlags, NULL, NULL, &m_StartInfo, &m_ProcInfo);
	bool r = CreateProcess(NULL, m_Cmd, NULL, NULL, FALSE, NULL, NULL, NULL, &m_StartInfo, &m_ProcInfo);
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

int kWinProc::getReturnCode() {
	DWORD exitCode;
	GetExitCodeProcess(m_ProcInfo.hProcess, &exitCode);
	return exitCode;
}
