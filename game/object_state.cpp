#include "object_state.h"

std::u16string Object_state::icon_to_json(Icon*& value, SerializationContext& context)
{
    if (value) {
        return u"\"" + cp1251_to_utf16(value->m_json_string) + u"\"";
    } else {
        return u"null";
    }
}

std::string Object_state::icon_to_binary(Icon*& value, SerializationContext& context)
{
    std::size_t s;
    if (value)
        s = value->m_index + 1;
    else
        s = 0;
    return std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t));
}

void Object_state::icon_from_json(const std::u16string& value, Icon*& prop, SerializationContext& context)
{
    auto&& name = utf16_to_cp1251(get_value(value));
    prop = GameObjectManager::m_config->m_icons.get_by_string(name);
}

void Object_state::icon_from_binary(const std::string& value, Icon*& prop, std::size_t& pos, SerializationContext& context)
{
    const auto s = *reinterpret_cast<const std::size_t*>(&value[pos]);
    pos += sizeof(std::size_t);
    if (s == 0) {
        prop = nullptr;
    } else {
        prop = Application::instance().m_game_object_manager->m_config->m_icons.m_by_index[s - 1];
    }
}

std::u16string Object_state::tilemanager_to_json(TileManager*& value, SerializationContext& context)
{
    if (value) {
        return u"\"" + cp1251_to_utf16(value->m_json_string) + u"\"";

    } else {
        return u"null";
    }
}

std::string Object_state::tilemanager_to_binary(TileManager*& value, SerializationContext& context)
{
    std::size_t s;
    if (value)
        s = value->m_index + 1;
    else
        s = 0;
    return std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t));
}

void Object_state::tilemanager_from_json(const std::u16string& value, TileManager*& prop, SerializationContext& context)
{
    if (get_value(value) == u"null") {
        prop = nullptr;
    } else {
        auto&& result = utf16_to_cp1251(get_value(value));
        prop = Application::instance().m_game_object_manager->m_config->m_tile_managers.get_by_string(result);
    }
}

void Object_state::tilemanager_from_binary(const std::string& value, TileManager*& prop, std::size_t& pos, SerializationContext& context)
{
    const auto s = *reinterpret_cast<const std::size_t*>(&value[pos]);
    pos += sizeof(std::size_t);
    if (s == 0) {
        prop = nullptr;
    } else {
        //LOG(INFO) << "Tile manager" << std::to_string(s);
        prop = Application::instance().m_game_object_manager->m_config->m_tile_managers.m_by_index[s - 1];
    }
}

IVISITABLE_IMPL(Object_state);

Object_state::Object_state()
{
    m_layer = 1;
    m_light = nullptr;
    m_tile_manager = nullptr;
    m_icon = nullptr; // GameObjectManager::m_config.m_icons.begin()->second;
    m_ai = nullptr;
    m_optical = nullptr;
    m_visibility = nullptr;
    m_weight = 0.0F;
}

Object_state* Object_state::clone()
{
    auto state = new Object_state();
    state->m_state = m_state;
    state->m_layer = m_layer;
    state->m_size = m_size;
    state->m_tile_size = m_tile_size;
    state->m_tile_manager = m_tile_manager;
    state->m_icon = m_icon;
    state->m_weight = m_weight;
    if (m_light) {
        state->m_light = new rgb_t<int>(*m_light);
    }
    if (m_ai) {
        state->m_ai = m_ai->clone();
    } else
        state->m_ai = nullptr;
    if (m_optical) {
        state->m_optical = new optical_properties_t { *m_optical };
    }
    if (m_visibility) {
        state->m_visibility = new float;
        *state->m_visibility = *m_visibility;
    }
    for (auto& m_item : m_items) {
        state->m_items[m_item.first] = m_item.second->clone();
    }
    return state;
}

void Object_state::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

void Object_state::reset_serialization_index()
{
    m_serialization_index = 0;
    AttributeMap::reset_serialization_index();
    if (m_ai) {
        m_ai->reset_serialization_index();
    }
}

iPacker& Object_state::get_packer()
{
    return Packer<Object_state>::instance();
}
