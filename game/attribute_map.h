#ifndef ATTRIBUTE_MAP_H
#define ATTRIBUTE_MAP_H

#include "Definiton.h"
#include "i_json_serializable.h"
#include "interaction_list.h"
#include "object_tag.h"

namespace Effect_functions {
Interaction_list* create_feature_list(feature_list_type_e key, interaction_e name);
}


class Attribute_map : public iSerializable {
public:
    std::map<interaction_e, Interaction_list*> m_items;

    Attribute_map();
    void add_effect(interaction_e key, Object_interaction* item);
    Interaction_list* create_feature_list(feature_list_type_e key, interaction_e name);
    virtual Attribute_map* clone();

    void reset_serialization_index() override;

    Interaction_list* get_list(interaction_e key);
    bool get_stat(object_tag_e key);
    Object_tag* get_tag(object_tag_e key);
    virtual void apply_visitor(Visitor_generic& visitor);

    // Для поддержки iSerializable
    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Attribute_map::m_items, u"item"));
    }

    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

#endif