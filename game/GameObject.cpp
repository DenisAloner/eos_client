#include "game/GameObject.h"
#include "game/Action.h"
#include "log.h"
#include "AI.h"

MapCell::MapCell(int x, int y, GameMap* map) :x(x), y(y), m_map(map)
{
	m_kind = entity_e::cell;
	m_notable = false;
}

void MapCell::add_object(GameObject* Object)
{
	m_items.push_back(Object);
}

void MapCell::save()
{
	LOG(INFO) << "Запись клетки карты";
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::mapcell;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&x, sizeof(int), 1, file);
	fwrite(&y, sizeof(int), 1, file);
	LOG(INFO) << "Конец записи клетки карты";
}

void MapCell::load()
{
}


Attribute_map::Attribute_map(){};

Interaction_list*  Attribute_map::create_feature_list(feature_list_type_e key, interaction_e name)
{
	Interaction_list* result=nullptr;
	switch (key)
		{
		case feature_list_type_e::generic:
		{
			result = new Interaction_list();
			break;
		}
		case feature_list_type_e::tag:
		{
			result = new Tag_list();
			break;
		}
		case feature_list_type_e::action:
		{
			result = new Action_list();
			break;
		}
		case feature_list_type_e::parameter:
		{
			result = new Parameter_list(name);
			break;
		}
		case feature_list_type_e::parts:
		{
			result = new Parts_list();
			break;
		}
		case feature_list_type_e::vision:
		{
			result = new Vision_list();
			break;
		}
		case feature_list_type_e::vision_item:
		{
			result = new Vision_item();
			break;
		}
		}
	m_item[name] = result;	
	return result;
}

void Attribute_map::add_effect(interaction_e key, Object_interaction* item)
{
	if (m_item.find(key) != m_item.end())
	{
		m_item[key]->add(item);
	}
}

Interaction_list* Attribute_map::get_list(interaction_e key)
{
	auto value = m_item.find(key);
	if (value != m_item.end())
	{
		return static_cast<Parts_list*>(value->second);
	}
	return nullptr;
}

Attribute_map* Attribute_map::clone()
{
	Attribute_map* result = new Attribute_map();
	for (auto item = m_item.begin(); item != m_item.end(); item++)
	{
		result->m_item[item->first] = item->second->clone();
	}
	return result;
}

void  Attribute_map::reset_serialization_index()
{
	m_serialization_index = 0;
	for (auto item = m_item.begin(); item != m_item.end(); item++)
	{
		(*item).second->reset_serialization_index();
	}
}

void Attribute_map::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	size_t s = m_item.size();
	fwrite(&s, sizeof(size_t), 1, file);
	LOG(INFO) << "листов " << std::to_string(s);
	for (auto item = m_item.begin(); item != m_item.end(); item++)
	{
		LOG(INFO) << "МАРКЕР АТРИБУТА  - " << std::to_string((int)(*item).first);
		fwrite(&((*item).first), sizeof(interaction_e), 1, file);
		Serialization_manager::instance().serialize((*item).second);
	}
}

void Attribute_map::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	m_item.clear();
	interaction_e ie;
	LOG(INFO) << "листов " << std::to_string(s);
	for (size_t i = 0; i < s; i++)
	{
		fread(&ie, sizeof(interaction_e), 1, file);
		LOG(INFO) << "МАРКЕР АТРИБУТА  - " << std::to_string((int)ie);
		m_item[ie] = dynamic_cast<Interaction_list*>(Serialization_manager::instance().deserialize());
	}
}

Object_state::Object_state()
{
	m_layer = 1;
	m_light = nullptr;
	m_tile_manager = nullptr;
	m_icon = Application::instance().m_graph->m_no_image;
	m_ai = nullptr;
	m_optical = nullptr;
	m_visibility = nullptr;
	m_weight = 0.0F;
}

Object_state* Object_state::clone()
{
	Object_state* state = new Object_state();
	state->m_state = m_state;
	state->m_layer = m_layer;
	state->m_size = m_size;
	state->m_tile_size = m_tile_size;
	state->m_tile_manager = m_tile_manager;
	state->m_icon = m_icon;
	state->m_weight = m_weight;
	if (m_light)
	{
		state->m_light = new light_t(*m_light);
	}
	if (m_ai)
	{
		state->m_ai = m_ai->clone();
	}
	else state->m_ai = nullptr;
	if (m_optical)
	{
		state->m_optical = new optical_properties_t(*m_optical);
	}
	if (m_visibility)
	{
		state->m_visibility = new float;
		*state->m_visibility = *m_visibility;
	}
	for (auto item = m_item.begin(); item != m_item.end(); item++)
	{
		state->m_item[item->first] = item->second->clone();
	}
	return state;
}

Object_state_equip::Object_state_equip()
{
	m_state = object_state_e::equip;
}

Object_state* Object_state_equip::clone()
{
	Object_state_equip* state = new Object_state_equip();
	state->m_body_part = m_body_part;
	state->m_state = m_state;
	state->m_layer = m_layer;
	state->m_size = m_size;
	state->m_tile_size = m_tile_size;
	state->m_tile_manager = m_tile_manager;
	state->m_light = m_light;
	state->m_icon = m_icon;
	if (m_ai)
	{
		state->m_ai = m_ai->clone();
	}
	else state->m_ai = nullptr;
	
	for (auto item = m_item.begin(); item != m_item.end(); item++)
	{
		state->m_item[item->first] = item->second->clone();
	}
	return state;
}

//Action* Object_state::find_action(action_e kind)
//{
//	for (std::list<Action*>::iterator Current = m_actions.begin(); Current != m_actions.end(); Current++)
//	{
//		if ((*Current)->m_kind == kind)
//		{
//			return (*Current);
//		}
//	}
//	return nullptr;
//}

void Object_state::set_tile_size()
{
	m_tile_size = dimension_t(tile_size_x_half*(m_size.x + m_size.y), tile_size_y_half*(m_size.x + m_size.y) + m_size.z * 18);
}

void Object_state::reset_serialization_index()
{
	m_serialization_index = 0;
	Attribute_map::reset_serialization_index();
	if (m_ai)
	{
		m_ai->reset_serialization_index();
	}
}

void Object_state::save()
{
	LOG(INFO) << "Состояние";
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::object_state;
	fwrite(&t, sizeof(type_e), 1, file);
	Attribute_map::save();
	fwrite(&m_state, sizeof(object_state_e), 1, file);
	fwrite(&m_layer, sizeof(int), 1, file);
	fwrite(&m_size, sizeof(game_object_size_t), 1, file);
	fwrite(&m_weight, sizeof(float), 1, file);
	FileSystem::instance().serialize_pointer(m_light, type_e::light_t, file);
	FileSystem::instance().serialize_pointer(m_optical, type_e::optical_properties_t, file);
	if(m_tile_manager->m_index>23)
	{
		LOG(FATAL) << "Тайловый менеджер с ошибкой " <<std::to_string(m_tile_manager->m_index);
	}
	fwrite(&m_tile_manager->m_index, sizeof(size_t), 1, file);
	LOG(INFO) << "ИИ";
	Serialization_manager::instance().serialize(m_ai);
	LOG(INFO) << "Конец ИИ";
	LOG(INFO) << "Конец состояния";
}

void Object_state::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	Attribute_map::load();
	fread(&m_state, sizeof(object_state_e), 1, file);
	fread(&m_layer, sizeof(int), 1, file);
	fread(&m_size, sizeof(game_object_size_t), 1, file);
	fread(&m_weight, sizeof(float), 1, file);
	m_light = static_cast<light_t*>(FileSystem::instance().deserialize_pointer(file));
	m_optical = static_cast<optical_properties_t*>(FileSystem::instance().deserialize_pointer(file));
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	LOG(INFO) << "тайл "<<std::to_string(s);
	m_tile_manager = Application::instance().m_graph->m_tile_managers[s];
	m_ai = static_cast<AI*>(Serialization_manager::instance().deserialize());
}

//Object_feature* MapCell::find_property(property_e kind, GameObject* excluded)
//{
//	Object_feature* result(nullptr);
//	for (std::list<GameObject*>::iterator item = m_items.begin(); item != m_items.end(); item++)
//	{
//		if (excluded != (*item))
//		{
//			result = (*item)->m_active_state->find_property(kind);
//			if (result != nullptr)
//			{
//				return result;
//			}
//		}
//	}
//	return nullptr;
//}

GameObject::GameObject()
{
	m_direction = object_direction_e::down;
	m_selected = false;
	m_active_state = nullptr;
	//rendering_necessary = false;
}

bool GameObject::on_turn()
{
	LOG(FATAL)<<"Метод не описан.";
	return false;
}

std::string GameObject::get_description()
{
	LOG(FATAL) << "Метод не описан.";
	return "";
}

Object_interaction* GameObject::clone()
{
	LOG(FATAL) << "Метод не описан.";
	return nullptr;
}

void GameObject::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<" + m_name + ">");
}

void GameObject::do_predicat(predicat func)
{ 
	func(this); 
	LOG(FATAL) << "Метод не описан.";
};

void GameObject::turn()
{
	/*object_direction_e dir;
	if (m_direction == object_direction_e_Down)
	{
		dir = object_direction_e_Left;
	}
	if (m_direction == object_direction_e_Left)
	{
		dir = object_direction_e_Up;
	}
	if (m_direction == object_direction_e_Up)
	{
		dir = object_direction_e_Right;
	}
	if (m_direction == object_direction_e_Right)
	{
		dir = object_direction_e_Down;
	}
	set_direction(dir);*/
}

void GameObject::set_direction(object_direction_e dir)
{
	/*if (((m_direction == object_direction_e_Left || m_direction == object_direction_e_Right) && (dir == object_direction_e_Up || dir == object_direction_e_Down)) || ((dir == object_direction_e_Left || dir == object_direction_e_Right) && (m_direction == object_direction_e_Up || m_direction == object_direction_e_Down)))
	{
		for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
		{
			(*item)->m_size = game_object_size_t((*item)->m_size.y, (*item)->m_size.x, (*item)->m_size.z);
		}
	}*/
	for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
	{
		(*item)->set_tile_size();
	}
	m_direction = dir;
}

void GameObject::set_state(object_state_e state)
{
	for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
	{
		if ((*item)->m_state == state)
		{
			m_active_state = (*item);
			return;
		}
	}
}

Object_state* GameObject::get_state(object_state_e state)
{
	for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
	{
		if ((*item)->m_state == state)
		{
			return (*item);
		}
	}
	return nullptr;
}

Interaction_list* GameObject::create_feature_list(feature_list_type_e key, interaction_e name)
{
	Interaction_list* result;

	switch (key)
	{
	case feature_list_type_e::generic:
	{
		result = new Interaction_list();
		break;
	}
	case feature_list_type_e::tag: 
	{
		result = new Tag_list();
		break;
	}
	case feature_list_type_e::action:
	{
		result = new Action_list();
		break;
	}
	case feature_list_type_e::parameter:
	{
		result = new Parameter_list(name);
		break;
	}
	case feature_list_type_e::parts:
	{
		result = new Parts_list();
		break;
	}
	case feature_list_type_e::vision:
	{
		result = new Vision_list();
		break;
	}
	case feature_list_type_e::vision_item:
	{
		result = new Vision_item();
		break;
	}
	}
	m_active_state->m_item[name] = result;
	return result;
}

void GameObject::add_effect(interaction_e key, Object_interaction* item)
{
	if (m_active_state->m_item.find(key) != m_active_state->m_item.end())
	{
		m_active_state->m_item[key]->add(item);
	}
}

void GameObject::add_from(interaction_e key, Interaction_list* feature)
{
	if (m_active_state)
	{
		if (m_active_state->m_item.find(key) == m_active_state->m_item.end())
		{
			m_active_state->m_item[key] = feature;
		}
		else
		{
			m_active_state->m_item[key]->add(feature);
		}
	}
}

void GameObject::remove_effect(interaction_e key, Object_interaction* item)
{
	if (m_active_state)
	{
		if (m_active_state->m_item.find(key) != m_active_state->m_item.end())
		{
			m_active_state->m_item[key]->remove(item);
		}
	}
}

void GameObject::remove_from(interaction_e key, Interaction_list* feature)
{
	if (m_active_state)
	{
		auto item = m_active_state->m_item.find(key);
		if (item != m_active_state->m_item.end())
		{
			if (item->second == feature)
			{
				m_active_state->m_item.erase(key);
			}
			else
			{
				m_active_state->m_item[key]->remove(feature);
			}
		}
	}
}

Interaction_list* GameObject::get_effect(interaction_e key)
{
	if (m_active_state)
	{
		auto value = m_active_state->m_item.find(key);
		if (value != m_active_state->m_item.end())
		{
			return value->second;
		}
	}
	return nullptr;
}

Parameter_list* GameObject::get_parameter(interaction_e key)
{
	if (m_active_state)
	{
		auto value = m_active_state->m_item.find(key);
		if (value != m_active_state->m_item.end())
		{
			return static_cast<Parameter_list*>(value->second);
		}
	}
	return nullptr;
}

Parts_list* GameObject::get_parts_list(interaction_e key)
{
	if (m_active_state)
	{
		auto value = m_active_state->m_item.find(key);
		if (value != m_active_state->m_item.end())
		{
			return static_cast<Parts_list*>(value->second);
		}
	}
	return nullptr;
}

bool GameObject::get_stat(object_tag_e key)
{
	if (m_active_state)
	{
		auto list = m_active_state->m_item.find(interaction_e::tag);
		if (list != m_active_state->m_item.end())
		{
			Tag_list* taglist = static_cast<Tag_list*>(list->second);
			for (auto item = taglist->m_effect.begin(); item != taglist->m_effect.end(); item++)
			{
				if (static_cast<Object_tag*>(*item)->m_type == key){ return true; }
			}
		}
	}
	return false;
}

Object_tag* GameObject::get_tag(object_tag_e key)
{
	if (m_active_state)
	{
		auto list = m_active_state->m_item.find(interaction_e::tag);
		if (list != m_active_state->m_item.end())
		{
			Tag_list* taglist = static_cast<Tag_list*>(list->second);
			for (auto item = taglist->m_effect.begin(); item != taglist->m_effect.end(); item++)
			{
				if (static_cast<Object_tag*>(*item)->m_type == key){ return static_cast<Object_tag*>(*item); }
			}
		}
	}
	return nullptr;
}

MapCell* GameObject::cell(){
	return static_cast<MapCell*>(m_owner);
}

bool GameObject::is_own(MapCell* c)
{
	if (c->x >= cell()->x&&c->x<cell()->x + m_active_state->m_size.x&&c->y <= cell()->y&&c->y > cell()->y - m_active_state->m_size.y)
	{
		return true;
	}
	return false;
}

bool GameObject::is_own(int x, int y)
{
	if (x >= cell()->x&&x<cell()->x + m_active_state->m_size.x&&y <= cell()->y&&y > cell()->y - m_active_state->m_size.y)
	{
		return true;
	}
	return false;
}

MapCell* GameObject::get_center(MapCell* c)
{
	if ((m_active_state->m_size.x % 2) == 0)
	{
		int x;
		int y;
		if (c->x <= cell()->x + m_active_state->m_size.x / 2 - 1)
		{
			x = cell()->x + m_active_state->m_size.x / 2 - 1;
		}
		else
		{
			x =cell()->x +m_active_state->m_size.x / 2;
		}
		if (c->y >= cell()->y - m_active_state->m_size.y / 2 + 1)
		{
			y = cell()->y - m_active_state->m_size.y / 2 + 1;
		}
		else
		{
			y = cell()->y - m_active_state->m_size.y / 2;
		}
		return cell()->m_map->m_items[y][x];
	}
	else
	{
		int x = cell()->x + m_active_state->m_size.x / 2;
		int y = cell()->y - m_active_state->m_size.y / 2;
		return cell()->m_map->m_items[y][x];
	}
}

void GameObject::update_interaction()
{
	if (m_active_state)
	{
		for (auto item = m_active_state->m_item.begin(); item != m_active_state->m_item.end(); item++)
		{
			item->second->update();
		}
	}
}

void GameObject::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_owner)
	{
		m_owner->reset_serialization_index();
	}
	for (auto item = m_state.begin(); item != m_state.end(); item++)
	{
		(*item)->reset_serialization_index();
	}
}

void GameObject::save()
{
	LOG(INFO) << m_name;
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::gameobject;
	fwrite(&t, sizeof(type_e), 1, file);
	FileSystem::instance().serialize_string(m_name, file);
	fwrite(&m_direction, sizeof(object_direction_e), 1, file);
	LOG(INFO) << " --- Запись владельца";
	Serialization_manager::instance().serialize(m_owner);
	LOG(INFO) << " --- Конец запись владельца";
	size_t s = m_state.size();
	fwrite(&s, sizeof(size_t), 1, file);
	for (auto item = m_state.begin(); item != m_state.end(); item++)
	{
		Serialization_manager::instance().serialize(*item);
	}
	Serialization_manager::instance().serialize(m_active_state);
}

void GameObject::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	FileSystem::instance().deserialize_string(m_name, file);
	LOG(INFO) << m_name;
	fread(&m_direction, sizeof(object_direction_e), 1, file);
	LOG(INFO) << " --- Загрузка владельца";
	m_owner = dynamic_cast<Game_object_owner*>(Serialization_manager::instance().deserialize());
	if (m_owner)
	{
		LOG(INFO) << " --- Конец загрузки владельца " << std::to_string((int)m_owner->m_kind);
	}
	else
	{
		LOG(INFO) << " --- Конец загрузки пустого владельца " ;
	}
	
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	Object_state* value;
	for (size_t i = 0; i < s; i++)
	{
		value = static_cast<Object_state*>(Serialization_manager::instance().deserialize());
		m_state.push_back(value);
	}
	m_active_state = static_cast<Object_state*>(Serialization_manager::instance().deserialize());
	set_direction(m_direction);
	update_interaction();
	LOG(INFO) << "конец загрузки объекта";
}

Player::Player(GameObject* object, GameMap* map) :m_object(object), m_map(map)
{
	AI_enemy* ai= static_cast<AI_enemy*>(m_object->m_active_state->m_ai);
	ai->calculate_FOV(m_object, m_map);
	int radius = 0;
	Vision_list* vl = static_cast<Vision_list*>(m_object->m_active_state->get_list(interaction_e::vision));
	AI_FOV current;
	for (auto item = vl->m_effect.begin(); item != vl->m_effect.end(); item++)
	{
		current = static_cast<Vision_item*>(*item)->m_value;
		radius = max(radius, current.radius);
	}
	for (int y = m_object->cell()->y -radius; y < m_object->cell()->y + radius + 1; y++)
	{
		if (!((y<0) || (y>map->m_size.h - 1)))
		{
			for (int x = m_object->cell()->x - radius; x <  m_object->cell()->x + radius + 1; x++)
			{
				if (!((x<0) || (x>map->m_size.w - 1)))
				{
						if (ai->m_fov->m_map[ai->m_fov->m_middle + (y - m_object->cell()->y)][ai->m_fov->m_middle + (x - m_object->cell()->x)].visible)
						{
							m_map->m_items[y][x]->m_notable = true;
						}

				}
			}
		}
	}
	//LOG(INFO) << "Поле зрение " << std::to_string(static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_fov_radius);
	m_actions.push_front(Application::instance().m_actions[action_e::autoexplore]);
	m_actions.push_front(Application::instance().m_actions[action_e::set_motion_path]);
	m_actions.push_front(Application::instance().m_actions[action_e::open_inventory]);
	m_actions.push_front(Application::instance().m_actions[action_e::cell_info]);
	m_actions.push_front(Application::instance().m_actions[action_e::show_parameters]);
	m_actions.push_front(Application::instance().m_actions[action_e::save]);
}

Inventory_cell::Inventory_cell(GameObject* item = nullptr) : m_item(item)
{
	m_kind = entity_e::inventory_cell;
}

void Inventory_cell::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_item)
	{
		// ??? исправить, временная заглушка, необходимо добавить свойство, которое показывает был ли сброшен объект для сохранения уже
		if (m_item->m_serialization_index != 0)
		{
			m_item->reset_serialization_index();
		}
	}
}

void Inventory_cell::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::inventory_cell;
	fwrite(&t, sizeof(type_e), 1, file);
	Serialization_manager::instance().serialize(m_item);
}

void Inventory_cell::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	m_item = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
}

Object_part::Object_part(GameObject* item) :Inventory_cell(item)
{
	m_interaction_message_type = interaction_message_type_e::part;
	m_kind = entity_e::body_part;
};

Object_part* Object_part::clone()
{
	Object_part* effect = new Object_part();
	//effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_kind = m_kind;
	effect->m_name = m_name;
	effect->m_part_kind = m_part_kind;
	effect->m_item = nullptr;
	effect->m_object_state = *m_object_state.clone();
	return effect;
}

bool Object_part::on_turn()
{
	return false;
}

std::string Object_part::get_description()
{
	return "slot";
}

void Object_part::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<" + m_name + ">:");
	info->push_back(std::string(level, '.') + "<эффекты>:");
	for (auto current = m_object_state.m_item.begin(); current != m_object_state.m_item.end(); current++)
	{
		info->push_back(std::string(level + 1, '.') + Application::instance().m_game_object_manager->get_effect_string(current->first) + ":");
		current->second->description(info, level + 2);
	}
}

void Object_part::do_predicat(predicat func)
{ 
	func(this);
	for (auto item = m_object_state.m_item.begin(); item != m_object_state.m_item.end(); item++)
	{
		item->second->do_predicat(func);
	}
}

void Object_part::reset_serialization_index()
{
	m_serialization_index = 0;
	m_object_state.reset_serialization_index();
	if (m_item)
	{
		// ??? исправить, временная заглушка, необходимо добавить свойство, которое показывает был ли сброшен объект для сохранения уже
		if(m_item->m_serialization_index!=0)
		{
			m_item->reset_serialization_index();
		}
	}
}

void Object_part::save()
{
	LOG(INFO) << "Часть";
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::object_part;
	fwrite(&t, sizeof(type_e), 1, file);
	LOG(INFO) << "Состояние";
	m_object_state.save();
	LOG(INFO) << "Конец состояния";
	fwrite(&m_part_kind, sizeof(body_part_e), 1, file);
	FileSystem::instance().serialize_string(m_name, file);
	LOG(INFO) << "--- Запись объекта ---";
	Serialization_manager::instance().serialize(m_item);
	LOG(INFO) << "--- Конец записи объекта ---";
	LOG(INFO) << "Конец листа частей";
}

void Object_part::load()
{
	LOG(INFO) << "Часть";
	FILE* file = Serialization_manager::instance().m_file;
	m_object_state.load();
	fread(&m_part_kind, sizeof(body_part_e), 1, file);
	FileSystem::instance().deserialize_string(m_name, file);
	LOG(INFO) << "Объект в части";
	m_item = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	if (m_item)
	{
		LOG(INFO) << "Конец объекта в части " << m_item->m_name;
		if (m_item->m_owner)
		{
			LOG(INFO) << "Объект имеет владельца";
		}
		else {
			LOG(INFO) << "Объект не имеет владельца";
		}
	}
	LOG(INFO) << "Конец части";
}
