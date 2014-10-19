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

bool Effect::on_turn()
{
	return false;
}

Effect* Effect::clone()
{
	return nullptr;
}

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

void Reaction::apply(int value, GameObject* object, Effect* effect)
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
	for (auto current = m_list.begin(); current != m_list.end(); current++)
	{
		(*current)(this, object, effect);
	}
}

Reaction* Reaction::clone()
{
	Reaction* result = new Reaction();
	result->m_applicator = m_applicator;
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