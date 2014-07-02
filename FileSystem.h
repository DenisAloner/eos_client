#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <memory>

//typedef std::vector<char> bytearray;
//typedef std::istream_iterator<char> istream_iterator;
//typedef std::ostreambuf_iterator<char> ostreambuf_iterator;

struct bytearray
{
	typedef unsigned char value_type;
	typedef unsigned char * pointer_type;
	typedef const unsigned char * const_pointer_type;

	bytearray() : m_ptr(), m_size(0) {}

	explicit bytearray(size_t size) : m_ptr(static_cast<pointer_type>(::malloc(size)), ::free), m_size(size)
	{
	}

	bytearray(const_pointer_type ptr, size_t size): m_ptr(static_cast<pointer_type>(::malloc(size)), ::free), m_size(size)
	{
		::memcpy(m_ptr.get(), ptr, size);
	}

	bytearray(const void *ptr, size_t size) : m_ptr(static_cast<pointer_type>(::malloc(size)), ::free), m_size(size)
	{
		::memcpy(m_ptr.get(), ptr, size);
	}

	bytearray(const bytearray& copy): m_ptr(copy.m_ptr), m_size(copy.m_size)
	{}

	inline bytearray& operator =(const bytearray& oth)
	{
		m_ptr = oth.m_ptr;
		m_size = oth.m_size;
		return *this;
	}

	inline operator void *() const { return cast<void>(); }

	template<typename T> 
	
	inline T* cast() const { return reinterpret_cast<T*>(get()); }

	inline bool is_empty() const { return !m_ptr || !m_size; }

	inline unsigned char * get() const { return m_ptr.get(); }

	inline size_t size() const { return m_size; }

	inline operator bool() const { return !is_empty(); }

	inline operator std::string() const { return std::string((const char*)get(), size()); }

private:
	std::shared_ptr<value_type> m_ptr;
	size_t m_size;
};

class FileSystem
{
public:

	std::string m_resource_path;

	static FileSystem& instance()
	{
		static FileSystem Singleton;
		return Singleton;
	}

	~FileSystem();

	bool load_from_file(const std::string& path, bytearray& data);
	bool save_to_file(const std::string& path, bytearray& data);

private:
	FileSystem();
	FileSystem(const FileSystem& root);
	FileSystem& operator=(const FileSystem&);
};

