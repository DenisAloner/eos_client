#include "FileSystem.h"
#include "impact\Effect.h"
#include "AI.h"
#include "GameMap.h"
#include "impact\Impact_copy_chance.h"
#include "Parameter.h"

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

void FileSystem::serialize_AI_FOV(AI_FOV& value, FILE* file)
{
	fwrite(&value.radius, sizeof(int), 1, file);
	fwrite(&value.qualifier->index, sizeof(size_t), 1, file);
	fwrite(&value.start_angle, sizeof(int), 1, file);
	fwrite(&value.end_angle, sizeof(int), 1, file);
}

void FileSystem::deserialize_AI_FOV(AI_FOV& value, FILE* file)
{
	fread(&value.radius, sizeof(int), 1, file);
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	value.qualifier = Application::instance().m_ai_manager->m_fov_qualifiers[s];
	fread(&value.start_angle, sizeof(int), 1, file);
	fread(&value.end_angle, sizeof(int), 1, file);
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
		fread(value, sizeof(light_t), 1, file);
		return value;
	}
	case type_e::optical_properties_t:
	{
		optical_properties_t* value = new optical_properties_t();
		fread(value, sizeof(optical_properties_t), 1, file);
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
		switch (value->m_serialization_index)
		{
		case 0:
		{
			m_index += 1;
			value->m_serialization_index = m_index;
			LOG(INFO) << "Присвоен индекс " << std::to_string(value->m_serialization_index);
			value->save();
			break;
		}
		case 1:
		{
			value->save();
			break;
		}
		default :
		{
			type_e type = type_e::link;
			fwrite(&type, sizeof(type_e), 1, m_file);
			LOG(INFO) << "линк "<<std::to_string(value->m_serialization_index);
			fwrite(&value->m_serialization_index, sizeof(size_t), 1, m_file);
			break;
		}
		}
	}
	else
	{
		LOG(INFO) << "nullptr";
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
		LOG(INFO) << "nullptr";
		return nullptr;
	}
	case type_e::link:
	{
		size_t s;
		fread(&s, sizeof(type_e), 1, m_file);
		LOG(INFO) << "линк " << std::to_string(s);
		if (!(*m_items)[s]) 
		{
			LOG(INFO) << "!!! Еще не заполнена";
		}
		return (*m_items)[s];
	}
	case type_e::action:
	{
		LOG(INFO) << "Загрузка действия";
		iSerializable* value;
		size_t s;
		fread(&s, sizeof(size_t), 1, m_file);
		value = Application::instance().m_actions[s];
		LOG(INFO) << "Загружено действие "<< Application::instance().m_actions[s]->m_name;
		return value;
	}
	case type_e::mapcell:
	{
		int x;
		int y;
		fread(&x, sizeof(int), 1, m_file);
		fread(&y, sizeof(int), 1, m_file);
		LOG(INFO) << "Координаты " << std::to_string(x)<<" " << std::to_string(y);
		return m_map->m_items[y][x];
		break;
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
		case type_e::vision_list:
		{
			value = new Vision_list();
			break;
		}
		case type_e::vision_component:
		{
			value = new Vision_component();
			break;
		}
		case type_e::object_part:
		{
			value = new Object_part();
			break;
		}
		case type_e::slot_set_state:
		{
			value = new Slot_set_state();
			break;
		}
		case type_e::slot_select_cell:
		{
			value = new Slot_select_cell();
			break;
		}
		case type_e::slot_allocator:
		{
			value = new Slot_allocator();
			break;
		}
		case type_e::slot_mover:
		{
			value = new Slot_mover();
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
		case type_e::tag_activator:
		{
			value = new ObjectTag::Activator();
			break;
		}
		case type_e::ai_enemy:
		{
			value = new AI_enemy();
			break;
		}
		case type_e::gameobject:
		{
			value = new GameObject();
			break;
		}
		case type_e::impact_copy_chance:
		{
			value = new Impact_copy_chance();
			break;
		}
		case type_e::gamemap:
		{
			value = new GameMap();
			break;
		}
		case type_e::inventory_cell:
		{
			value = new Inventory_cell(nullptr);
			break;
		}
		case type_e::parameter_mapcell:
		{
			value = new Parameter_MapCell();
			break;
		}
		case type_e::parameter_position:
		{
			value = new Parameter_Position();
			break;
		}
		case type_e::parameter_movegameobject:
		{
			value = new Parameter_MoveObjectByUnit();
			break;
		}
		case type_e::parameter_destination:
		{
			value = new Parameter_destination();
			break;
		}
		case type_e::parameter_gameobject:
		{
			value = new P_object();
			break;
		}
		case type_e::parameter_object_owner:
		{
			value = new P_object_owner();
			break;
		}
		case type_e::parameter_unit_interaction:
		{
			value = new P_unit_interaction();
			break;
		}
		case type_e::parameter_interaction_cell:
		{
			value = new P_interaction_cell();
			break;
		}
		case type_e::parameter_direction:
		{
			value = new Parameter_direction();
			break;
		}
		case type_e::action_wrapper:
		{
			value = new Action_wrapper();
			break;
		}
		}
		LOG(INFO) << "Тип обьекта: " << std::to_string((int)type);
		m_index += 1;
		(*m_items)[m_index] = value;
		LOG(INFO) << "индекс: " << std::to_string(m_index);
		value->load();
		return value;
	}
	}
}

void Serialization_manager::save(const std::string& path, GameMap* map)
{
	m_map = map;
	errno_t err;
	LOG(INFO) << "Сохранение игры";
	err = fopen_s(&m_file, (FileSystem::instance().m_resource_path + "Saves\\" + path + ".txt").c_str(), "wb");
	if (err == 0)
	{
		m_index = 1;
		LOG(INFO) << "шаг 1";
		fwrite(&m_index, sizeof(size_t), 1, m_file);
		LOG(INFO) << "шаг 2";
		map->reset_serialization_index();
		LOG(INFO) << "шаг 3";
		serialize(map);
		serialize(Application::instance().m_GUI->MapViewer->m_player->m_object);
		rewind(m_file);
		m_index += 1;
		fwrite(&m_index, sizeof(size_t), 1, m_file);
		LOG(INFO) << "Количество сущностей: " << std::to_string(m_index - 1);
		fclose(m_file);
		LOG(INFO) << "Сохранение завершено успешно";
	}
	else
	{
		LOG(INFO) << "Сохранение завершилось с ошибкой";
	}
}

GameMap* Serialization_manager::load(const std::string& path)
{
	errno_t err;
	LOG(INFO) << "Загрузка игры";
	err = fopen_s(&m_file, (FileSystem::instance().m_resource_path + "Saves\\" + path + ".txt").c_str(), "rb");
	if (err == 0)
	{
		fread(&m_index, sizeof(size_t), 1, m_file);
		LOG(INFO) << "Количество сущностей: " << std::to_string(m_index);
		m_items = new std::vector<iSerializable*>(m_index, nullptr);
		m_index = 1;
		m_map = dynamic_cast<GameMap*>(deserialize());
		Application::instance().m_GUI->MapViewer->m_player = new Player(dynamic_cast<GameObject*>(deserialize()), m_map);
		m_items->clear();
		fclose(m_file);
		LOG(INFO) << "Загрузка завершена успешно";
		return m_map;
	}
	else
	{
		LOG(INFO) << "Загрузка завершилась с ошибкой";
		return nullptr;
	}
}