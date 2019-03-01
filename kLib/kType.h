#pragma once
#include "kPreprocessor.h"
#include <string>
#define K_DEFTYPE_IF_NOT(from, to) 


#ifndef UNICODE
using string_t = std::string;
using char_t = char;
#else
using string_t = std::wstring;
using char_t = wchar_t;
#endif // !UNICODE

using kErrorCode_t = unsigned long;