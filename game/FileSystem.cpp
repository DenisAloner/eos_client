#include "FileSystem.h"
#include "impact\Effect.h"

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

void FileSystem::serialize_pointer(const void* value, type_e object_type, FILE* file)
{
	if (value == nullptr)
	{
		object_type = type_e::null;
		fwrite(&object_type, sizeof(type_e), 1, file);
	}
	else
	{
		switch (object_type)
		{
		case type_e::light_t:
		{
			fwrite(&object_type, sizeof(type_e), 1, file);
			fwrite(value, sizeof(light_t), 1, file);
			break;
		}
		case type_e::optical_properties_t:
		{
			fwrite(&object_type, sizeof(type_e), 1, file);
			fwrite(value, sizeof(optical_properties_t), 1, file);
			break;
		}
		}
	}
}

void* FileSystem::deserialize_pointer(FILE* file)
{
	type_e type;
	fread(&type, sizeof(type_e), 1, file);

	switch (type)
	{
	case type_e::null:
	{
		return nullptr;
	}
	case type_e::light_t:
	{
		light_t* value = new light_t();
		fwrite(value, sizeof(light_t), 1, file);
		return value;
	}
	case type_e::optical_properties_t:
	{
		optical_properties_t* value = new optical_properties_t();
		fwrite(value, sizeof(optical_properties_t), 1, file);
		return value;
	}
	}
}

Object_interaction* FileSystem::deserialize_impact(FILE* file)
{
	type_e type;
	fread(&type, sizeof(type_e), 1, file);
	Object_interaction* value;
	switch (type)
	{
	case type_e::null:
	{
		return nullptr;
	}
	case type_e::interaction_addon:
	{
		value = new Interaction_addon();
		break;
	}
	case type_e::interaction_time:
	{
		value = new Interaction_time();
		break;
	}
	case type_e::interaction_list:
	{
		value = new Interaction_list();
		break;
	}
	case type_e::parameter_list:
	{
		value = new Parameter_list();
		break;
	}
	case type_e::tag_list:
	{
		value = new Tag_list();
		break;
	}
	case type_e::effect:
	{
		value = new Effect();
		break;
	}
	case type_e::tag_label:
	{
		value = new ObjectTag::Label(object_tag_e::none);
		break;
	}
	}
	value->load(file);
	return value;
}