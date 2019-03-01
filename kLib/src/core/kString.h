#pragma once
#include <string>

#define K_VER_KSTRING 1,0,0

//THIS LIBRARY IS IN DEVELOPPMENT AND MIGHT BE NOT USED

/*
	\brief ETString: Custom string class with compatibility with wstring AND string and some custom functions
*/
class kString {
	std::wstring m_Data; //Data as wstring to contain UTF-16 characters
public:
	//Constructors from std or c strings
	//Warning: this constructor is the slowest if can you may avoid it
	kString(std::string str);
	inline kString(const wchar_t* cstr) : m_Data(cstr) {}
	inline kString(const std::wstring& wstr) : m_Data(wstr) {}
	inline kString(const long long i) : m_Data(std::to_wstring(i)) {}
	inline kString(const kString& k) : m_Data(k.m_Data) {}
	//This may not used
	inline kString() {}

	//Getters
	inline std::wstring getWString() const { return m_Data; }
	std::string getString() const;

	//Conversion to std strings
	operator std::wstring() const;
	operator std::string() const;

	//Operator+
	kString operator+(const kString&);
	kString operator+(const char*);
	kString operator+(const wchar_t*);

	//Operator=
	kString operator=(const std::string&);
	kString operator=(const std::wstring&);
	kString operator=(const kString&);

	//Operator[] (Standard)
	wchar_t operator[](size_t);

	//Standard iterator functions
	using iterator = std::wstring::iterator;
	using const_iterator = std::wstring::const_iterator;

	inline iterator begin() { return m_Data.begin(); }
	inline iterator end() { return m_Data.end(); }

	//Custom iterator functions
	size_t itToIndex(const iterator&);
	iterator indexToIt(const size_t&);

	//Standard basic_string functions
	inline size_t size() const { return m_Data.size(); }
	inline const wchar_t* c_str() const { return m_Data.c_str(); }
	void insert(iterator pos, iterator _First, iterator _Last);
	void insert(size_t pos, const std::wstring& str);
};

namespace {
	kString operator"" _k(const char* c, size_t) {
		return kString(c);
	}

	kString operator"" _k(const wchar_t* c, size_t) {
		return kString(c);
	}

	kString operator"" _k(unsigned long long c) {
		return kString(std::to_wstring(c));
	}

	//Custom ostream integration
	inline std::wostream& operator<<(
		std::wostream& _Ostr,
		const kString& _Str) {
		_Ostr.write(_Str.c_str(), _Str.size());
		return _Ostr;
	}

	inline const kString& operator>>(
		std::wistream& _Istr,
		const kString& _Str) {
		std::wstring str;
		_Istr >> str;
		return kString(str);
	}
}