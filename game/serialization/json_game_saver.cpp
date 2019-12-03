#include "json_game_saver.h"

#define VISIT_IMPL(Visitor, Class)                                              \
    std::u16string Visitor::write(Class& value) { return write_object(value); } \
    std::u16string Visitor::write(Class* value) { return write_pointer(value); }

template <typename T>
constexpr auto JsonGameSaver::object_properties()
{
    if constexpr (std::is_same_v<T, GameWorld>) {
        return std::tuple(
            MemberMap::get<&GameWorld::m_maps>,
            MemberMap::get<&GameWorld::m_objects>);
    }
    if constexpr (std::is_same_v<T, GameMap>) {
        return std::tuple(
            MemberMap::get<&GameMap::m_size>);
    }
    if constexpr (std::is_same_v<T, GameObject>) {
        return std::tuple(
            MemberMap::get<&GameObject::m_name>,
            MemberMap::get<&GameObject::m_direction>,
            MemberMap::get<&Game_object_owner::m_owner>,
            MemberMap::get<&GameObject::m_state>,
            CustomWriter(MemberMap::get<&GameObject::m_active_state>, &JsonGameSaver::active_object_state_save));
    }
    if constexpr (std::is_same_v<T, Attribute_map>) {
        return std::tuple(
            MemberMap::get<&Attribute_map::m_items>);
    }
    if constexpr (std::is_same_v<T, Object_state>) {
        return std::tuple_cat(object_properties<Attribute_map>(),
            std::make_tuple(
                MemberMap::get<&Object_state::m_state>,
                MemberMap::get<&Object_state::m_size>,
                MemberMap::get<&Object_state::m_layer>,
                MemberMap::get<&Object_state::m_ai>,
                MemberMap::get<&Object_state::m_tile_manager>,
                MemberMap::get<&Object_state::m_icon>,
                MemberMap::get<&Object_state::m_light>,
                MemberMap::get<&Object_state::m_optical>));
    }
    if constexpr (std::is_same_v<T, Interaction_list>) {
        return std::tuple(
            MemberMap::get<&Interaction_list::m_list_type>,
            MemberMap::get<&Interaction_list::m_items>);
    }
    if constexpr (std::is_same_v<T, Parameter_list>) {
        return std::tuple_cat(object_properties<Interaction_list>(),
            std::make_tuple(
                MemberMap::get<&Parameter_list::m_subtype>,
                MemberMap::get<&Parameter_list::m_basic_value>,
                MemberMap::get<&Parameter_list::m_basic_limit>));
    }
    if constexpr (std::is_same_v<T, MapCell>) {
        return std::tuple(
            MemberMap::get<&MapCell::x>,
            MemberMap::get<&MapCell::y>,
            MemberMap::get<&MapCell::z>,
            CustomWriter(MemberMap::get<&MapCell::m_map>, &JsonGameSaver::map_cell_owner_save));
    }
    if constexpr (std::is_same_v<T, Game_object_owner>) {
        return std::tuple(
            MemberMap::get<&Game_object_owner::m_kind>,
            MemberMap::get<&Game_object_owner::m_owner>);
    }
    if constexpr (std::is_same_v<T, InventoryCell>) {
        return std::tuple_cat(object_properties<Game_object_owner>(),
            std::make_tuple(
                MemberMap::get<&InventoryCell::m_item>));
    }
    if constexpr (std::is_same_v<T, ObjectPart>) {
        return std::tuple_cat(object_properties<InventoryCell>(),
            std::make_tuple(
                MemberMap::get<&ObjectPart::m_part_kind>,
                MemberMap::get<&ObjectPart::m_name>,
                MemberMap::get<&ObjectPart::m_attributes>));
    }
    if constexpr (std::is_same_v<T, AI>) {
        return std::tuple(
            MemberMap::get<&AI::m_ai_type>);
    }
    if constexpr (std::is_same_v<T, AI_enemy>) {
        return std::tuple_cat(object_properties<AI>(),
            std::make_tuple(
                MemberMap::get<&AI_enemy::m_path_qualifier>));
    }
    if constexpr (std::is_same_v<T, Effect>) {
        return std::tuple(
            MemberMap::get<&Effect::m_value>,
            MemberMap::get<&Effect::m_subtype>);
    }
    if constexpr (std::is_same_v<T, Instruction_arg_extract>) {
        return std::tuple(
            MemberMap::get<&Instruction_arg_extract::m_value>,
            MemberMap::get<&Instruction_arg_extract::m_index>);
    }
    if constexpr (std::is_same_v<T, Instruction_check_owner_type>) {
        return std::tuple(
            MemberMap::get<&Instruction_check_owner_type::m_value>);
    }
    if constexpr (std::is_same_v<T, Instruction_check_part_type>) {
        return std::tuple(
            MemberMap::get<&Instruction_check_part_type::m_value>);
    }
    if constexpr (std::is_same_v<T, Instruction_result>) {
        return std::tuple();
    }
    if constexpr (std::is_same_v<T, Object_tag>) {
        return std::tuple(
            MemberMap::get<&Object_tag::m_type>);
    }
    if constexpr (std::is_same_v<T, ObjectTag::Can_transfer_object>) {
        return std::tuple_cat(object_properties<Object_tag>(),
            std::make_tuple(
                MemberMap::get<&ObjectTag::Can_transfer_object::m_value>));
    }
    if constexpr (std::is_same_v<T, Instruction_check_tag>) {
        return std::tuple(
            MemberMap::get<&Instruction_check_tag::m_value>);
    }
    if constexpr (std::is_same_v<T, ObjectTag::Label>) {
        return object_properties<Object_tag>();
    }
    if constexpr (std::is_same_v<T, Interaction_slot>) {
        return std::tuple(
            MemberMap::get<&Interaction_slot::m_value>);
    }
    if constexpr (std::is_same_v<T, Interaction_time>) {
        return std::tuple_cat(object_properties<Interaction_slot>(),
            std::make_tuple(
                MemberMap::get<&Interaction_time::m_turn>));
    }
    if constexpr (std::is_same_v<T, Interaction_timer>) {
        return std::tuple_cat(object_properties<Interaction_slot>(),
            std::make_tuple(
                MemberMap::get<&Interaction_timer::m_turn>,
                MemberMap::get<&Interaction_timer::m_period>));
    }
}

VISIT_IMPL(JsonGameSaver, GameWorld);
VISIT_IMPL(JsonGameSaver, GameMap);
VISIT_IMPL(JsonGameSaver, GameObject);
VISIT_IMPL(JsonGameSaver, Attribute_map);
VISIT_IMPL(JsonGameSaver, Object_state);
VISIT_IMPL(JsonGameSaver, Interaction_list);
VISIT_IMPL(JsonGameSaver, InventoryCell);
VISIT_IMPL(JsonGameSaver, ObjectPart);

std::u16string JsonGameSaver::write(MapCell& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(MapCell* value)
{
    return value ? write(*value) : u"null";
}


std::u16string JsonGameSaver::write(Action& value)
{
    return write(value.m_kind);
}

std::u16string JsonGameSaver::write(Action* value)
{
    return value ? write(*value) : u"null";
}

std::u16string JsonGameSaver::write(predicate_t& value)
{
    return write(value.index);
}

std::u16string JsonGameSaver::write(predicate_t* value)
{
    return value ? write(*value) : u"null";
}

std::u16string JsonGameSaver::write(TileManager& value)
{
    return u"\"" + cp1251_to_utf16(value.m_json_string) + u"\"";
}

std::u16string JsonGameSaver::write(TileManager* value)
{
    return value ? write(*value) : u"null";
}

std::u16string JsonGameSaver::write(Icon& value)
{
    return u"\"" + cp1251_to_utf16(value.m_json_string) + u"\"";
}

std::u16string JsonGameSaver::write(Icon* value)
{
    return value ? write(*value) : u"null";
}

VISIT_IMPL(JsonGameSaver, AI_enemy);
VISIT_IMPL(JsonGameSaver, Effect);
VISIT_IMPL(JsonGameSaver, Instruction_arg_extract);
VISIT_IMPL(JsonGameSaver, Instruction_check_owner_type);
VISIT_IMPL(JsonGameSaver, Instruction_check_part_type);
VISIT_IMPL(JsonGameSaver, Instruction_result);
VISIT_IMPL(JsonGameSaver, Parameter_list);
VISIT_IMPL(JsonGameSaver, ObjectTag::Can_transfer_object);
VISIT_IMPL(JsonGameSaver, Instruction_check_tag);
VISIT_IMPL(JsonGameSaver, ObjectTag::Label);
VISIT_IMPL(JsonGameSaver, Interaction_time);
VISIT_IMPL(JsonGameSaver, Interaction_timer);

std::u16string JsonGameSaver::write(Instruction_game_owner& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Instruction_game_owner* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Instruction_get_owner& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Instruction_get_owner* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Instruction_slot_link& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Instruction_slot_link* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Interaction_addon& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Interaction_addon* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Interaction_copyist& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Interaction_copyist* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Interaction_prefix& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Interaction_prefix* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Activator& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Activator* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Equippable& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Equippable* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Fast_move& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Fast_move* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Mortal& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Mortal* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Purification_from_poison& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Purification_from_poison* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Requirements_to_object& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Requirements_to_object* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Slot_set_state& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Slot_set_state* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Parameter& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Parameter* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Config& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Config* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Poison_resist& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(ObjectTag::Poison_resist* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(AI_trap& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(AI_trap* value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Instruction_slot_parameter& value)
{
    return u"";
}

std::u16string JsonGameSaver::write(Instruction_slot_parameter* value)
{
    return u"";
}

std::u16string JsonGameSaver::map_cell_owner_save(GameMap* value)
{
    return value ? write(value->m_index) : u"null";
}

std::u16string JsonGameSaver::active_object_state_save(Object_state* value)
{
    return value ? write(value->m_state) : u"null";
}