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

    template <typename Type>
    constexpr static const char16_t* type = u"undefined"; //not_found<Type>();
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

template <template <typename, typename> typename Property,
    typename Class,
    typename T,
    typename F,
    typename MemberType = std::remove_reference_t<T>>
struct CustomReader : iCustomHandler {
    typedef void (F::*custom_function_t)(T, const std::u16string_view&);
    typedef const Property<Class, MemberType> PropertyType;

    PropertyType property;

    custom_function_t custom_function;

    constexpr CustomReader(PropertyType& property,
        custom_function_t custom_function)
        : property(property)
        , custom_function(custom_function)
    {
    }
};

#define TYPENAME(Type, name) template <> \
                             constexpr const char16_t* MemberMap::type<Type> = name

#define PROPERTY(Pointer_to_member, name) template <> \
                                          constexpr auto MemberMap::get<Pointer_to_member> = JsonProperty(Pointer_to_member, name)

// Game_object_owner
PROPERTY(&Game_object_owner::m_kind, u"kind");
PROPERTY(&Game_object_owner::m_owner, u"owner");
// GameWorld
TYPENAME(GameWorld, u"game_world");
PROPERTY(&GameWorld::m_maps, u"maps");
PROPERTY(&GameWorld::m_objects, u"objects");
// GameMap
TYPENAME(GameMap, u"game_map");
PROPERTY(&GameMap::m_size, u"size");
// GameObject
TYPENAME(GameObject, u"game_object");
PROPERTY(&GameObject::m_name, u"name");
PROPERTY(&GameObject::m_direction, u"direction");
PROPERTY(&GameObject::m_state, u"state");
PROPERTY(&GameObject::m_active_state, u"active_state");
// Attribute_map
TYPENAME(Attribute_map, u"attribute_map");
PROPERTY(&Attribute_map::m_items, u"items");
// Object_state
TYPENAME(Object_state, u"object_state");
PROPERTY(&Object_state::m_state, u"state");
PROPERTY(&Object_state::m_size, u"size");
PROPERTY(&Object_state::m_layer, u"layer");
PROPERTY(&Object_state::m_ai, u"AI");
PROPERTY(&Object_state::m_tile_manager, u"tile_manager");
PROPERTY(&Object_state::m_icon, u"icon");
PROPERTY(&Object_state::m_light, u"light");
PROPERTY(&Object_state::m_optical, u"optical");
// Interaction_list
TYPENAME(Interaction_list, u"interaction_list");
PROPERTY(&Interaction_list::m_list_type, u"list_type");
PROPERTY(&Interaction_list::m_items, u"items");
// MapCell
TYPENAME(MapCell, u"map_cell");
PROPERTY(&MapCell::x, u"x");
PROPERTY(&MapCell::y, u"y");
PROPERTY(&MapCell::z, u"z");
PROPERTY(&MapCell::m_map, u"map");
// InventoryCell
TYPENAME(InventoryCell, u"inventory_cell");
PROPERTY(&InventoryCell::m_item, u"item");
// ObjectPart
TYPENAME(ObjectPart, u"object_part");
PROPERTY(&ObjectPart::m_part_kind, u"part_kind");
PROPERTY(&ObjectPart::m_name, u"name");
PROPERTY(&ObjectPart::m_attributes, u"attributes");
// AI
PROPERTY(&AI::m_ai_type, u"ai_type");
PROPERTY(&AI_enemy::m_path_qualifier, u"path_qualifier");
// Effect
TYPENAME(Effect, u"effect");
PROPERTY(&Effect::m_value, u"value");
PROPERTY(&Effect::m_subtype, u"subtype");
// Instruction_arg_extract
TYPENAME(Instruction_arg_extract, u"instruction_arg_extract");
PROPERTY(&Instruction_arg_extract::m_value, u"value");
PROPERTY(&Instruction_arg_extract::m_index, u"index");
// Instruction_check_owner_type
TYPENAME(Instruction_check_owner_type, u"instruction_check_owner_type");
PROPERTY(&Instruction_check_owner_type::m_value, u"value");
PROPERTY(&Instruction_check_part_type::m_value, u"value");
// Parameter_list
TYPENAME(Parameter_list, u"parameter_list");
PROPERTY(&Parameter_list::m_subtype, u"subtype");
PROPERTY(&Parameter_list::m_basic_value, u"basic_value");
PROPERTY(&Parameter_list::m_basic_limit, u"basic_limit");
// Parameter_list
TYPENAME(Action_list, u"action_list");
// Object_tag
PROPERTY(&Object_tag::m_type, u"type");
// ObjectTag::Can_transfer_object
TYPENAME(ObjectTag::Can_transfer_object, u"object_tag_can_transfer_object");
PROPERTY(&ObjectTag::Can_transfer_object::m_value, u"value");
// ObjectTag::Equippable
TYPENAME(ObjectTag::Equippable, u"object_tag_equippable");
PROPERTY(&ObjectTag::Equippable::m_value, u"value");
PROPERTY(&ObjectTag::Equippable::m_condition, u"condition");
// ObjectTag::Requirements_to_object
TYPENAME(ObjectTag::Requirements_to_object, u"object_tag_requirements_to_object");
PROPERTY(&ObjectTag::Requirements_to_object::m_value, u"value");
// Instruction_check_tag
TYPENAME(Instruction_check_tag, u"instruction_check_tag");
PROPERTY(&Instruction_check_tag::m_value, u"value");
// Interaction_slot
PROPERTY(&Interaction_slot::m_value, u"value");
// Interaction_timer
PROPERTY(&Interaction_time::m_turn, u"turn");
// Interaction_timer
PROPERTY(&Interaction_timer::m_turn, u"turn");
PROPERTY(&Interaction_timer::m_period, u"period");
// Instruction_slot_link
PROPERTY(&Instruction_slot_link::m_subtype, u"subtype");
// Instruction_get_owner
PROPERTY(&Instruction_get_owner::m_value, u"value");
// GraphicalController
PROPERTY(&GraphicalController::atlas_tiles, u"tiles");
PROPERTY(&GraphicalController::gui_styles, u"styles");
// gui_style_t
PROPERTY(&gui_style_t::background_tile, u"background");
PROPERTY(&gui_style_t::border_x_tile, u"border_x");
PROPERTY(&gui_style_t::border_y_tile, u"border_y");
PROPERTY(&gui_style_t::corner_tile, u"corner");
PROPERTY(&gui_style_t::scroll_y_head_tile, u"scroll_y_head");
PROPERTY(&gui_style_t::scroll_y_body_tile, u"scroll_y_body");

#endif
