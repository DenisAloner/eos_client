#include "Effect.h"


Effect::Effect()
{
}

void Effect::apply(GameObject* object, Effect* effect)
{
	auto reaction = object->m_reaction.find(m_kind);
	if (reaction != object->m_reaction.end())
	{
		reaction->second->apply(m_value, object, effect);
	}
}

void Reaction_mods::apply(int value, GameObject* object, Effect* effect)
{
	m_value = value;
	m_applicator(this, object, effect);
	for (auto current = m_list.begin(); current != m_list.end(); current++)
	{
		(*current)(this, object, effect);
	}
}

Reaction* Reaction_mods::clone()
{
	Reaction_mods* result = new Reaction_mods();
	result->m_applicator = m_applicator;
	for (auto current = m_list.begin(); current != m_list.end(); current++)
	{
		result->m_list.push_back(*current);
	}
	return result;
}

void Reaction_subtype::apply(int value, GameObject* object, Effect* effect)
{
	m_value = value;
	auto reaction = m_items.find(effect->m_subtype);
	if (reaction != m_items.end())
	{
		for (auto current = reaction->second.begin(); current != reaction->second.end(); current++)
		{
			(*current)(this, object, effect);
		}
	}
	m_applicator(this, object, effect);
}

Reaction* Reaction_subtype::clone()
{
	Reaction_subtype* result = new Reaction_subtype();
	result->m_applicator = m_applicator;
	for (auto sub = m_items.begin(); sub != m_items.end(); sub++)
	{
		for (auto current = sub->second.begin(); current != sub->second.end(); current++)
		{
			result->m_items[sub->first].push_back(*current);
		}
	}
	return result;
}

void Reaction_manager::change_health(Reaction* reaction,GameObject* object, Effect* effect)
{
	Game_object_feature* prop = static_cast<Game_object_feature*>(object->m_active_state->find_property(property_e::health));
	if (prop)
	{
		if (prop->m_value < 1)
		{
			Application::instance().m_GUI->MapViewer->m_map->m_ai.remove(object);
			object->set_state(state_e::dead);
		}
	}
}

void Reaction_manager::get_damage(Reaction* reaction, GameObject* object, Effect* effect)
{
	Game_object_feature* prop = static_cast<Game_object_feature*>(object->m_active_state->find_property(property_e::health));
	if (prop)
	{
		prop->m_value -= reaction->m_value;
	}
	auto r = object->m_reaction.find(reaction_e::change_health);
	if (r != object->m_reaction.end())
	{
		r->second->apply(effect->m_value, object, effect);
	}
}

Reaction_manager::Reaction_manager()
{
	m_items.insert(std::pair<reaction_applicator_e, Reaction::func>(reaction_applicator_e::change_health, std::bind(&Reaction_manager::change_health, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
	m_items.insert(std::pair<reaction_applicator_e, Reaction::func>(reaction_applicator_e::get_damage, std::bind(&Reaction_manager::get_damage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
}
