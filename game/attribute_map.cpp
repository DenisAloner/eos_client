#include "attribute_map.h"
#include "Action.h"

Interaction_list* Effect_functions::create_feature_list(feature_list_type_e key, interaction_e name)
{
    Interaction_list* result;
    switch (key) {
    case feature_list_type_e::generic: {
        result = new Interaction_list();
        break;
    }
    case feature_list_type_e::tag: {
        result = new Tag_list();
        break;
    }
    case feature_list_type_e::action: {
        result = new Action_list();
        break;
    }
    case feature_list_type_e::parameter: {
        result = new Parameter_list(name);
        break;
    }
    case feature_list_type_e::parts: {
        result = new Parts_list();
        break;
    }
    case feature_list_type_e::vision: {
        result = new Vision_list();
        break;
    }
    case feature_list_type_e::vision_component: {
        result = new Vision_component();
        break;
    }
    }
    return result;
}

AttributeMap::AttributeMap() = default;

Interaction_list* AttributeMap::create_feature_list(feature_list_type_e key, interaction_e name)
{
    const auto result = Effect_functions::create_feature_list(key, name);
    m_items[name] = result;
    return result;
}

void AttributeMap::add_effect(interaction_e key, Object_interaction* item)
{
    if (m_items.find(key) != m_items.end()) {
        m_items[key]->add(item);
    }
}

Interaction_list* AttributeMap::get_list(interaction_e key)
{
    const auto value = m_items.find(key);
    if (value != m_items.end()) {
        return value->second;
    }
    return nullptr;
}

AttributeMap* AttributeMap::clone()
{
    auto result = new AttributeMap();
    for (auto& m_item : m_items) {
        result->m_items[m_item.first] = m_item.second->clone();
    }
    return result;
}

void AttributeMap::reset_serialization_index()
{
    m_serialization_index = 0;
    for (auto m_item : m_items) {
        m_item.second->reset_serialization_index();
    }
}

Tag_getter::Tag_getter(object_tag_e key)
    : m_key(key)
    , m_result(nullptr) {};

void Tag_getter::visit(Object_interaction& value)
{
    if (!m_result) {
        if (value.get_interaction_message_type() == interaction_message_type_e::tag) {
            if (static_cast<Object_tag&>(value).m_type == m_key) {
                m_result = static_cast<Object_tag*>(&value);
            }
        }
    }
}

bool AttributeMap::get_stat(const object_tag_e key)
{
    const auto list = m_items.find(interaction_e::tag);
    if (list != m_items.end()) {
        auto taglist = static_cast<Tag_list*>(list->second);
        Tag_getter tg(key);
        taglist->do_predicate(tg);
        return tg.m_result;
    }
    return false;
}

Object_tag* AttributeMap::get_tag(const object_tag_e key)
{
    const auto list = m_items.find(interaction_e::tag);
    if (list != m_items.end()) {
        auto taglist = static_cast<Tag_list*>(list->second);
        Tag_getter tg(key);
        taglist->do_predicate(tg);
        return tg.m_result;
    }
    return nullptr;
}

void AttributeMap::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

iPacker& AttributeMap::get_packer()
{
    return Packer<AttributeMap>::instance();
}

IVISITABLE_IMPL(AttributeMap);