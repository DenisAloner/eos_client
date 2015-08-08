#ifndef FILESYSTEM_H
#define	FILESYSTEM_H

#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include "utils/bytearray.h"
#include "Definiton.h"


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
	void serialize_string(std::string& text, FILE* file);
	void deserialize_string(std::string& text, FILE* file);
	void serialize_pointer(const void* value, type_e object_type, FILE* file);

private:
	FileSystem();
	FileSystem(const FileSystem& root);
	FileSystem& operator=(const FileSystem&);
};

#endif //FILESYSTEM_H
