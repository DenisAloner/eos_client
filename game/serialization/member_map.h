#ifndef MEMBER_MAP_H
#define MEMBER_MAP_H

#include "game_world.h"
#include "json_helpers.h"
#include <string>
#include <type_traits>

template <typename>
struct dependent_false : std::false_type {
};

class MemberMap {
    template <auto T>
    constexpr static auto not_found()
    {
        static_assert(dependent_false<decltype(T)>::value, "Not found");
    }

public:
    template <auto PointerToMember>
    constexpr static auto get = not_found<PointerToMember>();
};

struct iCustomHandler {
};

template <template <typename, typename> typename Property,
    typename Class,
    typename T,
    typename F>
struct CustomWriter : iCustomHandler {
    typedef std::u16string (F::*custom_function_t)(T);
    typedef const Property<Class, T> PropertyType;

    PropertyType property;

    custom_function_t custom_function;

    constexpr CustomWriter(PropertyType& property,
        custom_function_t custom_function)
        : property(property)
        , custom_function(custom_function)
    {
    }
};

#define PROPERTY(Pointer_to_member, name) template <> \
                                          constexpr auto MemberMap::get<##Pointer_to_member> = JsonProperty(##Pointer_to_member, ##name)

// Game_object_owner
PROPERTY(&Game_object_owner::m_kind, u"kind");
PROPERTY(&Game_object_owner::m_owner, u"owner");
// GameWorld
PROPERTY(&GameWorld::m_maps, u"maps");
PROPERTY(&GameWorld::m_objects, u"objects");
PROPERTY(&GameMap::m_size, u"size");
PROPERTY(&GameObject::m_name, u"name");
PROPERTY(&GameObject::m_direction, u"direction");
PROPERTY(&GameObject::m_state, u"state");
PROPERTY(&GameObject::m_active_state, u"active_state");
PROPERTY(&Attribute_map::m_items, u"items");
PROPERTY(&Object_state::m_state, u"state");
PROPERTY(&Object_state::m_size, u"size");
PROPERTY(&Object_state::m_layer, u"layer");
PROPERTY(&Object_state::m_ai, u"AI");
PROPERTY(&Object_state::m_tile_manager, u"tile_manager");
PROPERTY(&Object_state::m_icon, u"icon");
PROPERTY(&Object_state::m_light, u"light");
PROPERTY(&Object_state::m_optical, u"optical");
PROPERTY(&Interaction_list::m_list_type, u"list_type");
PROPERTY(&Interaction_list::m_items, u"items");
PROPERTY(&MapCell::x, u"x");
PROPERTY(&MapCell::y, u"y");
PROPERTY(&MapCell::z, u"z");
PROPERTY(&MapCell::m_map, u"map");
// InventoryCell
PROPERTY(&InventoryCell::m_item, u"item");
// ObjectPart
PROPERTY(&ObjectPart::m_part_kind, u"part_kind");
PROPERTY(&ObjectPart::m_name, u"name");
PROPERTY(&ObjectPart::m_attributes, u"attributes");
// AI
PROPERTY(&AI::m_ai_type, u"ai_type");
PROPERTY(&AI_enemy::m_path_qualifier, u"path_qualifier");
PROPERTY(&Effect::m_value, u"value");
PROPERTY(&Effect::m_subtype, u"subtype");
PROPERTY(&Instruction_arg_extract::m_value, u"value");
PROPERTY(&Instruction_arg_extract::m_index, u"index");
PROPERTY(&Instruction_check_owner_type::m_value, u"value");
PROPERTY(&Instruction_check_part_type::m_value, u"value");
PROPERTY(&Parameter_list::m_subtype, u"subtype");
PROPERTY(&Parameter_list::m_basic_value, u"basic_value");
PROPERTY(&Parameter_list::m_basic_limit, u"basic_limit");
PROPERTY(&Object_tag::m_type, u"type");
PROPERTY(&ObjectTag::Can_transfer_object::m_value, u"value");
PROPERTY(&Instruction_check_tag::m_value, u"value");
// Interaction_slot
PROPERTY(&Interaction_slot::m_value, u"value");
// Interaction_timer
PROPERTY(&Interaction_time::m_turn, u"turn");
// Interaction_timer
PROPERTY(&Interaction_timer::m_turn, u"turn");
PROPERTY(&Interaction_timer::m_period, u"period");
#endif
