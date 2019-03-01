#include "kString.h"
#include <cstdlib>

	kString::kString(std::string str) {
		m_Data.assign(str.begin(), str.end());
	}

	std::string kString::getString() const {
		return std::string(m_Data.begin(), m_Data.end());
	}

	kString::operator std::wstring() const {
		return m_Data;
	}

	kString::operator std::string() const {
		return std::string(m_Data.begin(), m_Data.end());
	}
	kString kString::operator+(const kString& other) {
		return m_Data + other.getWString();;
	}
	kString kString::operator+(const char * other) {
		return m_Data + kString(other).getWString();
	}
	kString kString::operator+(const wchar_t* other) {
		return m_Data + kString(other).getWString();
	}
	kString kString::operator=(const std::string & data) {
		m_Data.assign(data.begin(), data.end());
		return *this;
	}
	kString kString::operator=(const std::wstring& data) {
		m_Data = data;
		return *this;
	}
	kString kString::operator=(const kString& data) {
		m_Data = data.getWString();
		return *this;
	}
	wchar_t kString::operator[](size_t i) {
		return m_Data[i];
	}
	size_t kString::itToIndex(const iterator &it) {
		return it - this->begin();
	}
	kString::iterator kString::indexToIt(const size_t & i) {
		return iterator(this->begin()+i);
	}
	void kString::insert(iterator pos, iterator _First, iterator _Last) {
		m_Data.insert(pos, _First, _Last);
	}
	void kString::insert(size_t pos, const std::wstring& str) {
		m_Data.insert(pos, str);
	}

//User defined literals


