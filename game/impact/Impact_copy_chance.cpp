#include "Impact_copy_chance.h"

Impact_copy_chance::Impact_copy_chance()
{
}

Impact_copy_chance::~Impact_copy_chance()
{
}

Object_interaction* Impact_copy_chance::clone()
{
    if (rand() % 100 < m_chance) {
        return m_value->clone();
    }
    return nullptr;
}

iPacker& Impact_copy_chance::get_packer()
{
    return Packer<Impact_copy_chance>::instance();
}

interaction_message_type_e Impact_copy_chance::get_interaction_message_type()
{
    return interaction_message_type_e::impact_copy_chance;
}
