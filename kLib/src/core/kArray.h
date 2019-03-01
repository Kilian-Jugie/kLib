#pragma once
#include "kPreprocessor.h"
#include <iostream>

namespace kLib {
	_Success_(return >= 0) template <typename _Ty>
		int _copy_unchecked(_Out_ _Ty *_Dest, _In_reads_(_size) const _Ty *_Src,
			_In_opt_ size_t _size) {
		for (size_t i(0u); i < _size; ++i)
			_Dest[i] = _Src[i];
		return 0;
	}

	_Success_(return >= 0) template <typename _Ty>
		int _copy_checked(_Out_writes_all_(_destSize) _Ty *_Dest,
			_In_reads_(_srcSize) const _Ty *_Src,
			_In_opt_ size_t _destSize, _In_opt_ size_t _srcSize) {
		if (_destSize < _srcSize)
			return -1;
		return _copy_unchecked(_Dest, _Src, _srcSize);
	}

	template <typename _Ty>
	size_t _copy_or_trunc(_Out_ _Ty *_Dest, _In_ const _Ty *_Src,
		_In_opt_ const size_t destSize,
		_In_opt_ const size_t sourceSize) {
		for (int i = 0; i < destSize && i < sourceSize; ++i)
			_Dest[i] = _Src[i];
		return destSize < sourceSize ? destSize : sourceSize;
	}

	template <typename _Ty, size_t _DestSize, size_t _SrcSize>
	void copy(_Ty(&_Dest)[_DestSize], _Ty(&_Src)[_SrcSize]) {
#ifdef K_LIB_USE_UNSAFE_COPY
		_copy_unchecked(_Dest, _Src, _SrcSize);
#else
		_copy_checked(_Dest, _Src, _DestSize, _SrcSize);
#endif // K_LIB_USE_UNSAFE_COPY
	}
} // namespace kLib

template <typename _Ty> class _kContainer {
public:
	virtual inline _Ty &at(size_t index) = 0;
	virtual inline const _Ty &at(size_t index) const = 0;

	virtual _kContainer &alloc(size_t size) = 0;
	virtual _kContainer &realloc(size_t newsize) = 0;
	virtual _kContainer &free() = 0;

	virtual inline _Ty &operator[](size_t index) { return at(index); }
	virtual inline const _Ty &operator[](size_t index) const { return at(index); }

	virtual inline size_t size() const = 0;
	virtual inline size_t allocSize() const = 0;

	virtual inline _Ty *data() = 0;

	virtual inline void copyFrom(const _Ty *arr, size_t size) = 0;
};

template <typename _Ty> class kSimpleContainer : public _kContainer<_Ty> {
	_Ty *m_Data;
	size_t m_Size;

public:
	kSimpleContainer(const _Ty *data, size_t size);
	kSimpleContainer(size_t size);
	~kSimpleContainer();

	inline _Ty &at(size_t index) override;
	inline const _Ty &at(size_t index) const override;

	kSimpleContainer &alloc(size_t size) override;
	kSimpleContainer &realloc(size_t newsize) override;
	kSimpleContainer &free() override;

	void copyFrom(const _Ty *arr, size_t size) override;

	_Check_return_ inline size_t size() const override { return m_Size; }

	_Check_return_ inline size_t allocSize() const override { return m_Size; }

	_Check_return_ inline _Ty *data() override { return m_Data; }
};

template <typename _Ty, size_t _maxSSize = 64, size_t _maxSCount = 32768>
class kSegmentedContainer : public _kContainer<_Ty> {
	_Ty **m_Data;
	size_t _segIndex, _lastIndex;

public:
	kSegmentedContainer(_In_ _In_reads_(size) const _Ty *data, _In_ size_t size) {
		this->alloc(size);
	}

	kSegmentedContainer(size_t size) { this->alloc(size); }

	~kSegmentedContainer() { this->free(); }

	virtual void copyFrom(const _Ty *arr, size_t size) override {
		if (this->allocSize() > size)
			this->realloc(size);
	}

	kSegmentedContainer &alloc(size_t size) override {
		if (m_Data)
			return this->realloc(size);
		if (!size)
			return *this;
		m_Data = new _Ty *[_maxSCount];
		_segIndex = (size_t)(size / _maxSSize) + 1;
		for (size_t _i(0u); _i < _segIndex; ++_i)
			m_Data[_i] = new _Ty[_maxSSize];
		return *this;
	}

	kSegmentedContainer &trunc(size_t newsize, size_t _segtrm = 0) {
		if (!newsize)
			this->free();
		if (!_segtrm)
			_segtrm = _segIndex - ((size_t)(_maxSSize / newsize) + 1);
		for (size_t _i(_segIndex - _segtrm); _i < _segIndex; ++_i) {
			delete[] m_Data[_i];
			m_Data[_i] = nullptr;
		}
		return *this;
	}

	kSegmentedContainer &realloc(size_t newsize) override {
		if (!newsize)
			this->free();
		const size_t _newIndex = (size_t)(_maxSSize / newsize) + 1;
		if (_newIndex < _segIndex)
			return this->trunc(newsize);
		if (newsize == this->size())
			return *this;
		for (size_t _i(_segIndex); _i < _newIndex; ++_i)
			m_Data[_i] = new _Ty[_maxSSize];
		return *this;
	}

	kSegmentedContainer &free() override {
		for (size_t _i(0u); _i < _segIndex; ++_i) {
			delete[] m_Data[_i];
			m_Data[_i] = nullptr;
		}
		if (m_Data) {
			delete[] m_Data;
			m_Data = nullptr;
		}
		return *this;
	}

	_Check_return_ inline _Ty &at(size_t index) override {
		return m_Data[(size_t)(index / _segIndex) + 1][index % _lastIndex];
	}

	_Check_return_ inline const _Ty &at(size_t index) const override {
		return m_Data[(size_t)(index / _segIndex) + 1][index % _lastIndex];
	}

	_Check_return_ inline size_t allocSize() const override {
		return _segIndex * _maxSSize;
	}

	_Check_return_ inline size_t size() const override {
		return (_segIndex * _maxSSize) - (_maxSSize - _segIndex + 1);
	}

	// Not adapted to this type of container
	_Check_return_ __declspec(deprecated) _Ty *data() override { return nullptr; }
};

template <typename _Ty>
void kSimpleContainer<_Ty>::copyFrom(_In_ const _Ty *arr, _In_ size_t size) {
	kLib::_copy_unchecked(m_Data, arr, size);
}

//_k_an_kd_wwa("use of opt or not for size param")
template <typename _Ty>
kSimpleContainer<_Ty>::kSimpleContainer(_In_ const _Ty *data,
	_In_opt_ size_t size) {
	this->alloc(size); // explicit thiscall to avoid confusions
	copyFrom(data, size);
}

template <typename _Ty>
kSimpleContainer<_Ty>::kSimpleContainer(_In_ size_t size) {
	this->alloc(size);
}

template <typename _Ty> kSimpleContainer<_Ty>::~kSimpleContainer() {
	this->free();
}

template <typename _Ty>
kSimpleContainer<_Ty> &kSimpleContainer<_Ty>::alloc(_In_ size_t size) {
	if (size) {
		m_Data = new _Ty[size];
		m_Size = size;
	}
	else
		m_Size = 0;
	return *this;
}

template <typename _Ty> kSimpleContainer<_Ty> &kSimpleContainer<_Ty>::free() {
	if (m_Data && m_Size) {
		delete[] m_Data;
		m_Size = 0;
	}
	return *this;
}

_k_an_kd_iwa("use of opt or not for newsize param") template <typename _Ty>
kSimpleContainer<_Ty> &kSimpleContainer<_Ty>::realloc(_In_opt_ size_t newsize) {
	_Ty *cp = new _Ty[m_Size];
	kLib::_copy_unchecked(cp, m_Data, m_Size);
	size_t _oldS(m_Size);
	this->free();
	this->alloc(newsize);
	kLib::_copy_or_trunc(m_Data, cp, newsize, _oldS);
	delete[] cp;
	return *this;
}

_Check_return_ template <typename _Ty>
inline _Ty &kSimpleContainer<_Ty>::at(_In_opt_ _In_range_(0, SIZE_MAX)
	size_t index) {
	return m_Data[index];
}

_Check_return_ template <typename _Ty>
inline const _Ty &kSimpleContainer<_Ty>::at(_In_opt_ _In_range_(0, SIZE_MAX)
	size_t index) const {
	return m_Data[index];
}

template <typename _Ty>
std::wostream &operator<<(std::wostream &_Ostr,
	_In_ const kSimpleContainer<_Ty> &_Cont) {
	for (size_t i(0u); i < _Cont.size(); ++i)
		_Ostr << _Cont[i];
	return _Ostr;
}

template <typename _Ty>
std::ostream &operator<<(std::ostream &_Ostr,
	_In_ const kSimpleContainer<_Ty> &_Cont) {
	for (size_t i(0u); i < _Cont.size(); ++i)
		_Ostr << _Cont[i];
	return _Ostr;
}

template <typename _Ty>
std::wostream &operator<<(std::wostream &_Ostr,
	_In_ const kSegmentedContainer<_Ty> &_Cont) {
	for (size_t i(0u); i < _Cont.size(); ++i)
		_Ostr << _Cont[i];
	return _Ostr;
}

template <typename _Ty>
std::ostream &operator<<(std::ostream &_Ostr,
	_In_ const kSegmentedContainer<_Ty> &_Cont) {
	for (size_t i(0u); i < _Cont.size(); ++i)
		_Ostr << _Cont[i];
	return _Ostr;
}

template <typename _Ty> class _kArray {
protected:
	_kContainer<_Ty> *m_Data;

public:
	_Check_return_ _Ty &at(_In_ size_t index) { return m_Data->at(index); }

	_Check_return_ const _Ty &at(_In_opt_ _In_range_(0, SIZE_MAX)
		size_t index) const {
		return m_Data->at(index);
	}

	_Check_return_ _Ty &operator[](_In_opt_ _In_range_(0, SIZE_MAX)
		size_t index) {
		return this->at(index);
	}

	_Check_return_ const _Ty &operator[](_In_opt_ _In_range_(0, SIZE_MAX)
		size_t index) const {
		return this->at(index);
	}

	virtual _kArray &alloc(_In_ _In_range_(1, SIZE_MAX) size_t size) = 0;
	virtual _kArray &realloc(_In_ _In_range_(1, SIZE_MAX) size_t size) = 0;
	virtual _kArray &free() = 0;

	_k_an_kd_iwa("use of opt or not for size param") virtual void copyFrom(
		_In_ const _Ty *arr, _In_opt_ size_t size) {
		if (m_Data->allocSize() < size)
			m_Data->realloc(size);
		m_Data->copyFrom(arr, size);
	}

	virtual size_t copyTo(_Out_writes_all_(size) _Ty *arr, _In_ size_t size) {
		kLib::_copy_or_trunc(arr, m_Data->data(), size, this->size());
		return size < this->size() ? size : this->size();
	}

	virtual _kArray &push_back(const _Ty &_elem) = 0;

	_Check_return_ size_t size() const { return m_Data->size(); }

	_Check_return_ _Ty const *data() const { return m_Data->data(); }
};

template <typename _Ty> class kSimpleArray : public _kArray<_Ty> {
public:
	kSimpleArray() { this->m_Data = new kSimpleContainer<_Ty>(0); }
	kSimpleArray(const _Ty *arr, size_t size) {}

	virtual kSimpleArray &alloc(_In_ _In_range_(1, SIZE_MAX)
		size_t size) override {
		this->m_Data->alloc(size);
		return *this;
	}

	virtual kSimpleArray &realloc(_In_ _In_range_(1, SIZE_MAX)
		size_t size) override {
		this->m_Data->realloc(size);
		return *this;
	}

	virtual kSimpleArray &free() override {
		this->m_Data->free();
		return *this;
	}

	virtual kSimpleArray &push_back(_In_ const _Ty &elem) {
		this->m_Data->realloc(this->size() + 1);
		this->m_Data->data()[this->size() - 1] = elem;
		return *this;
	}
};

template <typename _Ty>
std::wostream &operator<<(std::wostream &_Ostr,
	_In_ const _kArray<_Ty> &_Cont) {
	for (size_t i(0u); i < _Cont.size(); ++i)
		_Ostr << _Cont[i];
	return _Ostr;
}

template <typename _Ty>
std::ostream &operator<<(std::ostream &_Ostr, _In_ const _kArray<_Ty> &_Cont) {
	for (size_t i(0u); i < _Cont.size(); ++i)
		_Ostr << _Cont[i];
	return _Ostr;
}