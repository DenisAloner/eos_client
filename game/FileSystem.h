#ifndef FILESYSTEM_H
#define	FILESYSTEM_H

#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include "utils/bytearray.h"
#include "Definiton.h"
#include <vector>

class AI;

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
	void* deserialize_pointer(FILE* file);
	Object_interaction* deserialize_impact(FILE* file);
	void serialize_AI(AI* value, FILE* file);
	AI* deserialize_AI(FILE* file);

private:
	FileSystem();
	FileSystem(const FileSystem& root);
	FileSystem& operator=(const FileSystem&);
};

class Serialization_manager
{
public:

	std::vector<iSerializable*> m_items;

	static Serialization_manager& instance()
	{
		static Serialization_manager Singleton;
		return Singleton;
	}

	~Serialization_manager();

	bool load_from_file(const std::string& path, bytearray& data);
	bool save_to_file(const std::string& path, bytearray& data);
	void serialize_string(std::string& text, FILE* file);
	void deserialize_string(std::string& text, FILE* file);
	void serialize_pointer(const void* value, type_e object_type, FILE* file);
	void* deserialize_pointer(FILE* file);
	Object_interaction* deserialize_impact(FILE* file);
	void serialize_AI(AI* value, FILE* file);
	AI* deserialize_AI(FILE* file);

private:
	Serialization_manager();
	Serialization_manager(const Serialization_manager& root);
	Serialization_manager& operator=(const Serialization_manager&);
};

#endif //FILESYSTEM_H
