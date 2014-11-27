#include "Effect.h"


Object_interaction::Object_interaction()
{

}

Interaction_slot::Interaction_slot()
{

}

void Interaction_slot::apply(GameObject* object)
{
	auto reaction = object->get_reaction(m_kind);
	if (reaction)
	{
		reaction->apply(object, this);
	}
}

bool Interaction_slot::on_turn()
{
	return false;
}

std::string Interaction_slot::get_description()
{
	return "slot";
}

Object_interaction* Interaction_slot::clone()
{
	Interaction_slot* effect = new Interaction_slot();
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
	return false;
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


Object_interaction* Interaction_list::clone()
{
	Interaction_list* result = new Interaction_list();
	//result->m_effect.insert(m_effect.begin(), m_effect.end());
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->m_effect.push_back((*item)->clone());
	}
	return result;
}

Interaction_list* Interaction_list::clone_list()
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

std::string Parameter_list::get_description()
{
	std::string result = std::to_string(m_value) + "/" + std::to_string(m_limit)+",";
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		result += (*current)->get_description() + ",";
	}
	return result;
}

void Parameter_list::add(Object_interaction* item)
{
}

Object_interaction* Parameter_list::clone()
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

Interaction_list* Parameter_list::clone_list()
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

Buff::Buff()
{
}

bool Buff::on_turn()
{
	m_duration -= 1;
	if (m_duration < 1)
	{
		return true;
	}
	return false;
}

Object_interaction* Buff::clone()
{
	Buff* effect = new Buff();
	effect->m_kind = m_kind;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value;
	effect->m_duration = m_duration;
	return effect;
}

std::string Buff::get_description()
{
	return "<" + std::to_string(m_value) + " урона от яда, " + std::to_string(m_duration) + " ход(а/ов)>";
}


Buff_chance::Buff_chance()
{
}

Object_interaction* Buff_chance::clone()
{
	Buff_chance* effect = new Buff_chance();
	effect->m_kind = m_kind;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value;
	effect->m_duration = m_duration;
	effect->m_chance = m_chance;
	return effect;
}

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