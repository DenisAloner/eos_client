#include "game/GameObject.h"
#include "game/Action.h"
#include "log.h"
#include "AI.h"

Game_object_owner* Game_object_owner::get_owner(entity_e kind)
{
	if (m_owner)
	{
		if (m_owner->m_kind == kind)
		{
			return m_owner;
		}
		else
		{
			return m_owner->get_owner(kind);
		}
	}
	return nullptr;
}

Game_object_owner* Game_object_owner::get_owner()
{
	if (m_owner)
	{
		return m_owner->get_owner();
	}
	else return this;
}

MapCell::MapCell(int x, int y, GameMap* map) :x(x), y(y), m_map(map)
{
	m_kind = entity_e::cell;
	m_notable = false;
	m_owner = nullptr;
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
	fwrite(&(m_map->m_index), sizeof(std::size_t), 1, file);
	LOG(INFO) << "Координаты " << std::to_string(x) << " " << std::to_string(y) << " " << std::to_string(m_map->m_index);
	LOG(INFO) << "Конец записи клетки карты";
}

void MapCell::load()
{
}

Attribute_map::Attribute_map(){};

Interaction_list*  Attribute_map::create_feature_list(feature_list_type_e key, interaction_e name)
{
	Interaction_list* result = Effect_functions::create_feature_list(key, name);
	m_items[name] = result;	
	return result;
}

void Attribute_map::add_effect(interaction_e key, Object_interaction* item)
{
	if (m_items.find(key) != m_items.end())
	{
		m_items[key]->add(item);
	}
}

Interaction_list* Attribute_map::get_list(interaction_e key)
{
	auto value = m_items.find(key);
	if (value != m_items.end())
	{
		return value->second;
	}
	return nullptr;
}

Attribute_map* Attribute_map::clone()
{
	Attribute_map* result = new Attribute_map();
	for (auto item = m_items.begin(); item != m_items.end(); item++)
	{
		result->m_items[item->first] = item->second->clone();
	}
	return result;
}

void  Attribute_map::reset_serialization_index()
{
	m_serialization_index = 0;
	for (auto item = m_items.begin(); item != m_items.end(); item++)
	{
		(*item).second->reset_serialization_index();
	}
}

void Attribute_map::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	size_t s = m_items.size();
	fwrite(&s, sizeof(size_t), 1, file);
	LOG(INFO) << "листов " << std::to_string(s);
	for (auto item = m_items.begin(); item != m_items.end(); item++)
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
	m_items.clear();
	interaction_e ie;
	LOG(INFO) << "листов " << std::to_string(s);
	for (size_t i = 0; i < s; i++)
	{
		fread(&ie, sizeof(interaction_e), 1, file);
		LOG(INFO) << "МАРКЕР АТРИБУТА  - " << std::to_string((int)ie);
		m_items[ie] = dynamic_cast<Interaction_list*>(Serialization_manager::instance().deserialize());
	}
}

Tag_getter::Tag_getter(object_tag_e key) :m_key(key), m_result(nullptr) {};

void Tag_getter::visit(Object_interaction& value)
{
	if (!m_result)
	{
		if (value.m_interaction_message_type == interaction_message_type_e::tag)
		{
			if (static_cast<Object_tag&>(value).m_type == m_key) { m_result = static_cast<Object_tag*>(&value); }
		}
	}
}


bool Attribute_map::get_stat(object_tag_e key)
{
	auto list = m_items.find(interaction_e::tag);
	if (list != m_items.end())
	{
		Tag_list* taglist = static_cast<Tag_list*>(list->second);
		Tag_getter tg(key);
		taglist->do_predicat(tg);
		return tg.m_result;
	}
	return false;
}

Object_tag* Attribute_map::get_tag(object_tag_e key)
{
	auto list = m_items.find(interaction_e::tag);
	if (list != m_items.end())
	{
		Tag_list* taglist = static_cast<Tag_list*>(list->second);
		Tag_getter tg(key);
		taglist->do_predicat(tg);
		return tg.m_result;
	}
	return nullptr;
}

void Attribute_map::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

Object_state::Object_state()
{
	m_layer = 1;
	m_light = nullptr;
	m_tile_manager = nullptr;
	m_icon = GameObjectManager::m_config.m_icons.begin()->second;
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
		state->m_optical = new optical_properties_t{ *m_optical };
	}
	if (m_visibility)
	{
		state->m_visibility = new float;
		*state->m_visibility = *m_visibility;
	}
	for (auto item = m_items.begin(); item != m_items.end(); item++)
	{
		state->m_items[item->first] = item->second->clone();
	}
	return state;
}

void Object_state::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
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
	
	for (auto item = m_items.begin(); item != m_items.end(); item++)
	{
		state->m_items[item->first] = item->second->clone();
	}

	for (auto item = m_equip.m_items.begin(); item != m_equip.m_items.end(); item++)
	{
		state->m_equip.m_items[item->first] = item->second->clone();
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
	m_tile_size = dimension_t(tile_size_x_half*(m_size.x + m_size.y), tile_size_y_half*(m_size.x + m_size.y) + m_size.z * tile_size_y);
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
	fwrite(&m_icon, sizeof(unsigned int), 1, file);
	fwrite(&m_size, sizeof(game_object_size_t), 1, file);
	fwrite(&m_weight, sizeof(float), 1, file);
	FileSystem::instance().serialize_pointer(m_light, type_e::light_t, file);
	FileSystem::instance().serialize_pointer(m_optical, type_e::optical_properties_t, file);
	if(m_tile_manager->m_index>28)
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
	fread(&m_icon, sizeof(unsigned int), 1, file);
	fread(&m_size, sizeof(game_object_size_t), 1, file);
	fread(&m_weight, sizeof(float), 1, file);
	m_light = static_cast<light_t*>(FileSystem::instance().deserialize_pointer(file));
	m_optical = static_cast<optical_properties_t*>(FileSystem::instance().deserialize_pointer(file));
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	LOG(INFO) << "тайл "<<std::to_string(s);
	//m_tile_manager = Application::instance().m_graph->m_tile_managers[s];
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
	m_interaction_message_type = interaction_message_type_e::game_object;
	m_kind = entity_e::game_object;
	m_owner = nullptr;
	m_direction = object_direction_e::down;
	m_selected = false;
	m_active_state = nullptr;
	m_owner = nullptr;
	//rendering_necessary = false;
}

bool GameObject::on_turn()
{
	LOG(FATAL)<<"Метод не описан.";
	return false;
}

Object_interaction* GameObject::clone()
{
	LOG(FATAL) << "Метод не описан.";
	return nullptr;
}

void GameObject::do_predicat(predicat func)
{ 
	func(this); 
	LOG(FATAL) << "Метод не описан.";
}

void GameObject::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
};

void GameObject::turn()
{
	object_direction_e dir;
	if (m_direction == object_direction_e::down)
	{
		dir = object_direction_e::left;
	}
	if (m_direction == object_direction_e::left)
	{
		dir = object_direction_e::top;
	}
	if (m_direction == object_direction_e::top)
	{
		dir = object_direction_e::right;
	}
	if (m_direction == object_direction_e::right)
	{
		dir = object_direction_e::down;
	}
	set_direction(dir);
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
	for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); ++item)
	{
		(*item)->set_tile_size();
	}
	m_direction = dir;
}

void GameObject::set_state(object_state_e state)
{
	for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); ++item)
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
	for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); ++item)
	{
		if ((*item)->m_state == state)
		{
			return (*item);
		}
	}
	return nullptr;
}

void GameObject::add_effect(interaction_e key, Object_interaction* item)
{
	if (m_active_state->m_items.find(key) != m_active_state->m_items.end())
	{
		m_active_state->m_items[key]->add(item);
	}
}

void GameObject::remove_effect(interaction_e key, Object_interaction* item)
{
	if (m_active_state)
	{
		if (m_active_state->m_items.find(key) != m_active_state->m_items.end())
		{
			m_active_state->m_items[key]->remove(item);
		}
	}
}

void GameObject::add_from(interaction_e key, Object_interaction* item)
{
	if (m_active_state->m_items.find(key) != m_active_state->m_items.end())
	{
		m_active_state->m_items[key]->equip(item);
	}
}

void GameObject::add_from(interaction_e key, Interaction_list* feature)
{
	if (m_active_state)
	{
		if (m_active_state->m_items.find(key) == m_active_state->m_items.end())
		{
			m_active_state->m_items[key] = feature;
		}
		else
		{
			m_active_state->m_items[key]->equip(feature);
		}
	}
}

void GameObject::remove_from(interaction_e key, Interaction_list* feature)
{
	if (m_active_state)
	{
		auto item = m_active_state->m_items.find(key);
		if (item != m_active_state->m_items.end())
		{
			if (item->second == feature)
			{
				m_active_state->m_items.erase(key);
			}
			else
			{
				m_active_state->m_items[key]->unequip(feature);
			}
		}
	}
}

Interaction_list* GameObject::get_effect(interaction_e key)
{
	if (m_active_state)
	{
		auto value = m_active_state->m_items.find(key);
		if (value != m_active_state->m_items.end())
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
		auto value = m_active_state->m_items.find(key);
		if (value != m_active_state->m_items.end())
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
		auto value = m_active_state->m_items.find(key);
		if (value != m_active_state->m_items.end())
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
		return m_active_state->get_stat(key);
	}
	return false;
}

Object_tag* GameObject::get_tag(object_tag_e key)
{
	if (m_active_state)
	{
		return m_active_state->get_tag(key);
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
		bool was_changed;
		int i = 0;
		do
		{
			was_changed = false;
			for (auto item = m_active_state->m_items.begin(); item != m_active_state->m_items.end(); ++item)
			{
				was_changed = item->second->update();
				if (was_changed) break;
			}
			i += 1;
		} while (was_changed);
	}
}

GameObject::Action_getter::Action_getter(GameObject* object, std::list<Action_helper_t>& list) : m_object(object), m_list(list) {}

void GameObject::Action_getter::visit(Object_interaction& value)
{
	switch (value.m_interaction_message_type)
	{
	/*case interaction_message_type_e::action:
	{
		m_list.push_back(Action_helper_t(static_cast<Action*>(&value)));
		break;
	}*/
	case interaction_message_type_e::part:
	{
		Object_part& op = dynamic_cast<Object_part&>(value);
		Tag_list* tl = static_cast<Tag_list*>(op.m_attributes.get_list(interaction_e::tag));
		Object_tag* t;
		if (tl)
		{
			for (auto tag = tl->m_items.begin(); tag != tl->m_items.end(); ++tag)
			{
				t = static_cast<Object_tag*>(*tag);
				switch (t->m_type)
				{
				case object_tag_e::can_transfer_object:
				{
					if (op.m_item)
					{
						Parameter* p = new Parameter(parameter_type_e::destination);
						(*p)[0].set(m_object);
						(*p)[1].set(op.m_item);
						(*p)[3].set(&op);
						m_list.push_back(Action_helper_t(Application::instance().m_actions[action_e::move_out], p));
					}
					else
					{
						Parameter* p = new Parameter(parameter_type_e::destination);
						(*p)[0].set(m_object);
						(*p)[2].set(&op);
						(*p)[3].set(&op);
						m_list.push_back(Action_helper_t(Application::instance().m_actions[action_e::pick], p));
					}
					break;
				}
				}
			}
		}

		Action_list* al = static_cast<Action_list*>(op.m_attributes.get_list(interaction_e::action));
		Action* a;
		if (al)
		{
			for (auto action = al->m_items.begin(); action != al->m_items.end(); ++action)
			{
				a = static_cast<Action*>(*action);
				switch (a->m_kind)
				{
				case action_e::pick:
				{
					Parameter* p = new Parameter(parameter_type_e::destination);
					(*p)[0].set(m_object);
					(*p)[2].set(&op);
					//m_common_actions.pick = true;
					m_list.push_back(Action_helper_t(a, p));
					break;
				}
				case action_e::move_step:
				{
					Parameter* p = new Parameter(parameter_type_e::position);
					(*p)[0].set(m_object);
					m_list.push_back(Action_helper_t(a, p));
					break;
				}
				case action_e::turn:
				{
					Parameter* p = new Parameter(parameter_type_e::direction);
					(*p)[0].set(m_object);
					m_list.push_back(Action_helper_t(a, p));
					break;
				}
				case action_e::hit_melee:
				{
					Parameter* p = new Parameter(parameter_type_e::interaction_cell);
					(*p)[0].set(m_object);
					(*p)[2].set(&op);
					m_list.push_back(Action_helper_t(a, p));
					break;
				}
				}
			}
		}
		if (op.m_item)
		{
			al = static_cast<Action_list*>(op.m_item->get_effect(interaction_e::action));
			if (al)
			{
				for (auto action = al->m_items.begin(); action != al->m_items.end(); ++action)
				{
					a = static_cast<Action*>(*action);
					switch (a->m_kind)
					{
					case action_e::hit_melee:
					{
						Parameter* p = new Parameter(parameter_type_e::interaction_cell);
						(*p)[0].set(m_object);
						(*p)[2].set(&op);
						m_list.push_back(Action_helper_t(a, p));
						break;
					}
					case action_e::shoot:
					{
						Parameter* p = new Parameter(parameter_type_e::bow_shoot);
						(*p)[0].set(m_object);
						(*p)[2].set(&op);
						m_list.push_back(Action_helper_t(a, p));
						break;
					}
					}
				}
			}
		}
		break;
	}
	}
}

void GameObject::get_actions_list(std::list<Action_helper_t>& value)
{
	Action_getter ag(this, value);
	
	Action_list* al= static_cast<Action_list*>(get_effect(interaction_e::action));
	if(al)
	{
		al->do_predicat(ag);
	}

	Parts_list* parts = get_parts_list(interaction_e::body);
	parts->do_predicat(ag);
}

void GameObject::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_owner)
	{
		m_owner->reset_serialization_index();
	}
	for (auto item = m_state.begin(); item != m_state.end(); ++item)
	{
		(*item)->reset_serialization_index();
	}
}

void GameObject::save()
{
	/*LOG(INFO) << m_name;
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
	for (auto item = m_state.begin(); item != m_state.end(); ++item)
	{
		Serialization_manager::instance().serialize(*item);
	}
	Serialization_manager::instance().serialize(m_active_state);*/
}

void GameObject::load()
{
	/*FILE* file = Serialization_manager::instance().m_file;
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
	LOG(INFO) << "конец загрузки объекта";*/
}

Player::Player(GameObject* object, GameMap* map) :m_object(object), m_map(map)
{
	AI_enemy* ai = static_cast<AI_enemy*>(m_object->m_active_state->m_ai);
	ai->calculate_FOV(m_object, map);
	Vision_list* vl = static_cast<Vision_list*>(m_object->m_active_state->get_list(interaction_e::vision));


	int radius = vl->m_max_radius;

	int xc = m_object->cell()->x;
	int yc = m_object->cell()->y;
	int dx = m_object->m_active_state->m_size.x;
	int dy = m_object->m_active_state->m_size.y;

	int xs = ((dx - 1) >> 1);
	int ys = ((dy - 1) >> 1);

	int x_start = xc - radius + xs;
	x_start = max(x_start, 0);
	x_start = min(x_start, m_map->m_size.w - 1);
	int x_end = xc + radius + xs;
	x_end = max(x_end, 0);
	x_end = min(x_end, m_map->m_size.w - 1);
	int y_start = yc - radius - ys;
	y_start = max(y_start, 0);
	y_start = min(y_start, m_map->m_size.h - 1);
	int y_end = yc + radius - ys;
	y_end = max(y_end, 0);
	y_end = min(y_end, m_map->m_size.h - 1);

	for (int y = y_start; y < y_end + 1; y++)
	{
		for (int x = x_start; x < x_end + 1; x++)
		{
			if (ai->m_fov->m_map[y - y_start][x - x_start].visible)
			{
				m_map->m_items[y][x]->m_notable = true;
			}
		}
	}

	//LOG(INFO) << "Поле зрение " << std::to_string(static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_fov_radius);
	m_actions.push_front(Application::instance().m_actions[action_e::autoexplore]);
	m_actions.push_front(Application::instance().m_actions[action_e::set_motion_path]);
	//m_actions.push_front(Application::instance().m_actions[action_e::open_inventory]);
	m_actions.push_front(Application::instance().m_actions[action_e::cell_info]);
	m_actions.push_front(Application::instance().m_actions[action_e::equip]);
	m_actions.push_front(Application::instance().m_actions[action_e::show_parameters]);
	m_actions.push_front(Application::instance().m_actions[action_e::save]);
	m_actions.push_front(Application::instance().m_actions[action_e::load]);

	m_object->event_update += std::bind(&Event<VoidEventArgs>::operator(),&event_update, std::placeholders::_1);
	
}

void Player::get_actions_list(std::list<Action_helper_t>& value)
{
	for (auto item = m_actions.begin(); item != m_actions.end(); ++item)
	{
		value.push_back(Action_helper_t(*item));
	}
	m_object->get_actions_list(value);
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
	m_part_kind = body_part_e::container;
	m_owner = nullptr;
	m_attributes.create_feature_list(feature_list_type_e::parts, interaction_e::body);
};

Object_part* Object_part::clone()
{
	Object_part* result = new Object_part();
	result->m_kind = m_kind;
	result->m_name = m_name;
	result->m_part_kind = m_part_kind;
	result->m_item = nullptr;
	result->m_attributes = *m_attributes.clone();
	return result;
}

bool Object_part::on_turn()
{
	return false;
}

void Object_part::do_predicat(Visitor& helper)
{ 
	helper.visit(*this);
	for (auto item = m_attributes.m_items.begin(); item != m_attributes.m_items.end(); item++)
	{
		item->second->do_predicat(helper);
	}
}

void Object_part::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

void Object_part::reset_serialization_index()
{
	m_serialization_index = 0;
	m_attributes.reset_serialization_index();
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
	/*LOG(INFO) << "Часть";
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::object_part;
	fwrite(&t, sizeof(type_e), 1, file);
	LOG(INFO) << "Состояние";
	m_attributes.save();
	LOG(INFO) << "Конец состояния";
	fwrite(&m_part_kind, sizeof(body_part_e), 1, file);
	FileSystem::instance().serialize_string(m_name, file);
	LOG(INFO) << "--- Запись объекта ---";
	Serialization_manager::instance().serialize(m_item);
	LOG(INFO) << "--- Конец записи объекта ---";
	LOG(INFO) << "Конец листа частей";*/
}

void Object_part::load()
{
	/*LOG(INFO) << "Часть";
	FILE* file = Serialization_manager::instance().m_file;
	m_attributes.load();
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
	LOG(INFO) << "Конец части";*/
}