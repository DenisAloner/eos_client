#include "json_game_saver.h"

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
            MemberMap::get<&GameObject::m_owner>,
            MemberMap::get<&GameObject::m_state>,
            MemberMap::get<&GameObject::m_active_state>);
    }
    if constexpr (std::is_same_v<T, Attribute_map>) {
        return std::tuple(
            MemberMap::get<&Attribute_map::m_items>);
    }
    if constexpr (std::is_same_v<T, Object_state>) {
        return std::tuple(
            MemberMap::get<&Object_state::m_state>,
            MemberMap::get<&Object_state::m_size>,
            MemberMap::get<&Object_state::m_layer>,
            MemberMap::get<&Object_state::m_ai>);
    }
    if constexpr (std::is_same_v<T, Interaction_list>) {
        return std::tuple(
            MemberMap::get<&Interaction_list::m_list_type>);
    }
    if constexpr (std::is_same_v<T, MapCell>) {
        return std::tuple(
            MemberMap::get<&MapCell::x>,
            MemberMap::get<&MapCell::y>,
            MemberMap::get<&MapCell::z>,
            CustomWriter(MemberMap::get<&MapCell::m_map>, &JsonGameSaver::map_cell_owner_save));
    }
    if constexpr (std::is_same_v<T, InventoryCell>) {
        return std::tuple(
            MemberMap::get<&InventoryCell::m_item>);
    }
    if constexpr (std::is_same_v<T, ObjectPart>) {
        return std::tuple(
            MemberMap::get<&ObjectPart::m_part_kind>,
            MemberMap::get<&ObjectPart::m_name>,
            MemberMap::get<&ObjectPart::m_attributes>);
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
}

std::u16string JsonGameSaver::write(GameWorld& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(GameMap& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(GameObject& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(Attribute_map& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(Object_state& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(Interaction_list& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(MapCell& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(InventoryCell& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(ObjectPart& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(Game_object_owner* value)
{
    return value->serialize_to_json_pointer(*this);
}

std::u16string JsonGameSaver::write(MapCell* value)
{
    return value->serialize_to_json_reference(*this);
}

std::u16string JsonGameSaver::write(GameWorld* value)
{
    return write_pointer(value);
}

std::u16string JsonGameSaver::write(GameMap* value)
{
    Logger::instance().info("GameMap*");
    return write_pointer(value);
}

std::u16string JsonGameSaver::write(GameObject* value)
{
    return write_pointer(value);
}

std::u16string JsonGameSaver::write(Attribute_map* value)
{
    return write_pointer(value);
}

std::u16string JsonGameSaver::write(Object_state* value)
{
    return write_pointer(value);
}

std::u16string JsonGameSaver::write(Interaction_list* value)
{
    return write_pointer(value);
}

std::u16string JsonGameSaver::write(InventoryCell* value)
{
    return write_pointer(value);
}

std::u16string JsonGameSaver::write(ObjectPart* value)
{
    return write_pointer(value);
}

std::u16string JsonGameSaver::write(AI& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(AI* value)
{
    return write_pointer(value);
}

std::u16string JsonGameSaver::write(AI_enemy& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(AI_enemy* value)
{
    return write_pointer(value);
}

std::u16string JsonGameSaver::write(predicate_t& value)
{
    return write(value.index);
}

std::u16string JsonGameSaver::write(predicate_t* value)
{
    return write(value->index);
}

std::u16string JsonGameSaver::map_cell_owner_save(GameMap* value)
{
    return cp1251_to_utf16(std::to_string(value->m_index));
}
