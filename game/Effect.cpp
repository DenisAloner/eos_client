#include "Effect.h"
#include "Application.h"
#include "game\graphics\GUI_TextBox.h"

//class Application;

Slot_set_state* Slot_set_state::clone()
{
	Slot_set_state* effect = new Slot_set_state();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_value = m_value;
	return effect;
}

void Slot_set_state::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "изменить состояние");
}

void Slot_set_state::apply_effect(GameObject* unit, Object_interaction* object)
{
	unit->set_state(m_value);
}

void Slot_select_cell::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "выбрать локацию");
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
			obj->set_direction(ObjectDirection_Left);
			map->add_object(obj, n);
		}
	};
	if (map->check(c->x + 1, c->y + 1))
	{
		n = map->m_items[c->y + 1][c->x + 1];
		if (n->m_items.size() == 1)
		{
			GameObject* obj = Application::instance().m_game_object_manager->new_object(m_value);
			obj->set_direction(ObjectDirection_Left);
			map->add_object(obj, n);
		}
	};
}

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

Effect::Effect()
{
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
			Interaction_prefix_ex* prefix = new Interaction_prefix_ex();
			prefix->m_value = object;
			prefix->m_subtype = effect_prefix_e::parameter_change;
			prefix->m_effect = clone();
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

Interaction_list::Interaction_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
}

bool Interaction_list::on_turn()
{
	bool result = false;
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		if ((*item)->on_turn())
		{
			result = true;
		}
	}
	return result;
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
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->m_effect.push_back((*item)->clone());
	}
	return result;
}

Tag_list::Tag_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
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

Parts_list::Parts_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
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

Parameter_list::Parameter_list(interaction_e subtype) :m_subtype(subtype){};

void Parameter_list::update_list(Interaction_list* list)
{
	Effect* item;
	Interaction_list* list_item;
	for (auto current = list->m_effect.begin(); current != list->m_effect.end(); current++)
	{
		switch ((*current)->m_interaction_message_type)
		{
		case interaction_message_type_e::list:
		{
			update_list(static_cast<Interaction_list*>(*current));
			break;
		}
		default:
		{
			item = static_cast<Effect*>(*current);
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
}

void Parameter_list::update()
{
	m_value = m_basic_value;
	m_limit = m_basic_limit;
	update_list(this);
}

void Interaction_list::apply_effect(GameObject* unit, Object_interaction* object)
{
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->apply_effect(unit,object);
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

//void Interaction_list::add(Object_interaction* item)
//{
//	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
//	{
//		(*item)->apply(object);
//	}
//}


void Effect::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level,'.')+Application::instance().m_game_object_manager->get_effect_subtype_string(m_subtype) + ":" + std::to_string(m_value));
}

void Interaction_copyist::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<тип параметра:" + Application::instance().m_game_object_manager->get_effect_string(m_subtype) + ">:");
	m_value->description(info,level+1);
}

void Interaction_list::description(std::list<std::string>* info, int level)
{
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		(*current)->description(info,level);
	}
}

void Parameter_list::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + std::to_string(m_value) + "(" + std::to_string(m_basic_value) + ")/" + std::to_string(m_limit) + "(" + std::to_string(m_basic_limit) + "):");
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		(*current)->description(info,level);
	}
}

Interaction_timer::Interaction_timer()
{

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
	info->push_back(std::string(level, '.') + "<цикличное наложение эффекта:" + std::to_string(m_turn) + "(" + std::to_string(m_period) + ")>:");
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

void Interaction_timer::apply_effect(GameObject* unit, Object_interaction* object)
{
	if (m_turn == m_period - 1)
	{
		m_value->apply_effect(unit, object);
	}
}

Interaction_copyist::Interaction_copyist()
{
}

Interaction_time::Interaction_time()
{

}

bool Interaction_time::on_turn()
{
	m_turn -= 1;
	return m_turn == 0;
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
	info->push_back(std::string(level, '.') + "<длительное наложение эффекта:" + std::to_string(m_turn) + ">:");
	m_value->description(info, level + 1);
}

void Interaction_time::apply_effect(GameObject* unit, Object_interaction* object)
{
	m_value->apply_effect(unit,object);
}

std::string Interaction_prefix::get_description()
{
	return "none";
}

Interaction_prefix::Interaction_prefix(){};

void Interaction_prefix::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<тип эффекта:" + Application::instance().m_game_object_manager->get_effect_prefix_string(m_subtype) + ">:");
	m_value->description(info, level + 1);
}

Interaction_prefix* Interaction_prefix::clone()
{
	Interaction_prefix* effect = new Interaction_prefix();
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

Interaction_prefix_ex* Interaction_prefix_ex::clone()
{
	Interaction_prefix_ex* effect = new Interaction_prefix_ex();
	effect->m_subtype = m_subtype;
	effect->m_value = m_value;
	effect->m_effect = m_effect;
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
	if (m_value){ m_value->apply_effect(unit, object); }
}

Interaction_addon::Interaction_addon(){};

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
	info->push_back(std::string(level, '.') + "<наложение эффекта:" + Application::instance().m_game_object_manager->get_effect_string(m_subtype) + ">:");
	m_value->description(info, level + 1);
}

void Interaction_addon::apply_effect(GameObject* unit, Object_interaction* object)
{
unit->add_effect(m_subtype, m_value->clone());
}

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
		Interaction_prefix_ex* prefix_ex = static_cast<Interaction_prefix_ex*>(object);
		if (static_cast<Parameter_list*>(prefix_ex->m_value)->m_subtype == interaction_e::health)
		{
			if (static_cast<Parameter_list*>(prefix_ex->m_value)->m_basic_value == 0)
			{
				unit->set_state(object_state_e::dead);
			}
		}
		break;
	}
	}
}

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

Action_list::Action_list(){};

Interaction_list* Action_list::clone()
{
	Action_list* result = new Action_list();
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->m_effect.push_back((*item)->clone());
	}
	return result;
}

ObjectTag::Label::Label(object_tag_e type) :Object_tag(type){};

ObjectTag::Label* ObjectTag::Label::clone()
{
	ObjectTag::Label* effect = new ObjectTag::Label(m_type);
	return effect;
}

void ObjectTag::Label::apply_effect(GameObject* unit, Object_interaction* object){};