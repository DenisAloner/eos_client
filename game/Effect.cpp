#include "Effect.h"


Effect::Effect()
{
}

Effect_int::Effect_int()
{
}

Effect_damage::Effect_damage()
{
}

void Effect_damage::apply(GameObject* object, Effect* effect)
{
	auto reaction = object->m_reaction.find(m_kind);
	if (reaction != object->m_reaction.end())
	{
		reaction->second->call_items(m_value, object, effect);
	}
}

void Reaction::call_items(int value, GameObject* object, Effect* effect)
{
	m_value = value;
	for (auto current = m_items.begin(); current != m_items.end(); current++)
	{
		(*current)(this, object, effect);
	}
}

void Reaction_manager::check_health(Reaction* reaction,GameObject* object, Effect* effect)
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

void Reaction_manager::get_physical_damage(Reaction* reaction, GameObject* object, Effect* effect)
{
	Effect_damage* damage = static_cast<Effect_damage*>(effect);
	Game_object_feature* prop = static_cast<Game_object_feature*>(object->m_active_state->find_property(property_e::health));
	if (prop)
	{
		prop->m_value -= reaction->m_value;
	}
	auto r = object->m_reaction.find(reaction_e::get_damage);
	if (r != object->m_reaction.end())
	{
		r->second->call_items(static_cast<Effect_int*>(effect)->m_value, object, effect);
	}
}

Reaction_manager::Reaction_manager()
{
	m_items.insert(std::pair<reaction_applicator_e, Reaction::func>(reaction_applicator_e::check_health, std::bind(&Reaction_manager::check_health, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
	m_items.insert(std::pair<reaction_applicator_e, Reaction::func>(reaction_applicator_e::get_physical_damage, std::bind(&Reaction_manager::get_physical_damage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
}
