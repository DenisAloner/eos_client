#ifndef EFFECT_H
#define EFFECT_H

#include "Parameter.h"
#include <functional>
#include "interaction_list.h"
#include "parameter_list.h"
#include "vision_list.h"
#include "vision_component.h"
#include "tag_list.h"
#include "parts_list.h"
#include "action_list.h"
#include "interaction_slot.h"
#include "slot_set_state.h"
#include "interaction_copyist.h"
#include "interaction_prefix.h"
#include "interaction_addon.h"
#include "interaction_time.h"
#include "interaction_timer.h"
#include "object_tag.h"

class Game_object_owner;

class Effect : public Object_interaction {
public:
    int m_value;
    effect_e m_subtype;
    Effect();
    Effect* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Effect::m_value, u"value"),
            make_property(&Effect::m_subtype, u"subtype"));
    }

    interaction_message_type_e get_interaction_message_type() override;
};

namespace ObjectTag {
class Poison_resist : public Object_tag {
public:
    Poison_resist();
    Poison_resist* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
};

class Mortal : public Object_tag {
public:
    Mortal();
    Mortal* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    void apply_visitor(Visitor_generic& visitor) override;

    // Для поддержки iSerializable
    iPacker& get_packer() override;

    /*constexpr static auto properties() { return Object_tag::properties(); }*/
};

class Purification_from_poison : public Object_tag {
public:
    Purification_from_poison();
    virtual Purification_from_poison* clone();
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
};

class Activator : public Object_tag {
public:
    Activator();
    std::list<GameObject*> m_link;
    Activator* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    void reset_serialization_index() override;

    iPacker& get_packer() override;
};

class Fast_move : public Object_tag {
public:
    Fast_move();
    Fast_move* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
};

class Label : public Object_tag {
public:
    Label()
        : Object_tag(object_tag_e::none) {};
    Label(object_tag_e type);
    Label* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    void apply_visitor(Visitor_generic& visitor) override;

    // Для поддержки iSerializable
    iPacker& get_packer() override;

    constexpr static auto properties() { return Object_tag::properties(); }
};

class Equippable : public Object_tag {
public:
    Object_interaction* m_value;
    Interaction_list* m_condition;

    Equippable();
    Equippable* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    void reset_serialization_index() override;

    // Для поддержки iSerializable
    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Object_tag::properties(),
            std::make_tuple(
                make_property(&Equippable::m_value, u"value"),
                make_property(&Equippable::m_condition, u"condition")));
    }
};

class Requirements_to_object : public Object_tag {
public:
    Object_interaction* m_value;
    bool m_result;

    Requirements_to_object();
    virtual Requirements_to_object* clone();
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    void apply_visitor(Visitor_generic& visitor) override;

    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Object_tag::properties(),
            std::make_tuple(
                make_property(&Requirements_to_object::m_value, u"value")));
    }
};

class Can_transfer_object : public Object_tag {
public:
    Object_interaction* m_value;
    bool m_result;

    Can_transfer_object();
    virtual Can_transfer_object* clone();
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Object_tag::properties(),
            std::make_tuple(
                make_property(&Can_transfer_object::m_value, u"value")));
    }
};
}

namespace Effect_functions {
Interaction_list* create_feature_list(feature_list_type_e key, interaction_e name);
}

class Instruction_slot_link : public Interaction_slot {
public:
    interaction_e m_subtype;
    bool m_enable;
    Instruction_slot_link();
    Instruction_slot_link* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Interaction_slot::properties(),
            std::make_tuple(
                make_property(&Instruction_slot_link::m_subtype, u"subtype")));
    }

    interaction_message_type_e get_interaction_message_type() override;
};

class Instruction_slot_parameter : public Interaction_slot {
public:
    Parameter* m_parameter;
    bool m_result;

    Instruction_slot_parameter();
    Object_interaction* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override;
};

class Instruction_game_owner : public Object_interaction {
public:
    Game_object_owner* m_value;
    bool m_result;

    Instruction_game_owner();
    Object_interaction* clone() override { return nullptr; };
    virtual void apply_effect(GameObject* unit, Object_interaction* object) {};

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override { return interaction_message_type_e::instruction_game_owner; }
};

class Instruction_check_tag : public Object_interaction {
public:
    object_tag_e m_value;

    Instruction_check_tag();
    Instruction_check_tag* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    void apply_visitor(Visitor_generic& visitor) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Instruction_check_tag::m_value, u"value"));
    }

    interaction_message_type_e get_interaction_message_type() override;
};

class Instruction_check_part_type : public Object_interaction {
public:
    body_part_e m_value;

    Instruction_check_part_type();
    Instruction_check_part_type* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Instruction_check_part_type::m_value, u"value"));
    }

    interaction_message_type_e get_interaction_message_type() override;
};

class Instruction_arg_extract : public Object_interaction {
public:
    Object_interaction* m_value;
    std::size_t m_index;
    bool m_result;

    Instruction_arg_extract();
    Instruction_arg_extract* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Instruction_arg_extract::m_value, u"value"),
            make_property(&Instruction_arg_extract::m_index, u"index"));
    }

    interaction_message_type_e get_interaction_message_type() override;
};

class Instruction_get_owner : public Object_interaction {
public:
    Object_interaction* m_value;
    bool m_result;

    Instruction_get_owner();
    Instruction_get_owner* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Instruction_get_owner::m_value, u"value"));
    }

    interaction_message_type_e get_interaction_message_type() override;
};

class Instruction_get_owner_top : public Instruction_get_owner {
public:
    Instruction_get_owner_top();
    Instruction_get_owner_top* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties() { return Instruction_get_owner::properties(); }
};

class Instruction_check_owner_type : public Object_interaction {
public:
    entity_e m_value;

    Instruction_check_owner_type();
    Instruction_check_owner_type* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Instruction_check_owner_type::m_value, u"value"));
    }

    interaction_message_type_e get_interaction_message_type() override { return interaction_message_type_e::instruction_check_owner_type; };
};

#endif //EFFECT_H