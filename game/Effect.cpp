#include "Effect.h"

Object_interaction::Object_interaction()
{

}

Interaction_slot::Interaction_slot()
{

}

void Object_interaction::apply_effect(Object_interaction* object)
{
}

void Interaction_copyist::apply(GameObject* object)
{
	auto reaction = object->get_reaction(m_kind);
	if (reaction)
	{
		reaction->apply(object, this);
	}
}

bool Interaction_slot::on_turn()
{
	return m_value->on_turn();
}

std::string Interaction_copyist::get_description()
{
	return "slot";
}

Object_interaction* Interaction_copyist::clone()
{
	Interaction_copyist* effect = new Interaction_copyist();
	effect->m_kind = m_kind;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

Effect::Effect()
{
}

void Effect::apply(GameObject* object)
{
	auto reaction = object->get_reaction(m_kind);
	if (reaction)
	{
		static_cast<Reaction_effect*>(reaction)->m_value = m_value;
		reaction->apply(object, this);
	}
}

bool Effect::on_turn()
{
	return false;
}

Object_interaction* Effect::clone()
{
	Effect* effect = new Effect();
	effect->m_kind = m_kind;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value;
	return effect;
}

std::string Effect::get_description()
{
	return Application::instance().m_game_object_manager->get_effect_subtype_string(m_subtype) + ":" + std::to_string(m_value);
}

void Effect::apply_effect(Object_interaction* object)
{
	switch (m_subtype)
	{
	case effect_e::value:
	{
		Parameter_list* item = static_cast<Parameter_list*>(object);
		item->m_basic_value = item->m_basic_value + m_value;
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
	m_kind = reaction_e::list;
}

void Interaction_list::apply(GameObject* object)
{
	for (auto item = m_effect.begin(); item != m_effect.end();item++)
	{
		(*item)->apply(object);
	}
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

void Parameter_list::update_list(Interaction_list* list)
{
	Effect* item;
	Interaction_list* list_item;
	for (auto current = list->m_effect.begin(); current != list->m_effect.end(); current++)
	{
		switch ((*current)->m_kind)
		{
		case reaction_e::list:
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

void Interaction_list::apply_effect(Object_interaction* object)
{
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->apply_effect(object);
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

void Parameter_list::add(Object_interaction* item)
{
}

Parameter_list* Parameter_list::clone()
{
	Parameter_list* result = new Parameter_list();
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

Reaction::Reaction()
{
}

Reaction_effect::Reaction_effect()
{
}

Reaction* Reaction_effect::clone()
{
	Reaction_effect* result = new Reaction_effect();
	result->handler = handler;
	for (auto sub = m_items.begin(); sub != m_items.end(); sub++)
	{
		for (auto current = sub->second.begin(); current != sub->second.end(); current++)
		{
			result->m_items[sub->first].push_back(*current);
		}
	}
	for (auto current = m_list.begin(); current != m_list.end(); current++)
	{
		result->m_list.push_back(*current);
	}
	return result;
}

void Reaction_effect::apply(GameObject* object, Object_interaction* effect)
{
	auto reaction = m_items.find(static_cast<Effect*>(effect)->m_subtype);
	if (reaction != m_items.end())
	{
		for (auto current = reaction->second.begin(); current != reaction->second.end(); current++)
		{
			(*current)(this, object, effect);
		}
	}
	handler(this, object, effect);
	for (auto current = m_list.begin(); current != m_list.end(); current++)
	{
		(*current)(this, object, effect);
	}
}

//Reaction_parameter::Reaction_parameter()
//{
//}
//
//void Reaction_parameter::apply(GameObject* object, Object_interaction* effect)
//{
//	auto reaction = m_items.find(static_cast<Effect*>(effect)->m_subtype);
//	if (reaction != m_items.end())
//	{
//		for (auto current = reaction->second.begin(); current != reaction->second.end(); current++)
//		{
//			(*current)(this, object, effect);
//		}
//	}
//	handler(this, object, effect);
//	for (auto current = m_list.begin(); current != m_list.end(); current++)
//	{
//		(*current)(this, object, effect);
//	}
//}
//
//Reaction* Reaction_parameter::clone()
//{
//	Reaction_parameter* result = new Reaction_parameter();
//	result->handler = handler;
//	for (auto sub = m_items.begin(); sub != m_items.end(); sub++)
//	{
//		for (auto current = sub->second.begin(); current != sub->second.end(); current++)
//		{
//			result->m_items[sub->first].push_back(*current);
//		}
//	}
//	for (auto current = m_list.begin(); current != m_list.end(); current++)
//	{
//		result->m_list.push_back(*current);
//	}
//	return result;
//}

void Effect::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level,'.')+Application::instance().m_game_object_manager->get_effect_subtype_string(m_subtype) + ":" + std::to_string(m_value));
}

void Interaction_copyist::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<наложение эффекта:" + Application::instance().m_game_object_manager->get_effect_string(m_subtype) + ">:");
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

void Interaction_timer::apply(GameObject* object)
{
	if (m_turn == m_period)
	{
		m_value->apply(object);
	}
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
	effect->m_kind = m_kind;
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

void Interaction_copyist::apply_effect(Object_interaction* object)
{
	m_value->apply_effect(object);
}

void Interaction_timer::apply_effect(Object_interaction* object)
{
	if (m_turn == m_period)
	{
		m_value->apply_effect(object);
	}
}

Interaction_copyist::Interaction_copyist()
{
}

Interaction_time::Interaction_time()
{

}

void Interaction_time::apply(GameObject* object)
{
	if (m_turn != 0)
	{
		m_value->apply(object);
	}
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
	effect->m_kind = m_kind;
	effect->m_turn = m_turn;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_time::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<длительное наложение эффекта:" + std::to_string(m_turn) + ">:");
	m_value->description(info, level + 1);
}

void Interaction_time::apply_effect(Object_interaction* object)
{
	m_value->apply_effect(object);
}

Effect_reaction::Effect_reaction()
{
}

Effect_reaction* Effect_reaction::clone()
{
	Effect_reaction* result = new Effect_reaction();
	result->handler = handler;
	for (auto sub = m_items.begin(); sub != m_items.end(); sub++)
	{
		for (auto current = sub->second.begin(); current != sub->second.end(); current++)
		{
			result->m_items[sub->first].push_back(*current);
		}
	}
	return result;
}

void Effect_reaction::apply(GameObject* object, Object_interaction* effect)
{
	auto reaction = m_items.find(static_cast<Effect*>(effect)->m_subtype);
	if (reaction != m_items.end())
	{
		for (auto current = reaction->second.begin(); current != reaction->second.end(); current++)
		{
			(*current)(this, object, effect);
		}
	}
	handler(this, object, effect);
}