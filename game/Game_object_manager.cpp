#include "Game_object_manager.h"
#include "GameObject.h"

Config* GameObjectManager::m_config = new Config();

void GameObjectManager::init(SerializationContext& context)
{

    m_effect_prefix_string[effect_prefix_e::physical_damage] = "физический урон";
    m_effect_prefix_string[effect_prefix_e::poison_damage] = "урон от яда";
    m_effect_prefix_string[effect_prefix_e::state_change] = "изменить состояние";

    m_to_effect_prefix_e["physical_damage"] = effect_prefix_e::physical_damage;
    m_to_effect_prefix_e["poison_damage"] = effect_prefix_e::poison_damage;

    bytearray json;
    FileSystem::instance().load_from_file(FileSystem::instance().m_resource_path + "Configs\\Objects.json", json);
    const std::u8string json_u8(json);
    const auto json_config(utf8_to_utf16(json_u8));

    auto temp(json_config);
    const auto s = read_object(temp);
    m_config->from_json(s, context);
    delete s;

    //m_items.insert(m_config->m_items.begin(), m_config->m_items.end());
};

GameObject* GameObjectManager::new_object(const std::string unit_name)
{
    auto& obj = Application::instance().m_world->m_objects.emplace_back();
    const auto it = m_config->m_items.find(unit_name);
    if (it == m_config->m_items.end()) {
        Logger::instance().critical("Элемент `" + unit_name + "` отсутствует в m_items");
    }
    auto& config = it->second;

    /*Parser::reset_object_counter();
	config.reset_serialization_index();
	std::u16string json_test = Parser::parser_to_json<GameObject>(config);
	LOG(FATAL) << Parser::UTF16_to_CP1251(json_test);*/

    obj.m_direction = config.m_direction;
    obj.m_name = config.m_name;
    for (auto& item : config.m_state) {
        auto state = item->clone();
        obj.m_state.push_back(state);
    }
    obj.m_active_state = obj.m_state.front();
    //register_object(obj);
    obj.set_direction(obj.m_direction);
    bind_body(&obj);
    obj.update_interaction();
    return &obj;
}

//void GameObjectManager::register_object(GameObject*& object)
//{
//	m_objects.push_back(object);
//	if (object->get_effect(interaction_e::buff))
//	{
//		m_update_buff.push_front(object);
//	}
//}

std::string GameObjectManager::get_effect_prefix_string(effect_prefix_e key)
{
    const auto value = m_effect_prefix_string.find(key);
    if (value != m_effect_prefix_string.end()) {
        return value->second;
    }
    return "неизвестный тип";
}

effect_prefix_e GameObjectManager::get_effect_prefix_e(const std::string& key)
{
    const auto value = m_to_effect_prefix_e.find(key);
    if (value == m_to_effect_prefix_e.end()) {
        Logger::instance().critical("Элемент `" + key + "` отсутствует в m_items");
    }
    return value->second;
}

void GameObjectManager::bind_body(GameObject* object)
{
    for (auto item = object->m_state.begin(); item != object->m_state.end(); ++item) {
        auto list = (*item)->get_list(interaction_e::body);
        if (list) {
            Visitor_part_hierarchy_setter setter;
            setter.m_game_object_owner_stack.push_front(object);
            list->apply_visitor(setter);
        }
    }
}

void Visitor_part_hierarchy_setter::visit(ObjectPart& value)
{
    value.m_owner = m_game_object_owner_stack.front();
    m_game_object_owner_stack.push_front(&value);
    Visitor_simple::visit(value);
    m_game_object_owner_stack.pop_front();
}
