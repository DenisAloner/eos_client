#include "game_world.h"

GameWorld::GameWorld()
{
    float l;
    for (int z = 0; z < 21; z++) {
        for (int y = 0; y < 21; y++) {
            for (int x = 0; x < 21; x++) {
                l = static_cast<float>(sqrt(x * x + y * y + z * z));
                l = l / 20.0;
                m_coefficient[z][y][x] = 1 - l;
                if (m_coefficient[z][y][x] < 0) {
                    m_coefficient[z][y][x] = 0;
                }
            }
        }
    }
}

void GameWorld::reset_serialization_index()
{
    m_serialization_index = 0;
    for (auto& object : m_objects) {
        object.reset_serialization_index();
    }
    for (auto& map : m_maps) {
        map->reset_serialization_index();
    }
}

std::u16string GameWorld::serialize(SerializationContext& context)
{
    //LOG(INFO) << "std::u16string Object_manager::items_to_json(std::list<GameObject>& value)";
    std::size_t i = 0;
    for (auto& element : m_objects) {
        element.m_serialization_index = i;
        i += 1;
    }
    std::size_t ms = m_objects.size();
    //LOG(INFO) << "std::u16string GameWorld::serialize() " << std::to_string(ms);
    std::u16string result;
    for (auto& element : m_maps.front()->m_items) {
        if (!result.empty()) {
            result += u",";
        };
        result += parser_to_json<bool>(element.m_notable, context);
        result += u",";
        result += parser_to_json<std::list<GameObject*>>(element.m_items, context);
    }
    result = u"{\"map_size\":" + parser_to_json<dimension3_t>(m_maps.front()->m_size, context) + u",\"manager_size\":" + parser_to_json<std::size_t>(ms, context) + u",\"cells\":[" + result + u"],\"items\":";
    result += parser_to_json<std::list<GameObject>>(m_objects, context);
    result += u",\"player\":" + parser_to_json<GameObject*>(m_player->m_object, context);
    result += u"}";
    return result;
}

void GameWorld::deserialize(std::u16string& value, SerializationContext& context)
{
    //LOG(INFO) << "deserialize -> " << Parser::UTF16_to_CP1251(value);
    scheme_map_t* s = read_object(value);
    dimension3_t map_size;
    for (const auto& element : *s) {
        //LOG(INFO) << "deserialize -> " << Parser::UTF16_to_CP1251(element.first);
    }
    parser_from_json<dimension3_t>((*s)[u"map_size"], map_size, context);
    auto map = new GameMap(map_size);
    m_maps.push_front(map);

    std::size_t manager_size;
    parser_from_json<std::size_t>((*s)[u"manager_size"], manager_size, context);
    m_objects.resize(manager_size);

    scheme_list_t* items = read_array((*s)[u"cells"]);
    if (items) {
        auto it = items->begin();
        for (auto& element : m_maps.front()->m_items) {
            parser_from_json<bool>(*it, element.m_notable, context);
            ++it;
            parser_from_json<std::list<GameObject*>>(*it, element.m_items, context);
            ++it;
        }
        delete items;
    }

    items = read_array((*s)[u"items"]);
    if (items) {
        std::size_t i = 0;
        auto it = m_objects.begin();
        for (const auto& element : *items) {
            parser_from_json<GameObject>(element, *it, context);
            i += 1;
            ++it;
        }
        delete items;
    }

    GameObject* obj;
    parser_from_json<GameObject*>((*s)[u"player"], obj, context);
    m_player = new Player(obj, map);

    delete s;
}

std::string GameWorld::bin_serialize(SerializationContext& context)
{
    //LOG(INFO) << "std::string GameWorld::bin_serialize()";
    std::size_t i = 0;
    for (auto& element : m_objects) {
        element.m_serialization_index = i;
        i += 1;
    }
    auto ms = m_objects.size();
    //LOG(INFO) << "std::string GameWorld::bin_serialize() "<<std::to_string(ms);
    auto result = parser_to_binary<dimension3_t>(m_maps.front()->m_size, context) + parser_to_binary<std::size_t>(ms, context);
    for (auto& element : m_maps.front()->m_items) {
        result += parser_to_binary<std::list<GameObject*>>(element.m_items, context);
        result += parser_to_binary<bool>(element.m_notable, context);
    }
    for (auto& element : m_objects) {
        result += parser_to_binary<GameObject>(element, context);
    }
    result += parser_to_binary<GameObject*>(m_player->m_object, context);
    return result;
}

void GameWorld::bin_deserialize(std::string& value, SerializationContext& context)
{
    //LOG(INFO) << "GameWorld::bin_deserialize(std::string& value)";

    std::size_t pos = 0;
    dimension3_t map_size;
    parser_from_binary<dimension3_t>(value, map_size, pos, context);
    auto map = new GameMap(map_size);
    m_maps.push_front(map);

    std::size_t manager_size;
    parser_from_binary<std::size_t>(value, manager_size, pos, context);
    //LOG(INFO) << "void GameWorld::bin_deserialize(std::string& value) " << std::to_string(manager_size);
    m_objects.resize(manager_size);

    for (auto& element : m_maps.front()->m_items) {
        parser_from_binary<std::list<GameObject*>>(value, element.m_items, pos, context);
        parser_from_binary<bool>(value, element.m_notable, pos, context);
    }

    std::size_t i = 0;
    for (auto& element : m_objects) {
        parser_from_binary<GameObject>(value, element, pos, context);
        ++i;
    }
    GameObject* obj;
    parser_from_binary<GameObject*>(value, obj, pos, context);
    m_player = new Player(obj, map);
}

iPacker& GameWorld::get_packer()
{
    return Packer<GameWorld>::instance();
}

IVISITABLE_IMPL(GameWorld);

void GameWorld::calculate_lighting()
{
    int lx;
    int ly;
    int lz;
    int c;
    FOV_light fl;

    for (auto& m_map : m_maps) {
        auto& map = *m_map;
        for (int z = 0; z < map.m_size.dz; ++z) {
            for (int y = 0; y < map.m_size.dy; ++y) {
                for (int x = 0; x < map.m_size.dx; ++x) {
                    map.get(z, y, x).m_light = rgb_t<int>();
                }
            }
        }
    }
    for (auto& m_object : m_objects) {
        auto& object = m_object;
        if (!object.m_active_state->m_light) {
            continue;
        }
        if (object.m_owner->m_kind != entity_e::cell) {
            continue;
        }
        auto map = dynamic_cast<MapCell*>(object.m_owner)->m_map;
        fl.calculate(20, &m_object, map);
        for (int z = 0; z < 41; ++z) {
            for (int y = 0; y < 41; ++y) {
                for (int x = 0; x < 41; ++x) {
                    /*if(fl.get(z,y,x).visible)
					{*/
                    lx = abs(x - 20);
                    ly = abs(y - 20);
                    lz = abs(z - 20);
                    c = (m_object.m_active_state->m_light->r * m_coefficient[lz][ly][lx]); // *fl.m_map[y][x].damping.R;
                    if (c < 0) {
                        c = 0;
                    }
                    fl.get(z, y, x).light.r = c;
                    c = (m_object.m_active_state->m_light->g * m_coefficient[lz][ly][lx]); // *fl.m_map[y][x].damping.G;
                    if (c < 0) {
                        c = 0;
                    }
                    fl.get(z, y, x).light.g = c;
                    c = (m_object.m_active_state->m_light->b * m_coefficient[lz][ly][lx]); // *fl.m_map[y][x].damping.B;
                    if (c < 0) {
                        c = 0;
                    }
                    fl.get(z, y, x).light.b = c;
                    /*	}
					else
					{
						fl.get(z, y, x).light.R = 0;
						fl.get(z, y, x).light.G = 0;
						fl.get(z, y, x).light.B = 0;
					}*/
                }
            }
        }
        for (int z = 0; z < 41; ++z) {
            lz = m_object.cell()->z + z - 20;
            if (!((lz < 0) || (lz > map->m_size.dz - 1))) {
                for (int y = 0; y < 41; y++) {
                    ly = m_object.cell()->y + y - 20;
                    if (!((ly < 0) || (ly > map->m_size.dy - 1))) {
                        for (int x = 0; x < 41; x++) {
                            lx = m_object.cell()->x + x - 20;
                            if (!((lx < 0) || (lx > map->m_size.dx - 1))) {
                                map->get(lz, ly, lx).m_light.r += fl.get(z, y, x).light.r;
                                map->get(lz, ly, lx).m_light.g += fl.get(z, y, x).light.g;
                                map->get(lz, ly, lx).m_light.b += fl.get(z, y, x).light.b;
                            }
                        }
                    }
                }
            }
        }
    }
    for (auto& m_map : m_maps) {
        m_map->blur_lighting();
    }
}

void GameWorld::update_buff()
{
    Interaction_list* e;
    Visitor_onturn_updater visitor;

    for (auto& m_item : m_objects) {
        auto list = static_cast<Interaction_list*>(m_item.get_effect(interaction_e::buff));
        if (list) {
            e = list->clone();
            e->apply_effect(&m_item, nullptr);
            visitor.result = false;
            list->apply_visitor(visitor);
        }
        for (auto& item : m_item.m_active_state->m_items) {
            if (item.second->get_interaction_message_type() == interaction_message_type_e::list) {
                e = static_cast<Interaction_list*>(item.second);
                if (e->m_list_type == feature_list_type_e::parameter || e->m_list_type == feature_list_type_e::parts || e->m_list_type == feature_list_type_e::tag) {
                    visitor.result = false;
                    e->apply_visitor(visitor);
                }
            }
        }
        m_item.update_interaction();
        m_item.event_update(VoidEventArgs());
    }
}

void GameWorld::calculate_ai()
{
    for (auto& m_item : m_objects) {
        if (m_item.m_owner->m_kind == entity_e::cell) {
            if ((m_item.m_active_state->m_ai) && (&m_item != Application::instance().m_world->m_player->m_object)) {
                switch (m_item.m_active_state->m_ai->m_ai_type) {
                case ai_type_e::trap: {
                    auto obj = *dynamic_cast<AI_trap*>(m_item.m_active_state->m_ai);
                    obj.m_object = &m_item;
                    obj.create();
                    break;
                }
                case ai_type_e::non_humanoid: {
                    auto obj = dynamic_cast<AI_enemy*>(m_item.m_active_state->m_ai);
                    obj->m_map = dynamic_cast<MapCell*>(m_item.m_owner)->m_map;
                    obj->m_object = &m_item;
                    obj->create();
                    break;
                }
                }
            }
        }
    }
}