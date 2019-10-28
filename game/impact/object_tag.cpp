#include "object_tag.h"

Object_tag::Object_tag(object_tag_e key)
    : m_type(key)
{
}

interaction_message_type_e Object_tag::get_interaction_message_type()
{
    return interaction_message_type_e::tag;
};
