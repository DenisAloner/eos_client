#include "interaction_list.h"
#include "instruction_game_owner.h"

Interaction_list::Interaction_list()
{
    m_list_type = feature_list_type_e::generic;
}

bool Interaction_list::update()
{
    return false;
}

Interaction_list* Interaction_list::clone()
{
    auto result = new Interaction_list();
    result->m_list_type = m_list_type;
    for (auto& m_item : m_items) {
        auto c = m_item->clone();
        if (c) {
            result->m_items.push_back(c);
        }
    }
    return result;
}

void Interaction_list::apply_effect(GameObject* unit, Object_interaction* object)
{
    if (object) {
        switch (object->get_interaction_message_type()) {
        case interaction_message_type_e::instruction_game_owner: {
	        const auto i = static_cast<Instruction_game_owner*>(object);
            for (auto& m_item : m_items) {
                i->m_result = false;
                m_item->apply_effect(unit, object);
                if (!i->m_result) {
                    return;
                }
            }
            i->m_result = true;
            break;
        }
        default: {
            for (auto& m_item : m_items)
            {
	            m_item->apply_effect(unit, object);
            }
            break;
        }
        }
    } else {
        for (auto& m_item : m_items)
        {
	        m_item->apply_effect(unit, object);
        }
    }
}

void Interaction_list::add(Object_interaction* item)
{
	m_items.push_back(item);
}

void Interaction_list::remove(Object_interaction* item)
{
	m_items.remove(item);
}

void Interaction_list::equip(Object_interaction* item)
{
	m_items.push_back(item);
}

void Interaction_list::unequip(Object_interaction* item)
{
	m_items.remove(item);
}

void Interaction_list::do_predicate(Visitor& helper)
{
    helper.visit(*this);
    for (auto& m_item : m_items) {
        m_item->do_predicate(helper);
    }
}

void Interaction_list::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

void Interaction_list::reset_serialization_index()
{
    m_serialization_index = 0;
    for (auto& m_item : m_items) {
        if (m_item)
            m_item->reset_serialization_index();
    }
}

iPacker& Interaction_list::get_packer()
{
    return Packer<Interaction_list>::instance();
}

interaction_message_type_e Interaction_list::get_interaction_message_type()
{
    return interaction_message_type_e::list;
}

IJSONSERIALIZABLE_IMPL(Interaction_list);
