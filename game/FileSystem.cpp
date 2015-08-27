#include "FileSystem.h"
#include "impact\Effect.h"
#include "AI.h"

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

//Object_interaction* FileSystem::deserialize_impact(FILE* file)
//{
//	type_e type;
//	fread(&type, sizeof(type_e), 1, file);
//	Object_interaction* value;
//	switch (type)
//	{
//	case type_e::null:
//	{
//		return nullptr;
//	}
//	case type_e::interaction_addon:
//	{
//		value = new Interaction_addon();
//		break;
//	}
//	case type_e::interaction_time:
//	{
//		value = new Interaction_time();
//		break;
//	}
//	case type_e::interaction_timer:
//	{
//		value = new Interaction_timer();
//		break;
//	}
//	case type_e::interaction_list:
//	{
//		value = new Interaction_list();
//		break;
//	}
//	case type_e::interaction_copyist:
//	{
//		value = new Interaction_copyist();
//		break;
//	}
//	case type_e::interaction_prefix:
//	{
//		value = new Interaction_prefix();
//		break;
//	}
//	case type_e::parameter_list:
//	{
//		value = new Parameter_list();
//		break;
//	}
//	case type_e::action_list:
//	{
//		value = new Action_list();
//		break;
//	}
//	case type_e::parts_list:
//	{
//		value = new Parts_list();
//		break;
//	}
//	case type_e::tag_list:
//	{
//		value = new Tag_list();
//		break;
//	}
//	case type_e::effect:
//	{
//		value = new Effect();
//		break;
//	}
//	case type_e::tag_label:
//	{
//		value = new ObjectTag::Label(object_tag_e::none);
//		break;
//	}
//	case type_e::tag_poison_resist:
//	{
//		value = new ObjectTag::Poison_resist();
//		break;
//	}
//	case type_e::tag_mortal:
//	{
//		value = new ObjectTag::Mortal();
//		break;
//	}
//	case type_e::object_part:
//	{
//		value = new Object_part();
//		break;
//	}
//	case type_e::tag_purification_from_poison:
//	{
//		value = new ObjectTag::Purification_from_poison();
//		break;
//	}
//	case type_e::tag_fast_move:
//	{
//		value = new ObjectTag::Fast_move();
//		break;
//	}
//	case type_e::action:
//	{
//		size_t s;
//		fread(&s, sizeof(size_t), 1, file);
//		value = Application::instance().m_actions[s];
//		break;
//	}
//	}
//	value->load();
//	return value;
//}

Serialization_manager::Serialization_manager()
{
}

Serialization_manager::~Serialization_manager()
{
}

void Serialization_manager::serialize(iSerializable* value)
{
	if (value)
	{
		if (value->m_serialization_index == 0)
		{
			m_index += 1;
			value->m_serialization_index = m_index;
			value->save();
		}
		else
		{
			type_e type = type_e::link;
			fwrite(&type, sizeof(type_e), 1, m_file);
			fwrite(&value->m_serialization_index, sizeof(size_t), 1, m_file);
		}
	}
	else
	{
		type_e type = type_e::null;
		fwrite(&type, sizeof(type_e), 1, m_file);
	}
}

iSerializable* Serialization_manager::deserialize()
{
	type_e type;
	fread(&type, sizeof(type_e), 1, m_file);
	switch (type)
	{
	case type_e::null:
	{
		return nullptr;
	}
	case type_e::link:
	{
		size_t s;
		fread(&s, sizeof(type_e), 1, m_file);
		return m_items[s];
	}
	default:
	{
		iSerializable* value;
		switch (type)
		{
		case type_e::object_state:
		{
			value = new Object_state();
			break;
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
		case type_e::interaction_timer:
		{
			value = new Interaction_timer();
			break;
		}
		case type_e::interaction_list:
		{
			value = new Interaction_list();
			break;
		}
		case type_e::interaction_copyist:
		{
			value = new Interaction_copyist();
			break;
		}
		case type_e::interaction_prefix:
		{
			value = new Interaction_prefix();
			break;
		}
		case type_e::parameter_list:
		{
			value = new Parameter_list();
			break;
		}
		case type_e::action_list:
		{
			value = new Action_list();
			break;
		}
		case type_e::parts_list:
		{
			value = new Parts_list();
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
		case type_e::tag_poison_resist:
		{
			value = new ObjectTag::Poison_resist();
			break;
		}
		case type_e::tag_mortal:
		{
			value = new ObjectTag::Mortal();
			break;
		}
		case type_e::object_part:
		{
			value = new Object_part();
			break;
		}
		case type_e::tag_purification_from_poison:
		{
			value = new ObjectTag::Purification_from_poison();
			break;
		}
		case type_e::tag_fast_move:
		{
			value = new ObjectTag::Fast_move();
			break;
		}
		case type_e::action:
		{
			size_t s;
			fread(&s, sizeof(size_t), 1, m_file);
			value = Application::instance().m_actions[s];
			break;
		}
		case type_e::ai_enemy:
		{
			value = new AI_enemy();
			break;
		}
		}
		value->load();
		return value;
	}
	}
}