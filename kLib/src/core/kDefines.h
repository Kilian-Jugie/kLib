#pragma once

#ifndef _MSC_VER
#error only MSVC is supported !
#endif // !_MSC_VER

#ifndef __cplusplus
#error kLib is a C++ library (not an AINSI C)
#endif // !__cplusplus


#define K_VER_KLIB 1,1,0
#define K_VER_DEFS 1,0,0

#if defined(_M_AMD64)
#define KG_AMD64
#define KG_BIT64
#elif defined(_M_X64)
#define KG_BIT64
#elif defined(_M_IX86)
#define KG_IX86
#define KG_BIT32
#elseif defined(_M_ARM64)
#define KG_ARM64
#define KG_BIT64
#elif defined(_M_ARM)
#define KG_ARM
#define KG_BIT32
#endif // _M_AMD64

#if _MSVC_LANG == 201402L
#define KG_CPP 14
#elif _MSVC_LANG == 201703L
#define KG_CPP 17
#else
#define KG_CPP 20 //Value to change
#endif // _MSVC_LANG == 201402L

#ifdef _WIN32
#define KG_WIN
#ifdef _WIN64
#define KG_WIN64
#endif // _WIN64
#endif // _WIN32

#ifdef UNICODE
#define KG_UNICODE
#endif // UNICODE

