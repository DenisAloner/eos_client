#ifndef OBJECT_PART_H
#define OBJECT_PART_H

#include "Definiton.h"
#include "attribute_map.h"
#include "inventory_cell.h"

class ObjectPart : virtual public Object_interaction, public InventoryCell {

public:
    Attribute_map m_attributes;
    body_part_e m_part_kind;
    std::u16string m_name;

    explicit ObjectPart(GameObject* item = nullptr);
    ObjectPart* clone() override;
    void do_predicate(Visitor& helper) override;
    void apply_visitor(Visitor_generic& visitor) override;

    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&ObjectPart::m_kind, u"kind"),
            make_property(&ObjectPart::m_owner, u"owner"),
            make_property(&ObjectPart::m_item, u"item"),
            make_property(&ObjectPart::m_part_kind, u"part_kind"),
            make_property(&ObjectPart::m_name, u"name"),
            make_property(&ObjectPart::m_attributes, u"attributes"));
    }

    interaction_message_type_e get_interaction_message_type() override;
    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};
#endif