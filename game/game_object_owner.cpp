#include "game_object_owner.h"

Game_object_owner* Game_object_owner::get_owner(const entity_e kind)
{
    if (m_owner) {
        if (m_owner->m_kind == kind) {
            return m_owner;
        }
        return m_owner->get_owner(kind);
    }
    return nullptr;
}

void Game_object_owner::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_owner) {
        m_owner->reset_serialization_index();
    }
}

std::u16string Game_object_owner::serialize_to_json_reference(JsonWriter& value)
{
    return u"";
}

std::u16string Game_object_owner::serialize_to_json_pointer(JsonWriter& value)
{
    return u"";
}

Game_object_owner::Game_object_owner()
    : m_owner(nullptr)
{
}

Game_object_owner* Game_object_owner::get_owner()
{
    if (m_owner) {
        return m_owner->get_owner();
    }
    return this;
}
