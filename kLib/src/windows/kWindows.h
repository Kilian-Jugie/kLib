#pragma once
#include <core/kDefines.h>
#include <Windows.h>

#ifndef KG_WIN
#error This library is only intended to be builded for windows
#endif // 

#ifdef UNICODE
using kLPSTR = LPWSTR;
using kLPCSTR = LPCWSTR;
#else
using kLPSTR = LPSTR;
using kLPCSTR = LPCSTR;
#endif // UNICODE

#define K_VER_WIN 1,0,0