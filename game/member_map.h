#ifndef MEMBER_MAP_H
#define MEMBER_MAP_H

#include <string>
#include <type_traits>
#include "json_helpers.h"
#include "game_world.h"

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


template <>
constexpr auto MemberMap::get<&GameWorld::m_maps> = JsonProperty(&GameWorld::m_maps, u"maps");

template <>
constexpr auto MemberMap::get<&GameWorld::m_objects> = JsonProperty(&GameWorld::m_objects, u"objects");

template <>
constexpr auto MemberMap::get<&GameMap::m_size> = JsonProperty(&GameMap::m_size, u"size");

template <>
constexpr auto MemberMap::get<&GameObject::m_name> = JsonProperty(&GameObject::m_name, u"name");

template <>
constexpr auto MemberMap::get<&GameObject::m_direction> = JsonProperty(&GameObject::m_direction, u"direction");

template <>
constexpr auto MemberMap::get<&GameObject::m_owner> = JsonProperty(&GameObject::m_owner, u"owner");

template <>
constexpr auto MemberMap::get<&GameObject::m_state> = JsonProperty(&GameObject::m_state, u"state");

template <>
constexpr auto MemberMap::get<&GameObject::m_active_state> = JsonProperty(&GameObject::m_active_state, u"active_state");

template <>
constexpr auto MemberMap::get<&Attribute_map::m_items> = JsonProperty(&Attribute_map::m_items, u"items");

template <>
constexpr auto MemberMap::get<&Object_state::m_state> = JsonProperty(&Object_state::m_state, u"state");

template <>
constexpr auto MemberMap::get<&Interaction_list::m_list_type> = JsonProperty(&Interaction_list::m_list_type, u"list_type");

template <>
constexpr auto MemberMap::get<&MapCell::x> = JsonProperty(&MapCell::x, u"x");

template <>
constexpr auto MemberMap::get<&MapCell::y> = JsonProperty(&MapCell::y, u"y");

template <>
constexpr auto MemberMap::get<&MapCell::z> = JsonProperty(&MapCell::z, u"z");

template <>
constexpr auto MemberMap::get<&MapCell::m_map> = JsonProperty(&MapCell::m_map, u"map");

template <>
constexpr auto MemberMap::get<&InventoryCell::m_item> = JsonProperty(&InventoryCell::m_item, u"item");

template <>
constexpr auto MemberMap::get<&ObjectPart::m_part_kind> = JsonProperty(&ObjectPart::m_part_kind, u"part_kind");

template <>
constexpr auto MemberMap::get<&ObjectPart::m_name> = JsonProperty(&ObjectPart::m_name, u"name");

template <>
constexpr auto MemberMap::get<&ObjectPart::m_attributes> = JsonProperty(&ObjectPart::m_attributes, u"attributes");


#endif
