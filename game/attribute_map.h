#ifndef ATTRIBUTE_MAP_H
#define ATTRIBUTE_MAP_H

#include "Definiton.h"
#include "visitor.h"
#include "interaction_list.h"
#include "object_tag.h"

namespace Effect_functions {
Interaction_list* create_feature_list(feature_list_type_e key, interaction_e name);
}


class AttributeMap : public iSerializable {
public:
    std::map<interaction_e, Interaction_list*> m_items;

    AttributeMap();
    void add_effect(interaction_e key, Object_interaction* item);
    Interaction_list* create_feature_list(feature_list_type_e key, interaction_e name);
    virtual AttributeMap* clone();

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
            make_property(&AttributeMap::m_items, u"item"));
    }

   IVISITABLE();
};

#endif