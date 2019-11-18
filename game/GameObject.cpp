#include "GameObject.h"




//Object_feature* MapCell::find_property(property_e kind, GameObject* excluded)
//{
//	Object_feature* result(nullptr);
//	for (std::list<GameObject*>::iterator item = m_items.begin(); item != m_items.end(); item++)
//	{
//		if (excluded != (*item))
//		{
//			result = (*item)->m_active_state->find_property(kind);
//			if (result != nullptr)
//			{
//				return result;
//			}
//		}
//	}
//	return nullptr;
//}

GameObject::GameObject()
{
	m_kind = entity_e::game_object;
	m_owner = nullptr;
	m_direction = object_direction_e::down;
	m_selected = false;
	m_active_state = nullptr;
	m_owner = nullptr;
	//rendering_necessary = false;
}

Object_interaction* GameObject::clone()
{
    Logger::instance().critical("Метод не описан.");
    return nullptr;
}

void GameObject::do_predicate(predicate func)
{
    func(this);
    Logger::instance().critical("Метод не описан.");
}

void GameObject::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
};

void GameObject::turn()
{
    object_direction_e dir;
    if (m_direction == object_direction_e::down) {
        dir = object_direction_e::left;
    }
    if (m_direction == object_direction_e::left) {
        dir = object_direction_e::top;
    }
    if (m_direction == object_direction_e::top) {
        dir = object_direction_e::right;
    }
    if (m_direction == object_direction_e::right) {
        dir = object_direction_e::down;
    }
    set_direction(dir);
}

void GameObject::set_direction(object_direction_e dir)
{
    /*if (((m_direction == object_direction_e_Left || m_direction == object_direction_e_Right) && (dir == object_direction_e_Up || dir == object_direction_e_Down)) || ((dir == object_direction_e_Left || dir == object_direction_e_Right) && (m_direction == object_direction_e_Up || m_direction == object_direction_e_Down)))
	{
		for (std::list<Object_state*>::iterator item = m_state.begin(); item != m_state.end(); item++)
		{
			(*item)->m_size = game_object_size_t((*item)->m_size.y, (*item)->m_size.x, (*item)->m_size.z);
		}
	}*/
    for (auto& item : m_state) {
        item->set_tile_size();
    }
    m_direction = dir;
}

void GameObject::set_state(object_state_e state)
{
    for (auto& item : m_state) {
        if (item->m_state == state) {
            m_active_state = item;
            return;
        }
    }
}

Object_state* GameObject::get_state(object_state_e state)
{
    for (auto& item : m_state) {
        if (item->m_state == state) {
            return item;
        }
    }
    return nullptr;
}

void GameObject::add_effect(interaction_e key, Object_interaction* item)
{
    if (m_active_state->m_items.find(key) != m_active_state->m_items.end()) {
        m_active_state->m_items[key]->add(item);
    }
}

void GameObject::remove_effect(interaction_e key, Object_interaction* item)
{
    if (m_active_state) {
        if (m_active_state->m_items.find(key) != m_active_state->m_items.end()) {
            m_active_state->m_items[key]->remove(item);
        }
    }
}

void GameObject::add_from(interaction_e key, Object_interaction* item)
{
    if (m_active_state->m_items.find(key) != m_active_state->m_items.end()) {
        m_active_state->m_items[key]->equip(item);
    }
}

void GameObject::add_from(interaction_e key, Interaction_list* feature)
{
    if (m_active_state) {
        if (m_active_state->m_items.find(key) == m_active_state->m_items.end()) {
            m_active_state->m_items[key] = feature;
        } else {
            m_active_state->m_items[key]->equip(feature);
        }
    }
}

void GameObject::remove_from(const interaction_e key, Interaction_list* feature)
{
    if (m_active_state) {
        const auto item = m_active_state->m_items.find(key);
        if (item != m_active_state->m_items.end()) {
            if (item->second == feature) {
                m_active_state->m_items.erase(key);
            } else {
                m_active_state->m_items[key]->unequip(feature);
            }
        }
    }
}

Interaction_list* GameObject::get_effect(interaction_e key)
{
    if (m_active_state) {
	    const auto value = m_active_state->m_items.find(key);
        if (value != m_active_state->m_items.end()) {
            return value->second;
        }
    }
    return nullptr;
}

Parameter_list* GameObject::get_parameter(interaction_e key)
{
    if (m_active_state) {
	    const auto value = m_active_state->m_items.find(key);
        if (value != m_active_state->m_items.end()) {
            return static_cast<Parameter_list*>(value->second);
        }
    }
    return nullptr;
}

Parts_list* GameObject::get_parts_list(interaction_e key)
{
    if (m_active_state) {
	    const auto value = m_active_state->m_items.find(key);
        if (value != m_active_state->m_items.end()) {
            return static_cast<Parts_list*>(value->second);
        }
    }
    return nullptr;
}

bool GameObject::get_stat(object_tag_e key)
{
    if (m_active_state) {
        return m_active_state->get_stat(key);
    }
    return false;
}

Object_tag* GameObject::get_tag(object_tag_e key)
{
    if (m_active_state) {
        return m_active_state->get_tag(key);
    }
    return nullptr;
}

MapCell* GameObject::cell()
{
    return static_cast<MapCell*>(m_owner);
}

bool GameObject::is_own(MapCell* c)
{
    auto& oc = *cell();
    return c->x >= oc.x && c->x < oc.x + m_active_state->m_size.dx && c->y <= oc.y && c->y > oc.y - m_active_state->m_size.dy && c->z >= oc.z && c->z < oc.z + m_active_state->m_size.dz;
}

bool GameObject::is_own(int x, int y, int z)
{
    auto& oc = *cell();
    return x >= oc.x && x < oc.x + m_active_state->m_size.dx && y <= oc.y && y > oc.y - m_active_state->m_size.dy && z >= oc.z && z < oc.z + m_active_state->m_size.dz;
}

MapCell* GameObject::get_center(MapCell* c)
{
    if ((m_active_state->m_size.dx % 2) == 0) {
        int x;
        int y;
        if (c->x <= cell()->x + m_active_state->m_size.dx / 2 - 1) {
            x = cell()->x + m_active_state->m_size.dx / 2 - 1;
        } else {
            x = cell()->x + m_active_state->m_size.dx / 2;
        }
        if (c->y >= cell()->y - m_active_state->m_size.dy / 2 + 1) {
            y = cell()->y - m_active_state->m_size.dy / 2 + 1;
        } else {
            y = cell()->y - m_active_state->m_size.dy / 2;
        }
        return &cell()->m_map->get(c->z, y, x);
    } else {
        auto x = cell()->x + m_active_state->m_size.dx / 2;
        auto y = cell()->y - m_active_state->m_size.dy / 2;
        return &cell()->m_map->get(c->z, y, x);
    }
}

void GameObject::update_interaction()
{
    if (m_active_state) {
        bool was_changed;
        int i = 0;
        do {
            was_changed = false;
            for (auto& m_item : m_active_state->m_items) {
                was_changed = m_item.second->update();
                if (was_changed)
                    break;
            }
            i += 1;
        } while (was_changed);
    }
}

GameObject::Action_getter::Action_getter(GameObject* object, std::list<Action_helper_t>& list)
    : m_list(list)
    , m_object(object)
{
}

void GameObject::Action_getter::visit(Object_interaction& value)
{
    switch (value.get_interaction_message_type()) {
    /*case interaction_message_type_e::action:
	{
		m_list.push_back(Action_helper_t(static_cast<Action*>(&value)));
		break;
	}*/
    case interaction_message_type_e::part: {
        auto& op = dynamic_cast<ObjectPart&>(value);
        auto tl = static_cast<Tag_list*>(op.m_attributes.get_list(interaction_e::tag));
        if (tl) {
            for (auto& m_item : tl->m_items) {
                const auto t = static_cast<Object_tag*>(m_item);
                switch (t->m_type) {
                case object_tag_e::can_transfer_object: {
                    if (op.m_item) {
                        const auto p = new Parameter(parameter_type_e::destination);
                        (*p)[0].set(m_object);
                        (*p)[1].set(op.m_item);
                        (*p)[3].set(&op);
                        m_list.emplace_back(Application::instance().m_actions[action_e::move_out], p);
                    } else {
                        const auto p = new Parameter(parameter_type_e::destination);
                        (*p)[0].set(m_object);
                        (*p)[2].set(&op);
                        (*p)[3].set(&op);
                        m_list.emplace_back(Application::instance().m_actions[action_e::pick], p);
                    }
                    break;
                }
                }
            }
        }

        auto* al = static_cast<Action_list*>(op.m_attributes.get_list(interaction_e::action));
        Action* a;
        if (al) {
            for (auto& m_item : al->m_items) {
                a = static_cast<Action*>(m_item);
                switch (a->m_kind) {
                case action_e::pick: {
                    const auto p = new Parameter(parameter_type_e::destination);
                    (*p)[0].set(m_object);
                    (*p)[2].set(&op);
                    //m_common_actions.pick = true;
                    m_list.emplace_back(a, p);
                    break;
                }
                case action_e::move_step: {
                    const auto p = new Parameter(parameter_type_e::position);
                    (*p)[0].set(m_object);
                    m_list.emplace_back(a, p);
                    break;
                }
                case action_e::turn: {
                    const auto p = new Parameter(parameter_type_e::direction);
                    (*p)[0].set(m_object);
                    m_list.emplace_back(a, p);
                    break;
                }
                case action_e::hit_melee: {
                    const auto p = new Parameter(parameter_type_e::interaction_cell);
                    (*p)[0].set(m_object);
                    (*p)[2].set(&op);
                    m_list.emplace_back(a, p);
                    break;
                }
                }
            }
        }
        if (op.m_item) {
            al = static_cast<Action_list*>(op.m_item->get_effect(interaction_e::action));
            if (al) {
                for (auto& m_item : al->m_items) {
                    a = static_cast<Action*>(m_item);
                    switch (a->m_kind) {
                    case action_e::hit_melee: {
                        const auto p = new Parameter(parameter_type_e::interaction_cell);
                        (*p)[0].set(m_object);
                        (*p)[2].set(&op);
                        m_list.emplace_back(a, p);
                        break;
                    }
                    case action_e::shoot: {
                        const auto p = new Parameter(parameter_type_e::bow_shoot);
                        (*p)[0].set(m_object);
                        (*p)[2].set(&op);
                        m_list.emplace_back(a, p);
                        break;
                    }
                    }
                }
            }
        }
        break;
    }
    }
}

void GameObject::get_actions_list(std::list<Action_helper_t>& value)
{
    Action_getter ag(this, value);

    auto al = static_cast<Action_list*>(get_effect(interaction_e::action));
    if (al) {
        al->do_predicate(ag);
    }

    auto parts = get_parts_list(interaction_e::body);
    parts->do_predicate(ag);
}

iPacker& GameObject::get_packer()
{
    return Packer<GameObject>::instance();
}

interaction_message_type_e GameObject::get_interaction_message_type()
{
	return interaction_message_type_e::game_object;
}

std::u16string GameObject::serialize_to_json_reference(JsonWriter& value)
{
    return value.write(*this);
}

std::u16string GameObject::serialize_to_json_pointer(JsonWriter& value)
{
    return value.write(this);
}

void GameObject::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_owner) {
        m_owner->reset_serialization_index();
    }
    for (auto item : m_state) {
        item->reset_serialization_index();
    }
}


Player::Player(GameObject* object, GameMap* map)
    : m_object(object)
    , m_map(map)
{
	auto ai = dynamic_cast<AI_enemy*>(m_object->m_active_state->m_ai);
    ai->calculate_fov(m_object, map);
	const auto vl = dynamic_cast<Vision_list*>(m_object->m_active_state->get_list(interaction_e::vision));

    int radius = vl->m_max_radius;

    int xc = m_object->cell()->x;
    int yc = m_object->cell()->y;
    int zc = m_object->cell()->z;
    int dx = m_object->m_active_state->m_size.dx;
    int dy = m_object->m_active_state->m_size.dy;
    int dz = m_object->m_active_state->m_size.dz;

    int xs = ((dx - 1) >> 1);
    int ys = ((dy - 1) >> 1);
    int zs = ((dz - 1) >> 1);

    int x_start = xc - radius + xs;
    x_start = std::max(x_start, 0);
    x_start = std::min(x_start, m_map->m_size.dx - 1);
    int x_end = xc + radius + xs;
    x_end = std::max(x_end, 0);
    x_end = std::min(x_end, m_map->m_size.dx - 1);
    int y_start = yc - radius - ys;
    y_start = std::max(y_start, 0);
    y_start = std::min(y_start, m_map->m_size.dy - 1);
    int y_end = yc + radius - ys;
    y_end = std::max(y_end, 0);
    y_end = std::min(y_end, m_map->m_size.dy - 1);
    int z_start = zc - radius + zs;
    z_start = std::max(z_start, 0);
    z_start = std::min(z_start, m_map->m_size.dz - 1);
    int z_end = zc + radius + zs;
    z_end = std::max(z_end, 0);
    z_end = std::min(z_end, m_map->m_size.dz - 1);

    for (int z = z_start; z < z_end + 1; ++z) {
        for (int y = y_start; y < y_end + 1; ++y) {
            for (int x = x_start; x < x_end + 1; ++x) {
                if (ai->m_fov->m_map[y - y_start][x - x_start].visible) {
                    m_map->get(z, y, x).m_notable = true;
                }
            }
        }
    }

    //LOG(INFO) << "Поле зрение " << std::to_string(static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_fov_radius);
    m_actions.push_front(Application::instance().m_actions[action_e::autoexplore]);
    m_actions.push_front(Application::instance().m_actions[action_e::set_motion_path]);
    //m_actions.push_front(Application::instance().m_actions[action_e::open_inventory]);
    m_actions.push_front(Application::instance().m_actions[action_e::cell_info]);
    m_actions.push_front(Application::instance().m_actions[action_e::equip]);
    m_actions.push_front(Application::instance().m_actions[action_e::show_parameters]);
    m_actions.push_front(Application::instance().m_actions[action_e::save]);
    m_actions.push_front(Application::instance().m_actions[action_e::load]);
    m_actions.push_front(Application::instance().m_actions[action_e::rotate_view]);
    m_actions.push_front(Application::instance().m_actions[action_e::change_z_level]);
    m_object->event_update += std::bind(&Event<VoidEventArgs>::operator(), &event_update, std::placeholders::_1);
}

void Player::get_actions_list(std::list<Action_helper_t>& value)
{
    for (auto& m_action : m_actions) {
        value.emplace_back(m_action);
    }
    m_object->get_actions_list(value);
}

//Inventory_cell::Inventory_cell(): m_item(nullptr)
//{
//}



iPacker& Config::get_packer()
{
    return Packer<Config>::instance();
}

void Config::instancedictonary_icon_from_json(const std::u16string& value, InstanceDictionary<Icon*>& prop, SerializationContext& context)
{
    auto temp = value;
    const auto s = read_array(temp);
    if (s) {
        for (const auto& element : (*s)) {
            auto&& name = utf16_to_cp1251(get_value(element));
            const auto icon = new Icon;
            icon->m_value = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + name + ".bmp");
            prop.add(icon, name);
        }
        delete s;
    }
}

void Config::instancedictonary_icon_from_binary(const std::string& value, InstanceDictionary<Icon*>& prop, std::size_t& pos, SerializationContext& context)
{
}

void Config::instancedictonary_tilemanager_from_json_atlas(const std::u16string& value, InstanceDictionary<TileManager*>& prop, SerializationContext& context)
{
    auto temp = value;
    const auto s = read_pair(temp);
    std::string k;
    TileManager* v = nullptr;
    std::vector<std::string> names;
    if (s) {
        for (std::size_t i = 0; i < s->size(); i += 2) {
            auto& name = names.emplace_back();
            parser_from_json<std::string>((*s)[i], name, context);
            name = FileSystem::instance().m_resource_path + "Tiles\\" + name + ".png";
        }
        const auto id = Application::instance().m_graph->texture_array_load(names);

        for (std::size_t i = 0; i < s->size(); i += 2) {
            const auto p = read_pair((*s)[i + 1]);
            for (std::size_t j = 0; j < p->size(); j += 2) {
                parser_from_json<std::string>((*p)[j], k, context);
                parser_from_json<TileManager*>((*p)[j + 1], v, context);
                v->m_tiles[0] = id;
                v->m_layer = i / 2;
                m_tile_managers.add(v, k);
            }
            delete p;
        }
        delete s;
    }
}

void Config::instancedictonary_tilemanager_from_json(const std::u16string& value, InstanceDictionary<TileManager*>& prop, SerializationContext& context)
{
    auto temp = value;
    const auto s = read_array(temp);
    std::string k;
    TileManager* v = nullptr;
    if (s) {
        for (auto element : (*s)) {
            const auto p = read_pair(element);
            parser_from_json<std::string>((*p)[0], k, context);
            parser_from_json<TileManager*>((*p)[1], v, context);
            prop.add(v, k);
        }
        delete s;
    }
}

void Config::instancedictonary_tilemanager_from_binary(const std::string& value, InstanceDictionary<TileManager*>& prop, std::size_t& pos, SerializationContext& context)
{
}