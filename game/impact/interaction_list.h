#ifndef INTERACTION_LIST_H
#define INTERACTION_LIST_H

#include "Definiton.h"
#include "i_json_serializable.h"

class Interaction_list : public Object_interaction {
public:
    feature_list_type_e m_list_type;
    std::list<Object_interaction*> m_items;

    Interaction_list();
    virtual bool update();
    Interaction_list* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    virtual void add(Object_interaction* item);
    virtual void remove(Object_interaction* item);
    virtual void equip(Object_interaction* item);
    virtual void unequip(Object_interaction* item);

    void do_predicate(Visitor& helper) override;
    void apply_visitor(Visitor_generic& visitor) override;

    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Interaction_list::m_list_type, u"list_type"),
            make_property(&Interaction_list::m_items, u"items"));
    }

    interaction_message_type_e get_interaction_message_type() override;
    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

#endif //INTERACTION_LIST_H
