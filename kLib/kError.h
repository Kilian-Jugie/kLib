#pragma once
#include "kType.h"
#include <exception>


class kError {
	string_t m_Error;
	kErrorCode_t m_Code;
public:

	operator std::exception() {
		
	}
};