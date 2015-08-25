#include "Effect.h"
#include "Application.h"
#include "game\graphics\GUI_TextBox.h"


// Interaction_list

Interaction_list::Interaction_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
	m_list_type = feature_list_type_e::generic;
}

bool Interaction_list::on_turn()
{
	bool result = false;
	for (auto item = m_effect.begin(); item != m_effect.end();)
	{
		if ((*item)->on_turn())
		{
			item = m_effect.erase(item);
		}
		else
			++item;
	}
	return true;
}

std::string Interaction_list::get_description()
{
	std::string result = "";
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		result += (*current)->get_description() + ",";
	}
	return result;
}

void Interaction_list::update()
{
}

Interaction_list* Interaction_list::clone()
{
	Interaction_list* result = new Interaction_list();
	result->m_list_type = m_list_type;
	Object_interaction* c;
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		c = (*item)->clone();
		if (c) { result->m_effect.push_back(c); }

	}
	return result;
}

void Interaction_list::apply_effect(GameObject* unit, Object_interaction* object)
{
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->apply_effect(unit, object);
	}
}

void Interaction_list::do_predicat(predicat func)
{
	func(this);
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->do_predicat(func);
	}
}

void Interaction_list::description(std::list<std::string>* info, int level)
{
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		(*current)->description(info, level);
	}
}

void Interaction_list::reset_serialization_index()
{
	m_serialization_index = 0;
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->reset_serialization_index();
	}
}

void Interaction_list::save(FILE* file)
{
	type_e t = type_e::interaction_list;
	fwrite(&t, sizeof(type_e), 1, file);
	size_t s = m_effect.size();
	fwrite(&s, sizeof(size_t), 1, file);
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->save(file);
	}
}

void Interaction_list::load(FILE* file)
{
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	m_effect.clear();
	for (size_t i = 0; i < s; i++)
	{
		m_effect.push_back(FileSystem::instance().deserialize_impact(file));
	}
}

// Patameter_list

Parameter_list::Parameter_list(interaction_e subtype) :m_subtype(subtype)
{
	m_list_type = feature_list_type_e::parameter;
};

Parameter_list::Parameter_list()
{
	m_list_type = feature_list_type_e::parameter;
};

void Parameter_list::update_list(Object_interaction* list)
{

	switch (list->m_interaction_message_type)
	{
	case interaction_message_type_e::list:
	{
		Interaction_list* list_item = static_cast<Interaction_list*>(list);
		for (auto current = list_item->m_effect.begin(); current != list_item->m_effect.end(); current++)
		{
			update_list((*current));
		}
		break;
	}
	case interaction_message_type_e::slot_time:
	{
		LOG(INFO) << "buff in parameter";
		Interaction_time* item = static_cast<Interaction_time*>(list);
		update_list(item->m_value);
		break;
	}
	default:
	{
		Effect* item;
		item = static_cast<Effect*>(list);
		switch (item->m_subtype)
		{
		case effect_e::value:
		{
			m_value = m_value + item->m_value;
			break;
		}
		case effect_e::limit:
		{
			m_limit = m_limit + item->m_value;
			break;
		}
		}
	}
	}

}

void Parameter_list::update()
{
	m_value = m_basic_value;
	m_limit = m_basic_limit;
	update_list(this);
}

std::string Parameter_list::get_description()
{
	std::string result = std::to_string(m_value) + "/" + std::to_string(m_limit) + ",";
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		result += (*current)->get_description() + ",";
	}
	return result;
}

Parameter_list* Parameter_list::clone()
{
	Parameter_list* result = new Parameter_list(m_subtype);
	result->m_list_type = m_list_type;
	result->m_basic_limit = m_basic_limit;
	result->m_basic_value = m_basic_value;
	result->m_value = m_value;
	result->m_limit = m_limit;
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->add((*item)->clone());
	}
	return result;
}

void Parameter_list::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + std::to_string(m_value) + "(" + std::to_string(m_basic_value) + ")/" + std::to_string(m_limit) + "(" + std::to_string(m_basic_limit) + "):");
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		(*current)->description(info, level);
	}
}

void Parameter_list::save(FILE* file)
{
	type_e t = type_e::parameter_list;
	fwrite(&t, sizeof(type_e), 1, file);

	fwrite(&m_subtype, sizeof(interaction_e), 1, file);
	fwrite(&m_basic_value, sizeof(int), 1, file);
	fwrite(&m_basic_limit, sizeof(int), 1, file);

	size_t s = m_effect.size();
	fwrite(&s, sizeof(size_t), 1, file);
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->save(file);
	}
}

void Parameter_list::load(FILE* file)
{
	fread(&m_subtype, sizeof(interaction_e), 1, file);
	fread(&m_basic_value, sizeof(int), 1, file);
	fread(&m_basic_limit, sizeof(int), 1, file);

	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	m_effect.clear();
	for (size_t i = 0; i < s; i++)
	{
		m_effect.push_back(FileSystem::instance().deserialize_impact(file));
	}
}

// Tag_list

Tag_list::Tag_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
	m_list_type = feature_list_type_e::tag;
}

Tag_list* Tag_list::clone()
{
	Tag_list* result = new Tag_list();
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->m_effect.push_back((*item)->clone());
	}
	return result;
}

void Tag_list::save(FILE* file)
{
	type_e t = type_e::tag_list;
	fwrite(&t, sizeof(type_e), 1, file);
	size_t s = m_effect.size();
	fwrite(&s, sizeof(size_t), 1, file);
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->save(file);
	}
}

void Tag_list::load(FILE* file)
{
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	m_effect.clear();
	for (size_t i = 0; i < s; i++)
	{
		m_effect.push_back(FileSystem::instance().deserialize_impact(file));
	}
}

// Parts_list

Parts_list::Parts_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
	m_list_type = feature_list_type_e::parts;
}

Parts_list* Parts_list::clone()
{
	Parts_list* result = new Parts_list();
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->m_effect.push_back((*item)->clone());
	}
	return result;
}

void Parts_list::save(FILE* file)
{
	type_e t = type_e::parts_list;
	fwrite(&t, sizeof(type_e), 1, file);
	size_t s = m_effect.size();
	fwrite(&s, sizeof(size_t), 1, file);
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->save(file);
	}
}

void Parts_list::load(FILE* file)
{
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	m_effect.clear();
	for (size_t i = 0; i < s; i++)
	{
		m_effect.push_back(FileSystem::instance().deserialize_impact(file));
	}
}

// Action_list

Action_list::Action_list() 
{
	m_interaction_message_type = interaction_message_type_e::list;
	m_list_type = feature_list_type_e::action;
};

Interaction_list* Action_list::clone()
{
	Action_list* result = new Action_list();
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->m_effect.push_back((*item)->clone());
	}
	return result;
}

void Action_list::save(FILE* file)
{
	type_e t = type_e::action_list;
	fwrite(&t, sizeof(type_e), 1, file);
	size_t s = m_effect.size();
	fwrite(&s, sizeof(size_t), 1, file);
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->save(file);
	}
}

void Action_list::load(FILE* file)
{
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	m_effect.clear();
	for (size_t i = 0; i < s; i++)
	{
		m_effect.push_back(FileSystem::instance().deserialize_impact(file));
	}
}


// Slot_set_state

Slot_set_state* Slot_set_state::clone()
{
	Slot_set_state* effect = new Slot_set_state();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_value = m_value;
	return effect;
}

void Slot_set_state::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "�������� ���������");
}

void Slot_set_state::apply_effect(GameObject* unit, Object_interaction* object)
{
	unit->set_state(m_value);
}

void Slot_set_state::save(FILE* file)
{
	type_e t = type_e::slot_set_state;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_value, sizeof(object_state_e), 1, file);
}

void Slot_set_state::load(FILE* file)
{
}

// Slot_select_cell

void Slot_select_cell::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "������� �������");
}

Slot_select_cell* Slot_select_cell::clone()
{
	Slot_select_cell* effect = new Slot_select_cell();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_value = m_value;
	return effect;
}

void Slot_select_cell::apply_effect(GameObject* unit, Object_interaction* object)
{
	MapCell* c = unit->cell();
	MapCell* n = unit->cell();
	GameMap* map = c->m_map;
	if (map->check(c->x + 1, c->y))
	{
		n = map->m_items[c->y][c->x + 1];
		if (n->m_items.size() == 1)
		{
			GameObject* obj = Application::instance().m_game_object_manager->new_object(m_value);
			obj->set_direction(object_direction_e::left);
			map->add_object(obj, n);
		}
	};
	if (map->check(c->x + 1, c->y + 1))
	{
		n = map->m_items[c->y + 1][c->x + 1];
		if (n->m_items.size() == 1)
		{
			GameObject* obj = Application::instance().m_game_object_manager->new_object(m_value);
			obj->set_direction(object_direction_e::left);
			map->add_object(obj, n);
		}
	};
}

void Slot_select_cell::save(FILE* file)
{
	type_e t = type_e::slot_select_cell;
	fwrite(&t, sizeof(type_e), 1, file);
	FileSystem::instance().serialize_string(m_value, file);
}

void Slot_select_cell::load(FILE* file)
{
}


// Slot_allocator

void Slot_allocator::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "������ �� ���� ������");

}

Slot_allocator* Slot_allocator::clone()
{
	Slot_allocator* effect = new Slot_allocator();
	effect->m_interaction_message_type = m_interaction_message_type;
	return effect;
}

void Slot_allocator::apply_effect(GameObject* unit, Object_interaction* object)
{
	MapCell* c = unit->cell();
	GameMap* map = c->m_map;
	if (map->check(c->x + 1, c->y))
	{
		m_value = map->m_items[c->y][c->x + 1];
	};
}

void Slot_allocator::save(FILE* file)
{
	type_e t = type_e::slot_allocator;
	fwrite(&t, sizeof(type_e), 1, file);
}

void Slot_allocator::load(FILE* file)
{
}

// Slot_mover

void Slot_mover::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "����������� ������:");
	m_value->description(info, level + 1);
}

Slot_mover* Slot_mover::clone()
{
	Slot_mover* effect = new Slot_mover();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_value = m_value;
	return effect;
}

void Slot_mover::apply_effect(GameObject* unit, Object_interaction* object)
{

	MapCell* c = unit->cell();
	GameMap* map = c->m_map;
	m_value->apply_effect(unit, object);
	map->move_object(unit, m_value->m_value);
}

void Slot_mover::save(FILE* file)
{
	type_e t = type_e::slot_mover;
	fwrite(&t, sizeof(type_e), 1, file);
}

void Slot_mover::load(FILE* file)
{
}

// Interaction_slot

Interaction_slot::Interaction_slot()
{
}

bool Interaction_slot::on_turn()
{
	return m_value->on_turn();
}

void Interaction_slot::do_predicat(predicat func)
{
	func(this);
	m_value->do_predicat(func);
}

void Interaction_slot::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_value)
	{
		m_value->reset_serialization_index();
	}
}

void Interaction_slot::save(FILE* file)
{
	type_e t = type_e::interaction_slot;
	fwrite(&t, sizeof(type_e), 1, file);
}

void Interaction_slot::load(FILE* file)
{
}

// Interaction_copyist

Interaction_copyist::Interaction_copyist()
{
}

std::string Interaction_copyist::get_description()
{
	return "slot";
}


Object_interaction* Interaction_copyist::clone()
{
	Interaction_copyist* effect = new Interaction_copyist();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_copyist::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<��� ���������:" + Application::instance().m_game_object_manager->get_effect_string(m_subtype) + ">:");
	m_value->description(info, level + 1);
}

void Interaction_copyist::apply_effect(GameObject* unit, Object_interaction* object)
{
	auto i = unit->get_effect(m_subtype);
	if (i)
	{
		m_value->apply_effect(unit, i);
	}
}

void Interaction_copyist::save(FILE* file)
{
	type_e t = type_e::interaction_copyist;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_subtype, sizeof(interaction_e), 1, file);
	m_value->save(file);
}

void Interaction_copyist::load(FILE* file)
{
	fread(&m_subtype, sizeof(interaction_e), 1, file);
	m_value = FileSystem::instance().deserialize_impact(file);
}

// Interaction_prefix

Interaction_prefix::Interaction_prefix() {};

void Interaction_prefix::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<��� �������:" + Application::instance().m_game_object_manager->get_effect_prefix_string(m_subtype) + ">:");
	m_value->description(info, level + 1);
}

Interaction_prefix* Interaction_prefix::clone()
{
	Interaction_prefix* effect = new Interaction_prefix();
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_prefix::apply_effect(GameObject* unit, Object_interaction* object)
{
	Tag_list* i = static_cast<Tag_list*>(unit->get_effect(interaction_e::tag));
	if (i)
	{
		for (auto item = i->m_effect.begin(); item != i->m_effect.end(); item++)
		{
			(*item)->apply_effect(unit, this);
		}
	}
	if (m_value) { m_value->apply_effect(unit, object); }
}

std::string Interaction_prefix::get_description()
{
	return "none";
}

void Interaction_prefix::save(FILE* file)
{
	type_e t = type_e::interaction_prefix;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_subtype, sizeof(effect_prefix_e), 1, file);
	m_value->save(file);
}

void Interaction_prefix::load(FILE* file)
{
	fread(&m_subtype, sizeof(effect_prefix_e), 1, file);
	m_value = FileSystem::instance().deserialize_impact(file);
}

// Interaction_addon

Interaction_addon::Interaction_addon() {};

std::string Interaction_addon::get_description()
{
	return "slot";
}

Object_interaction* Interaction_addon::clone()
{
	Interaction_addon* effect = new Interaction_addon();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_addon::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<��������� �������:" + Application::instance().m_game_object_manager->get_effect_string(m_subtype) + ">:");
	m_value->description(info, level + 1);
}

void Interaction_addon::apply_effect(GameObject* unit, Object_interaction* object)
{
	unit->add_effect(m_subtype, m_value->clone());
}

void Interaction_addon::save(FILE* file)
{
	type_e t = type_e::interaction_addon;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_subtype, sizeof(interaction_e), 1, file);
	m_value->save(file);
}

void Interaction_addon::load(FILE* file)
{
	fread(&m_subtype, sizeof(interaction_e), 1, file);
	m_value = FileSystem::instance().deserialize_impact(file);
}

// Interaction_time

Interaction_time::Interaction_time()
{

}

bool Interaction_time::on_turn()
{
	m_turn -= 1;
	if (m_turn == 0)
	{
		m_value = nullptr;
		return true;
	}
	else return false;
}

std::string Interaction_time::get_description()
{
	return "slot";
}

Object_interaction* Interaction_time::clone()
{
	Interaction_time* effect = new Interaction_time();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_turn = m_turn;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_time::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<���������� ��������� �������:" + std::to_string(m_turn) + ">:");
	m_value->description(info, level + 1);
}

void Interaction_time::apply_effect(GameObject* unit, Object_interaction* object)
{
	m_value->apply_effect(unit, object);
}

void Interaction_time::save(FILE* file)
{
	type_e t = type_e::interaction_time;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_turn, sizeof(int), 1, file);
	m_value->save(file);
}

void Interaction_time::load(FILE* file)
{
	fread(&m_turn, sizeof(int), 1, file);
	m_value = FileSystem::instance().deserialize_impact(file);
}

// Interaction_timer

Interaction_timer::Interaction_timer()
{
	m_interaction_message_type = interaction_message_type_e::slot_time;
}

bool Interaction_timer::on_turn()
{
	m_turn += 1;
	if (m_turn > m_period)
	{
		m_turn = 0;
	}
	return m_value->on_turn();
}

std::string Interaction_timer::get_description()
{
	return "slot";
}

Object_interaction* Interaction_timer::clone()
{
	Interaction_timer* effect = new Interaction_timer();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_turn = m_turn;
	effect->m_period = m_period;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_timer::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<��������� ��������� �������:" + std::to_string(m_turn) + "(" + std::to_string(m_period) + ")>:");
	m_value->description(info, level + 1);
}

void Interaction_timer::apply_effect(GameObject* unit, Object_interaction* object)
{
	if (m_turn == m_period - 1)
	{
		m_value->apply_effect(unit, object);
	}
}

void Interaction_timer::save(FILE* file)
{
	type_e t = type_e::interaction_timer;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_turn, sizeof(int), 1, file);
	fwrite(&m_period, sizeof(int), 1, file);
	m_value->save(file);
}

void Interaction_timer::load(FILE* file)
{
	fread(&m_turn, sizeof(int), 1, file);
	fread(&m_period, sizeof(int), 1, file);
	m_value = FileSystem::instance().deserialize_impact(file);
}


// Effect

Effect::Effect()
{
	m_interaction_message_type = interaction_message_type_e::single;
}

bool Effect::on_turn()
{
	return false;
}

Object_interaction* Effect::clone()
{
	Effect* effect = new Effect();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value;
	return effect;
}

std::string Effect::get_description()
{
	return Application::instance().m_game_object_manager->get_effect_subtype_string(m_subtype) + ":" + std::to_string(m_value);
}

void Effect::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (m_subtype)
	{
	case effect_e::value:
	{
		Parameter_list* item = static_cast<Parameter_list*>(object);
		item->m_basic_value = item->m_basic_value + m_value;
		if (item->m_basic_value <=1)
		{ 
			item->m_basic_value = 0;
			Interaction_copyist* copyist = new Interaction_copyist();
			copyist->m_subtype = item->m_subtype;
			copyist->m_value = clone();
			Interaction_prefix* prefix = new Interaction_prefix();
			prefix->m_value = copyist;
			prefix->m_subtype = effect_prefix_e::parameter_change;
			prefix->apply_effect(unit, object);
		}
		break;
	}
	case effect_e::limit:
	{
		Parameter_list* item = static_cast<Parameter_list*>(object);
		item->m_basic_limit = item->m_basic_limit + m_value;
		break;
	}
	}
}


void Effect::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level,'.')+Application::instance().m_game_object_manager->get_effect_subtype_string(m_subtype) + ":" + std::to_string(m_value));
}

void Effect::save(FILE* file)
{
	type_e t = type_e::effect;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_value, sizeof(int), 1, file);
	fwrite(&m_subtype, sizeof(effect_e), 1, file);
}

void Effect::load(FILE* file)
{
	fread(&m_value, sizeof(int), 1, file);
	fread(&m_subtype, sizeof(effect_e), 1, file);
}

// Object_tag

Object_tag::Object_tag(object_tag_e key) :m_type(key){};

void Object_tag::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<" + Application::instance().m_game_object_manager->get_object_tag_string(m_type) + ">");
}

std::string Object_tag::get_description()
{
	return "none";
}

bool Object_tag::on_turn()
{
	return false;
}

// ObjectTag::Poison_resist

ObjectTag::Poison_resist::Poison_resist() :Object_tag(object_tag_e::poison_resist){};

ObjectTag::Poison_resist* ObjectTag::Poison_resist::clone()
{
	ObjectTag::Poison_resist* effect = new ObjectTag::Poison_resist();
	return effect;
}

void ObjectTag::Poison_resist::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::poison_damage:
	{
		static_cast<Effect*>(prefix->m_value)->m_value /= 2;
		break;
	}
	}
}

void ObjectTag::Poison_resist::save(FILE* file)
{
	type_e t = type_e::tag_poison_resist;
	fwrite(&t, sizeof(type_e), 1, file);
}

void ObjectTag::Poison_resist::load(FILE* file)
{
}

// ObjectTag::Mortal

ObjectTag::Mortal::Mortal() :Object_tag(object_tag_e::mortal){};

ObjectTag::Mortal* ObjectTag::Mortal::clone()
{
	ObjectTag::Mortal* effect = new ObjectTag::Mortal();
	return effect;
}

void ObjectTag::Mortal::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::parameter_change:
	{
		Interaction_copyist* copyist = static_cast<Interaction_copyist*>(prefix->m_value);
		if (copyist->m_subtype == interaction_e::health)
		{
			auto feat = unit->get_parameter(interaction_e::health);
			if (feat)
			{
				if (feat->m_basic_value == 0)
				{
					unit->set_state(object_state_e::dead);
				}
			}
		}
		break;
	}
	}
}

void ObjectTag::Mortal::save(FILE* file)
{
	type_e t = type_e::tag_mortal;
	fwrite(&t, sizeof(type_e), 1, file);
}

void ObjectTag::Mortal::load(FILE* file)
{
}

// ObjectTag::Purification_from_poison

ObjectTag::Purification_from_poison::Purification_from_poison() :Object_tag(object_tag_e::purification_from_poison){};

ObjectTag::Purification_from_poison* ObjectTag::Purification_from_poison::clone()
{
	ObjectTag::Purification_from_poison* effect = new ObjectTag::Purification_from_poison();
	return effect;
}

void ObjectTag::Purification_from_poison::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::poison_damage:
	{
		Parameter_list* p = unit->get_parameter(interaction_e::poison);
			if (p)
			{
				Effect* e = static_cast<Effect*>(prefix->m_value);
				p->m_basic_value -= e->m_value;
				if (p->m_basic_value > p->m_basic_limit)
				{
					e->m_value = p->m_basic_limit - p->m_basic_value;
					p->m_basic_value = p->m_basic_limit;
				}
				else
				{
					e->m_value = 0;
				}
			}
		break;
	}
	}
}

void ObjectTag::Purification_from_poison::save(FILE* file)
{
	type_e t = type_e::tag_purification_from_poison;
	fwrite(&t, sizeof(type_e), 1, file);
}

void ObjectTag::Purification_from_poison::load(FILE* file)
{
}

// ObjectTag::Activator::Activator

ObjectTag::Activator::Activator() :Object_tag(object_tag_e::activator){};

ObjectTag::Activator* ObjectTag::Activator::clone()
{
	ObjectTag::Activator* effect = new ObjectTag::Activator();
	effect->m_link = m_link;
	return effect;
}

void ObjectTag::Activator::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::state_change:
	{
		Slot_set_state* e = static_cast<Slot_set_state*>(prefix->m_value);
		if (e)
		{
			for (auto l = m_link.begin(); l != m_link.end(); l++)
			{
				(*l)->set_state(e->m_value);
			}
		}
		break;
	}
	}
}

void ObjectTag::Activator::save(FILE* file)
{
}

void ObjectTag::Activator::load(FILE* file)
{
}

// ObjectTag::Fast_move

ObjectTag::Fast_move::Fast_move() :Object_tag(object_tag_e::fast_move) {};

ObjectTag::Fast_move* ObjectTag::Fast_move::clone()
{
	ObjectTag::Fast_move* effect = new ObjectTag::Fast_move();
	return effect;
}

void ObjectTag::Fast_move::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::action:
	{
		Action_wrapper* e = static_cast<Action_wrapper*>(prefix->m_value);
		if (e)
		{
			if (e->m_action->m_kind == action_e::move)
			{
				if (e->m_decay != 1)
				{
					e->m_decay /= 2;
				}
			}
		}
		break;
	}
	}
}

void ObjectTag::Fast_move::save(FILE* file)
{
	type_e t = type_e::tag_fast_move;
	fwrite(&t, sizeof(type_e), 1, file);
}

void ObjectTag::Fast_move::load(FILE* file)
{
}

// ObjectTag::Label

ObjectTag::Label::Label(object_tag_e type) :Object_tag(type){};

ObjectTag::Label* ObjectTag::Label::clone()
{
	ObjectTag::Label* effect = new ObjectTag::Label(m_type);
	return effect;
}

void ObjectTag::Label::apply_effect(GameObject* unit, Object_interaction* object){};

void ObjectTag::Label::save(FILE* file)
{
	type_e t = type_e::tag_label;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_type, sizeof(object_tag_e), 1, file);
}

void ObjectTag::Label::load(FILE* file)
{
	fread(&m_type, sizeof(object_tag_e), 1, file);
}
