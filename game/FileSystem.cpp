#include "FileSystem.h"

#ifndef RESOURCES_PATH
#define RESOURCES_PATH "resources\\"
#endif

FileSystem::FileSystem()
: m_resource_path(RESOURCES_PATH)
{
}


FileSystem::~FileSystem()
{
}


bool FileSystem::load_from_file(const std::string& path, bytearray& data)
{
	FILE *file;
	if (fopen_s(&file, path.c_str(), "rb") != NULL)
	{
		// тут можно в stdout вывести сообщение об ошибке чтения файла, не обязательно
		return false;
	}
	fseek(file, 0, SEEK_END);
	const long int filesize = ftell(file);
	bytearray buff(filesize);
	fseek(file, 0, SEEK_SET);
	if (fread(buff.get(), buff.size(), 1, file) == 1)
	{
		data = buff;
		fclose(file);
		return true;
	}
	else
	{
		fclose(file);
		return false;
	}
}


bool FileSystem::save_to_file(const std::string& path, bytearray& data)
{
	FILE *file;
	if (fopen_s(&file, path.c_str(), "wb") != NULL)
	{
		// тут можно в stdout вывести сообщение об ошибке чтения файла, не обязательно
		return false;
	}
	if (fwrite(data.get(), data.size(), 1, file) == 1)
	{
		fclose(file);
		return true;
	}
	else
	{
		fclose(file);
		return false;
	}
}

void FileSystem::serialize_string(std::string& text,FILE* file)
{
	size_t s = text.size();
	fwrite(&s, sizeof(size_t), 1, file);
	fwrite(text.c_str(), sizeof(char), s, file);
}

void FileSystem::deserialize_string(std::string& text, FILE* file)
{
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	char* str = new char[s];
	if (fread(str, sizeof(char), s, file) != s)
	{
		std::cout << "error" << std::endl;
	}
	text.assign(str, s);
}