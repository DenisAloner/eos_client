#include "json_game_saver.h"
#include "json_helpers.h"

template <typename T>
constexpr auto JsonGameSaver::object_properties()
{
    if constexpr (std::is_same<T, GameWorld>::value) {
        return std::tuple(
            json_property(&GameWorld::m_maps, u"maps"),
            json_property(&GameWorld::m_objects, u"objects")
		);
    }
    if constexpr (std::is_same<T, GameMap>::value) {
        return std::tuple(
            json_property(&GameMap::m_size, u"size")
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
            json_property(&MapCell::z, u"z"),
            json_property(&MapCell::m_map, u"map", &JsonGameSaver::map_cell_owner_save)
		);
    }
    if constexpr (std::is_same<T, InventoryCell>::value) {
        return std::tuple(
            json_property(&InventoryCell::m_item, u"item")
		);
    }
    if constexpr (std::is_same<T, ObjectPart>::value) {
        return std::tuple(
            json_property(&ObjectPart::m_part_kind, u"part_kind"),
            json_property(&ObjectPart::m_name, u"name"),
            json_property(&ObjectPart::m_attributes, u"attributes")
		);
    }
}

std::u16string JsonGameSaver::write(GameWorld& value)
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

std::u16string JsonGameSaver::write(InventoryCell& value)
{
    return write_object( value);
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

std::u16string JsonGameSaver::map_cell_owner_save(GameMap* value)
{
    return cp1251_to_utf16(std::to_string(value->m_index));
}