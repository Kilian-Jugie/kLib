#include "kDataManager.h"


void kDataDocument::refresh() {
	std::wifstream in(m_Path, std::ios::binary);
	if (!in.is_open()) {
		this->write();
		return;
	}
	std::wstring line;
	while (!in.eof()) {
		std::getline(in, line);
		if(!line.empty())
			parseLine(line);
	}
	in.close();
	m_NeedReload = false;
}

void kDataDocument::parseLine(const std::wstring& line) {
	size_t equal = line.find_first_of('=');
	std::wstring name(line.substr(2, equal-2));
	std::wstring value(line.substr(equal + 1));
	switch (line[0]) {
	case 's': {
		m_VStr.push_back(kDataVariable<data_string>{value, name});
	}break;
	case 'i': {
		m_VInt.push_back(kDataVariable<data_int>{static_cast<data_int>(std::stoll(value)), name});
	}break;
	case 'd': {
		m_VDouble.push_back(kDataVariable<data_double>{static_cast<data_double>(std::stod(value)), name});
	}break;
	default:
		break;
	}
}

void kDataDocument::write() {
	std::wofstream of(m_Path, std::ios::binary);
	writeArray<data_string>(m_VStr, 's', of);
	writeArray(m_VDouble, 'd', of);
	writeArray(m_VInt, 'i', of);
	of.close();
	m_NeedWrite = false;
}
