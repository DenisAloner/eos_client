#include "Impact_random_value.h"

Impact_random_value::Impact_random_value()
{
}

Impact_random_value::~Impact_random_value()
{
}

Effect* Impact_random_value::clone()
{
	auto effect = new Effect();
    effect->m_interaction_message_type = m_interaction_message_type;
    effect->m_subtype = m_subtype;
    effect->m_value = rand() % (m_value - m_min_value) + m_min_value;
    return effect;
}