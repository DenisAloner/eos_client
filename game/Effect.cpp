#include "Effect.h"

Parameter_modification::Parameter_modification()
{
}

//void Parameter_modification::apply(GameObject* object)
//{
//	auto reaction = object->get_reaction(m_kind);
//	if (reaction)
//	{
//		reaction->apply(m_value, object, this);
//	}
//}

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

Effect* Effect::clone()
{
	return nullptr;
}

std::string Effect::get_description()
{
	return Application::instance().m_game_object_manager->get_effect_subtype_string(m_subtype) + ":" + std::to_string(m_value);
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
		result += (*current)->get_description()+",";
	}
	return result;
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

Effect* Buff::clone()
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

Effect* Buff_chance::clone()
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