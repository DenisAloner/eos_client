#include "object_part.h"
#include "GameObject.h"

ObjectPart::ObjectPart(GameObject* item)
    : InventoryCell(item)
{
    m_kind = entity_e::body_part;
    m_part_kind = body_part_e::container;
    m_owner = nullptr;
    m_attributes.create_feature_list(feature_list_type_e::parts, interaction_e::body);
};

ObjectPart* ObjectPart::clone()
{
    auto result = new ObjectPart();
    result->m_kind = m_kind;
    result->m_name = m_name;
    result->m_part_kind = m_part_kind;
    result->m_item = nullptr;
    result->m_attributes = *m_attributes.clone();
    return result;
}

void ObjectPart::do_predicate(Visitor& helper)
{
    helper.visit(*this);
    for (auto& m_item : m_attributes.m_items) {
        m_item.second->do_predicate(helper);
    }
}

void ObjectPart::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

void ObjectPart::reset_serialization_index()
{
    m_serialization_index = 0;
    m_attributes.reset_serialization_index();
    if (m_item) {
        // ??? исправить, временная заглушка, необходимо добавить свойство, которое показывает был ли сброшен объект для сохранения уже
        if (m_item->m_serialization_index != 0) {
            m_item->reset_serialization_index();
        }
    }
}

iPacker& ObjectPart::get_packer()
{
    return Packer<ObjectPart>::instance();
}

interaction_message_type_e ObjectPart::get_interaction_message_type()
{
    return interaction_message_type_e::part;
}

IVISITABLE_IMPL(ObjectPart);