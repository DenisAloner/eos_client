#include "json_game_saver.h"

template <typename T>
constexpr auto JsonGameSaver::object_properties()
{
    if constexpr (std::is_same<T, Game_world>::value) {
        return std::tuple(
            json_property(&Game_world::m_maps, u"maps"),
            json_property(&Game_world::m_objects, u"objects")
		);
    }
    if constexpr (std::is_same<T, GameMap>::value) {
        return std::tuple(
            json_property(&GameMap::m_size, u"size"),
            json_property(&GameMap::m_items, u"item", &JsonGameSaver::map_cell_save)
		);
    }
    if constexpr (std::is_same<T, GameObject>::value) {
        return std::tuple(
            json_property(&GameObject::m_name, u"name"),
            json_property(&GameObject::m_direction, u"direction"),
            json_property(&GameObject::m_owner, u"owner"),
            json_property(&GameObject::m_state, u"state"),
            json_property(&GameObject::m_active_state, u"active_state")
		);
    }
    if constexpr (std::is_same<T, Attribute_map>::value) {
        return std::tuple(
			json_property(&Attribute_map::m_items, u"items")
		);
    }
    if constexpr (std::is_same<T, Object_state>::value) {
        return std::tuple(
            json_property(&Object_state::m_state, u"state")
		);
    }
    if constexpr (std::is_same<T, Interaction_list>::value) {
        return std::tuple(
            json_property(&Interaction_list::m_list_type, u"list_type")
		);
    }
    if constexpr (std::is_same<T, MapCell>::value) {
        return std::tuple(
            json_property(&MapCell::x, u"x"),
            json_property(&MapCell::y, u"y"),
            json_property(&MapCell::m_items, u"items")
		);
    }
    if constexpr (std::is_same<T, Inventory_cell>::value) {
        return std::tuple(
            json_property(&Inventory_cell::m_item, u"item")
		);
    }
    if constexpr (std::is_same<T, Object_part>::value) {
        return std::tuple(
            json_property(&Object_part::m_part_kind, u"part_kind"),
            json_property(&Object_part::m_name, u"name"),
            json_property(&Object_part::m_attributes, u"attributes")
		);
    }
}

std::u16string JsonGameSaver::write(Game_world& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(GameMap& value)
{
    return write_object( value);
}

std::u16string JsonGameSaver::write(GameObject& value)
{
    return write_object( value);
}

std::u16string JsonGameSaver::write(Attribute_map& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::write(Object_state& value)
{
    return write_object( value);
}

std::u16string JsonGameSaver::write(Interaction_list& value)
{
    return write_object( value);
}

std::u16string JsonGameSaver::write(MapCell& value)
{
    return write_object( value);
}

std::u16string JsonGameSaver::write(Inventory_cell& value)
{
    return write_object( value);
}

std::u16string JsonGameSaver::write(Object_part& value)
{
    return write_object(value);
}

std::u16string JsonGameSaver::map_cell_save(std::vector<MapCell>& value)
{
    std::u16string result = u"[";
    for (auto& element : value) {
        if (result != u"[") {
            result += u",";
        }
        result += write(&element);
    }
    result += u"]";
    return result;
}