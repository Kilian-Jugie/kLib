#pragma once
/*
	This API is a Windows API Wrapper to make use of
	wslApi usable in REAL code (not in pseudo windows
	code)

	WARNING: THIS FILE IS VERY INSTABLE AND SUBJECT TO
	MAJOR CHANGES INCLUDING ITS SUPPRESSION DEPENDING
	OF RELEASING OF Wslapi.lib !

	THIS FILE MIGHT BE NOT USED IN OFFICIAL RELEASE !

	IMPLEMENTATION NEEDED: Wslapi.lib (not released)
*/
#include <windows/kWindows.h>
#include <wslapi.h>

typedef BOOL(STDAPICALLTYPE *_isDistribRegistered_t)(PCWSTR distribName);
typedef HRESULT(STDAPICALLTYPE *_wslRegisterDistrib_t)(PCWSTR distribName, PCWSTR tarGzFilename);
typedef HRESULT(STDAPICALLTYPE *_wslUnregisterDistrib_t)(PCWSTR distribName);
typedef HRESULT(STDAPICALLTYPE *_wslConfigureDistrib_t)(PCWSTR distribName, ULONG uid, WSL_DISTRIBUTION_FLAGS wslDistribFlags);
typedef HRESULT(STDAPICALLTYPE* _wslGetDistribConfiguration_t)(PCWSTR distribName,
	ULONG* distribVersion, ULONG* uid, WSL_DISTRIBUTION_FLAGS* wslDistribFlags, PSTR** defaultEnvVariables, ULONG* defaultEnvVariableCount);
typedef HRESULT(STDAPICALLTYPE *_wslLaunchInteractive_t)(PCWSTR distribName, PCWSTR cmd, BOOL useCurrentWorkingDir, DWORD* exitCode);
typedef HRESULT(STDAPICALLTYPE *_wslLaunch_t)(PCWSTR distribName, PCWSTR cmd, BOOL use, HANDLE stdIn, HANDLE stdOut, HANDLE stdErr, HANDLE* process);

class kWinWslAPI {
public:
	kWinWslAPI() : m_Dll(LoadLibraryExW(L"wslapi.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32)),
		_isDistribRegistered((_isDistribRegistered_t)GetProcAddress(m_Dll, "WslIsDistributionRegistered")),
		_wslRegisterDistrib((_wslRegisterDistrib_t)GetProcAddress(m_Dll, "WslRegisterDistribution")),
		_wslUnregisterDistrib((_wslUnregisterDistrib_t)GetProcAddress(m_Dll, "WslUnregisterDistribution")),
		_wslConfigureDistrib((_wslConfigureDistrib_t)GetProcAddress(m_Dll, "WslConfigureDistribution")),
		_wslGetDistribConfiguration((_wslGetDistribConfiguration_t)GetProcAddress(m_Dll, "WslGetDistributionConfiguration")),
		_wslLaunchInteractive((_wslLaunchInteractive_t)GetProcAddress(m_Dll, "WslLaunchInteractive")),
		_wslLaunch((_wslLaunch_t)GetProcAddress(m_Dll, "WslLaunch"))
	{}

	~kWinWslAPI() {
		FreeLibrary(m_Dll);
	}
	
	BOOL isDistribRegistered(PCWSTR distribName) { return _isDistribRegistered(distribName); }
	HRESULT wslRegisterDistribution(PCWSTR distribName, PCWSTR tarGzFilename) { return (*_wslRegisterDistrib)(distribName, tarGzFilename); }
	HRESULT wslUnregisterDistribution(PCWSTR distribName) { return (*_wslUnregisterDistrib)(distribName); }
	HRESULT wslConfigureDistribution(PCWSTR distribName, ULONG uid, WSL_DISTRIBUTION_FLAGS wslDistribFlags) {
		return (*_wslConfigureDistrib)(distribName, uid, wslDistribFlags);
	}
	HRESULT wslGetDistributionConfiguration(PCWSTR distribName, ULONG* distribVersion, 
		ULONG* uid, WSL_DISTRIBUTION_FLAGS* wslDistribFlags, PSTR** defaultEnvVariables, ULONG* defaultEnvVariableCount) {
		return (*_wslGetDistribConfiguration)(distribName, distribVersion, uid, wslDistribFlags, defaultEnvVariables, defaultEnvVariableCount);
	}
	HRESULT wslLaunchInteractive(PCWSTR distribName, PCWSTR cmd, BOOL useCurrentWorkingDir, DWORD* exitCode) {
		return (*_wslLaunchInteractive)(distribName, cmd, useCurrentWorkingDir, exitCode); 
	}
	HRESULT wslLaunch(PCWSTR distribName, PCWSTR cmd, BOOL use, HANDLE stdIn, HANDLE stdOut, HANDLE stdErr, HANDLE* process) {
		return (*_wslLaunch)(distribName, cmd, use, stdIn, stdOut, stdErr, process); 
	}

private:
	HMODULE m_Dll;

	_isDistribRegistered_t _isDistribRegistered;
	_wslRegisterDistrib_t _wslRegisterDistrib;
	_wslUnregisterDistrib_t _wslUnregisterDistrib;
	_wslConfigureDistrib_t _wslConfigureDistrib;
	_wslGetDistribConfiguration_t _wslGetDistribConfiguration;
	_wslLaunchInteractive_t _wslLaunchInteractive;
	_wslLaunch_t _wslLaunch;

};