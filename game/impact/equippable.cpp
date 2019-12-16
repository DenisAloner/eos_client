#include "equippable.h"
#include "GameObject.h"

ObjectTag::Equippable::Equippable()
    : Object_tag(object_tag_e::equippable)
{
    m_value = nullptr;
    m_condition = nullptr;
};

ObjectTag::Equippable* ObjectTag::Equippable::clone()
{
    auto result = new ObjectTag::Equippable;
    if (m_value) {
        result->m_value = m_value->clone();
    }
    result->m_condition = m_condition->clone();
    return result;
}

void ObjectTag::Equippable::apply_effect(GameObject* unit, Object_interaction* object)
{
    switch (object->get_interaction_message_type()) {
    case interaction_message_type_e::instruction_slot_parameter: {
	    const auto parameter = static_cast<Instruction_slot_parameter*>(object);
        auto& p(*parameter->m_parameter);

        switch (p[2].m_owner->m_kind) {
        case entity_e::cell: {
            break;
        }
        case entity_e::inventory_cell: {
            break;
        }
        case entity_e::body_part: {
	        const auto i = new Instruction_game_owner();
            i->m_value = p[2].m_owner;
            m_condition->apply_effect(unit, i);
            if (!i->m_result) {
                return;
            };
            if (p[2].m_owner->m_kind == entity_e::body_part) {
	            auto part = static_cast<ObjectPart*>(p[2].m_owner);
	            auto t = part->m_attributes.get_tag(object_tag_e::requirements_to_object);
                if (t) {
                    i->m_result = false;
                    i->m_value = p[1].m_object;
                    t->apply_effect(unit, i);
                    if (!i->m_result) {
                        return;
                    };
                }
            }
            break;
        }
        }

        switch (p[1].m_object->m_owner->m_kind) {
        case entity_e::cell: {
            //static_cast<MapCell*>(p->m_object->m_owner)->m_items.remove(p->m_object);
            static_cast<MapCell*>(p[1].m_object->m_owner)->m_map->remove_object(p[1].m_object);
            break;
        }
        case entity_e::inventory_cell: {
            static_cast<InventoryCell*>(p[1].m_object->m_owner)->m_item = nullptr;
            break;
        }
        case entity_e::body_part: {
	        auto part = static_cast<ObjectPart*>(p[1].m_object->m_owner);
            part->m_item = nullptr;
            part = nullptr;
            m_value->apply_effect(unit, object);
            break;
        }
        }

        switch (p[2].m_owner->m_kind) {
        case entity_e::cell: {
            //static_cast<MapCell*>(p->m_owner)->m_items.push_back(p->m_object);
            static_cast<MapCell*>(p[2].m_owner)->m_map->add_object(p[1].m_object, *static_cast<MapCell*>(p[2].m_owner));
            p[1].m_object->m_owner = p[2].m_owner;
            break;
        }
        case entity_e::inventory_cell: {
            static_cast<InventoryCell*>(p[2].m_owner)->m_item = p[1].m_object;
            p[1].m_object->m_owner = p[2].m_owner;
            break;
        }
        case entity_e::body_part: {
	        const auto part = static_cast<ObjectPart*>(p[2].m_owner);
            part->m_item = p[1].m_object;
            p[1].m_object->m_owner = part;
            if (m_value) {
                m_value->apply_effect(unit, object);
            }
            break;
        }
        }
        break;
    }
    }
};

void ObjectTag::Equippable::reset_serialization_index()
{

    m_serialization_index = 0;
    if (m_value && m_value->m_serialization_index != 0) {
        m_value->reset_serialization_index();
    }
    if (m_condition && m_condition->m_serialization_index != 0) {
        m_condition->reset_serialization_index();
    }
}

iPacker& ObjectTag::Equippable::get_packer()
{
    return Packer<Equippable>::instance();
}

IVISITABLE_IMPL(ObjectTag::Equippable);
