#pragma once
#include <core/kPreprocessor.h>
#include <string>
#include <filesystem>
#include <cassert>
#include <fstream>

#ifndef K_DATA_RELEASE
#define _k_i_dm_an(...) _k_i_an(0, __VA_ARGS__)
#define _K_IDMAN_REFER_TO(def, ...) KU_IF_ELSE(def)(_k_i_dm_an(__VA_ARGS__))()
#else
#define _k_i_dm_an(...) 
#define _K_IDMAN_REFER_TO(def, ...)
#endif // !RELEASE

#define K_VER_DATAMANAGER 1,0,0

_k_i_dm_an("TODO: support of kArray")
_k_i_dm_an("TODO: full support of UTF8")
_k_i_dm_an("TODO: full support change data type")
_k_i_dm_an("TODO: implement kDataType")


#ifndef K_DATA_NO_UNICODE
using dstring_t = std::wstring;
#define DSTR(x) L##x
#else
#error not supported yet
using dstring_t = std::string;
#define DSTR(x) x
#endif // !K_DATA_NO_UNICODE

#define K_DATA_USE_STD_CONTAINER
#ifndef K_DATA_USE_CUSTOM_ARRAY
#ifndef K_DATA_USE_STD_CONTAINER
#error kArray to be supported
#else
template<typename _Ty>using dvector_t = std::vector<_Ty>;
#endif // !K_DATA_USE_STD_CONTAINER
#endif // !K_DATA_USE_CUSTOM_ARRAY


//DO NOT USE: not implemented
enum class kDataType : unsigned {
	CONFIG,
	BINARY,
	NONE
};

/* Be carefull when changing that it can cause
	problems ! (!!! data_string !!!) */
using data_string = std::wstring;
using data_int = long long;
using data_double = double;
using data_any = void; //Not fully supported !

template<typename _Ty>class kDataVariable {
public:
	kDataVariable(const _Ty& v, const dstring_t& n) : m_Value(v), m_Name(n) {}
	kDataVariable(const kDataVariable& v) : m_Value(v.m_Value), m_Name(v.m_Name) {}

	kDataVariable& operator=(const kDataVariable& v) { m_Value = v.m_Value; m_Name = v.m_Name; }
private:
	_Ty m_Value;
	dstring_t m_Name;
	friend class kDataManager;
	friend class kDataDocument;
};

class kDataDocument {
public:
	kDataDocument(kDataType type, std::experimental::filesystem::path path, dstring_t id) : m_Type(type), m_Path(path), m_Id(id) {}
	kDataDocument() = default;
	kDataDocument(kDataDocument&&) = default;
	~kDataDocument() = default;
private:

	kDataDocument& operator=(kDataDocument&) = default;

	void refresh();
	void parseLine(const std::wstring&);
	void write();

	template<typename _Ty>void writeArray(dvector_t<kDataVariable<_Ty>> vec, char ty, std::wofstream& stream) {
		auto it(vec.begin());
		auto end(vec.end());
		for (; it != end; ++it)
			stream << ty << L":" << it->m_Name << "=" << it->m_Value << "\n";
	}

	//Hard-coded types : TODO flexibilize ?
	dvector_t<kDataVariable<data_string>> m_VStr;
	dvector_t<kDataVariable<data_int>> m_VInt;
	dvector_t<kDataVariable<data_double>> m_VDouble;

	kDataType m_Type;
	dstring_t m_Id;
	bool m_NeedReload = true;
	bool m_NeedWrite = false;
	std::experimental::filesystem::path m_Path;
	friend class kDataManager;
};

_k_i_dm_an("TODO: put def in .cpp")
class kDataManager {
public:
	kDataManager() = default;
	kDataManager(kDataManager&&) = default;
	~kDataManager() = default;

	void addDocument(const dstring_t& str, const dstring_t& id=DSTR("0"), kDataType type=kDataType::NONE) {
		m_Docs.push_back(kDataDocument(type, str, id));
	}

	_k_i_dm_an("TODO: avoid code copying (?)")
	template<typename _Ty>const _Ty& getValue(const dstring_t& name, const dstring_t& id = DSTR("0")) = delete;

	template<>const data_string& getValue<data_string>(const dstring_t& name, const dstring_t& id) {
		kDataDocument& doc(getDocument(id));
		_chkDcSte(doc);
		return getVariable(doc.m_VStr, name).m_Value;
	}

	template<>const data_int& getValue<data_int>(const dstring_t& name, const dstring_t& id) {
		kDataDocument& doc(getDocument(id));
		_chkDcSte(doc);
		return getVariable(doc.m_VInt, name).m_Value;
	}

	template<>const data_double& getValue<data_double>(const dstring_t& name, const dstring_t& id) {
		kDataDocument& doc(getDocument(id));
		_chkDcSte(doc);
		return getVariable(doc.m_VDouble, name).m_Value;
	}

	void setValue(const dstring_t& name, const data_string& value, const dstring_t& docId=DSTR("0")) {
		kDataDocument& doc(getDocument(docId));
		_chkDcSte(doc);
		getVariable(doc.m_VStr, name).m_Value = value;
		doc.write();
	}
	
	void setValue(const dstring_t& name, const data_int& value, const dstring_t& docId = DSTR("0")) {
		kDataDocument& doc(getDocument(docId));
		_chkDcSte(doc);
		getVariable(doc.m_VInt, name).m_Value = value;
		doc.write();
	}

	void setValue(const dstring_t& name, const data_double& value, const dstring_t& docId = DSTR("0")) {
		kDataDocument& doc(getDocument(docId));
		_chkDcSte(doc);
		getVariable(doc.m_VDouble, name).m_Value = value;
		doc.write();
	}

	void createVariable(const dstring_t& name, const data_string& value, const dstring_t& docId = DSTR("0")) {
		kDataDocument& doc(getDocument(docId));
		_chkDcSte(doc);
		doc.m_VStr.push_back(kDataVariable<data_string>{ value, name });
		doc.write();
	}

	void createVariable(const dstring_t& name, const data_double& value, const dstring_t& docId = DSTR("0")) {
		kDataDocument& doc(getDocument(docId));
		_chkDcSte(doc);
		doc.m_VDouble.push_back({ value, name });
		doc.write();
	}

	void createVariable(const dstring_t& name, const data_int& value, const dstring_t& docId = DSTR("0")) {
		kDataDocument& doc(getDocument(docId));
		_chkDcSte(doc);
		doc.m_VInt.push_back({ value, name });
		doc.write();
	}

	template<typename _Ty>bool isVariable(const dstring_t& name, const dstring_t& docId=DSTR("0")) {
		kDataDocument& doc(getDocument(docId));
		_chkDcSte(doc);
		return (isVariable<data_string>(name, docId) ||
			isVariable<data_double>(name, docId) ||
			isVariable<data_int>(name, docId));
	}

	template<>bool isVariable<data_string>(const dstring_t& name, const dstring_t& docId) {
		kDataDocument& doc(getDocument(docId));
		_chkDcSte(doc);
		return isVariableIn(doc.m_VStr, name);
	}
	
	template<>bool isVariable<data_double>(const dstring_t& name, const dstring_t& docId) {
		kDataDocument& doc(getDocument(docId));
		_chkDcSte(doc);
		return isVariableIn(doc.m_VDouble, name);
	}

	template<>bool isVariable<data_int>(const dstring_t& name, const dstring_t& docId) {
		kDataDocument& doc(getDocument(docId));
		_chkDcSte(doc);
		return isVariableIn(doc.m_VInt, name);
	}

	void forceRead(const dstring_t& docId=DSTR("0")) {
		getDocument(docId).refresh();
	}

private:
	kDataDocument& getDocument(const dstring_t& id) {
		if (m_Docs.empty()) assert(false && "No documents are loaded");
		if (id == DSTR("0")) return m_Docs[0];
		dvector_t<kDataDocument>::iterator it(m_Docs.begin());
		const dvector_t<kDataDocument>::const_iterator end(m_Docs.end());
		for (; it != end; ++it) {
			if (it->m_Id == id)
				return *it;
		}
		assert(false&&"This document doesn't exists");
		__assume(0);
	}

	template<typename _Ty>bool isVariableIn(dvector_t<kDataVariable<_Ty>> vec, std::wstring name) {
		auto it(vec.begin()); //2 decl: end can be const
		auto end(vec.end());
		for (; it != end; ++it) {
			if (it->m_Name == name)
				return true;
		}
		return false;
	}

	template<typename _Ty>kDataVariable<_Ty>& getVariable(dvector_t<kDataVariable<_Ty>>& vec, std::wstring name) {
		auto it(vec.begin()), end(vec.end());
		for (; it != end; ++it) {
			if (it->m_Name == name)
				return (*it);
		}
		assert(false&&"Variable doesn't exist (for this type)");
		__assume(0);
	}

	_k_i_dm_an("To work around: use of _chkDcSt for read & write (memory erasing)")
	void _chkDcSte(kDataDocument& d) {
		if (d.m_NeedReload)
			d.refresh();
	}

	dvector_t<kDataDocument> m_Docs;
};
