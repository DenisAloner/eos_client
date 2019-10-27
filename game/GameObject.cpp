#include "game/GameObject.h"
#include <algorithm>

Game_object_owner* Game_object_owner::get_owner(entity_e kind)
{
    if (m_owner) {
        if (m_owner->m_kind == kind) {
            return m_owner;
        } else {
            return m_owner->get_owner(kind);
        }
    }
    return nullptr;
}

void Game_object_owner::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_owner) {
        m_owner->reset_serialization_index();
    }
}

Game_object_owner::Game_object_owner()
    : m_owner(nullptr)
{
}

Game_object_owner* Game_object_owner::get_owner()
{
    if (m_owner) {
        return m_owner->get_owner();
    }
    return this;
}

MapCell::MapCell()
{
    m_kind = entity_e::cell;
    m_notable = false;
    m_owner = nullptr;
}

MapCell::MapCell(int x, int y, int z, GameMap* map)
    : m_map(map)
    , x(x)
    , y(y)
    , z(z)
{
    m_kind = entity_e::cell;
    m_notable = false;
    m_owner = nullptr;
}

void MapCell::add_object(GameObject* object)
{
    m_items.push_back(object);
}

void MapCell::reset_serialization_index()
{
    m_serialization_index = 0;
    for (auto obj : m_items) {
        if (obj->m_serialization_index != 0) {
            obj->reset_serialization_index();
        }
    }
}

Packer_generic& MapCell::get_packer()
{
    return Packer<MapCell>::instance();
}

Attribute_map::Attribute_map() {};

Interaction_list* Attribute_map::create_feature_list(feature_list_type_e key, interaction_e name)
{
    const auto result = Effect_functions::create_feature_list(key, name);
    m_items[name] = result;
    return result;
}

void Attribute_map::add_effect(interaction_e key, Object_interaction* item)
{
    if (m_items.find(key) != m_items.end()) {
        m_items[key]->add(item);
    }
}

Interaction_list* Attribute_map::get_list(interaction_e key)
{
	const auto value = m_items.find(key);
    if (value != m_items.end()) {
        return value->second;
    }
    return nullptr;
}

Attribute_map* Attribute_map::clone()
{
	auto result = new Attribute_map();
    for (auto& m_item : m_items)
    {
        result->m_items[m_item.first] = m_item.second->clone();
    }
    return result;
}

void Attribute_map::reset_serialization_index()
{
    m_serialization_index = 0;
    for (auto m_item : m_items) {
        m_item.second->reset_serialization_index();
    }
}

Tag_getter::Tag_getter(object_tag_e key)
    : m_key(key)
    , m_result(nullptr) {};

void Tag_getter::visit(Object_interaction& value)
{
    if (!m_result) {
        if (value.m_interaction_message_type == interaction_message_type_e::tag) {
            if (static_cast<Object_tag&>(value).m_type == m_key) {
                m_result = static_cast<Object_tag*>(&value);
            }
        }
    }
}

bool Attribute_map::get_stat(const object_tag_e key)
{
    const auto list = m_items.find(interaction_e::tag);
    if (list != m_items.end()) {
        auto taglist = static_cast<Tag_list*>(list->second);
        Tag_getter tg(key);
        taglist->do_predicate(tg);
        return tg.m_result;
    }
    return false;
}

Object_tag* Attribute_map::get_tag(const object_tag_e key)
{
    const auto list = m_items.find(interaction_e::tag);
    if (list != m_items.end()) {
        auto taglist = static_cast<Tag_list*>(list->second);
        Tag_getter tg(key);
        taglist->do_predicate(tg);
        return tg.m_result;
    }
    return nullptr;
}

void Attribute_map::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

Packer_generic& Attribute_map::get_packer()
{
    return Packer<Attribute_map>::instance();
}

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

Object_state_equip::Object_state_equip()
{
    m_state = object_state_e::equip;
}

Object_state* Object_state_equip::clone()
{
    auto* state = new Object_state_equip();
    state->m_body_part = m_body_part;
    state->m_state = m_state;
    state->m_layer = m_layer;
    state->m_size = m_size;
    state->m_tile_size = m_tile_size;
    state->m_tile_manager = m_tile_manager;
    state->m_light = m_light;
    state->m_icon = m_icon;
    if (m_ai) {
        state->m_ai = m_ai->clone();
    } else
        state->m_ai = nullptr;

    for (auto& m_item : m_items) {
        state->m_items[m_item.first] = m_item.second->clone();
    }

    for (auto& m_item : m_equip.m_items) {
        state->m_equip.m_items[m_item.first] = m_item.second->clone();
    }
    return state;
}

Packer_generic& Object_state_equip::get_packer()
{
    return Packer<Object_state_equip>::instance();
}

//Action* Object_state::find_action(action_e kind)
//{
//	for (std::list<Action*>::iterator Current = m_actions.begin(); Current != m_actions.end(); Current++)
//	{
//		if ((*Current)->m_kind == kind)
//		{
//			return (*Current);
//		}
//	}
//	return nullptr;
//}

void Object_state::set_tile_size()
{
    //m_tile_size = dimension_t(tile_size_x_half*(m_size.dx + m_size.dy), tile_size_y_half*(m_size.dx + m_size.dy) + m_size.dz * tile_size_y);
    m_tile_size = dimension_t<int>(tile_size_x_half * (1 + 1), tile_size_y_half * (1 + 1) + 1 * tile_size_y);
}

void Object_state::reset_serialization_index()
{
    m_serialization_index = 0;
    Attribute_map::reset_serialization_index();
    if (m_ai) {
        m_ai->reset_serialization_index();
    }
}

Packer_generic& Object_state::get_packer()
{
    return Packer<Object_state>::instance();
}

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
    m_interaction_message_type = interaction_message_type_e::game_object;
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
    switch (value.m_interaction_message_type) {
    /*case interaction_message_type_e::action:
	{
		m_list.push_back(Action_helper_t(static_cast<Action*>(&value)));
		break;
	}*/
    case interaction_message_type_e::part: {
        auto& op = dynamic_cast<Object_part&>(value);
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

Packer_generic& GameObject::get_packer()
{
    return Packer<GameObject>::instance();
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

Inventory_cell::Inventory_cell(GameObject* item)
    : m_item(item)
{
    m_kind = entity_e::inventory_cell;
}

void Inventory_cell::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_item) {
        // ??? исправить, временная заглушка, необходимо добавить свойство, которое показывает был ли сброшен объект для сохранения уже
        if (m_item->m_serialization_index != 0) {
            m_item->reset_serialization_index();
        }
    }
}

Packer_generic& Inventory_cell::get_packer()
{
    return Packer<Inventory_cell>::instance();
}

Object_part::Object_part(GameObject* item)
    : Inventory_cell(item)
{
    m_interaction_message_type = interaction_message_type_e::part;
    m_kind = entity_e::body_part;
    m_part_kind = body_part_e::container;
    m_owner = nullptr;
    m_attributes.create_feature_list(feature_list_type_e::parts, interaction_e::body);
};

Object_part* Object_part::clone()
{
    auto result = new Object_part();
    result->m_kind = m_kind;
    result->m_name = m_name;
    result->m_part_kind = m_part_kind;
    result->m_item = nullptr;
    result->m_attributes = *m_attributes.clone();
    return result;
}

void Object_part::do_predicate(Visitor& helper)
{
    helper.visit(*this);
    for (auto& m_item : m_attributes.m_items) {
        m_item.second->do_predicate(helper);
    }
}

void Object_part::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

void Object_part::reset_serialization_index()
{
    m_serialization_index = 0;
    m_attributes.reset_serialization_index();
    if (m_item) {
        // ??? исправить, временная заглушка, необходимо добавить свойство, которое показывает был ли сброшен объект для сохранения уже
        if (m_item->m_serialization_index != 0) {
            m_item->reset_serialization_index();
        }
    }
}

Packer_generic& Object_part::get_packer()
{
    return Packer<Object_part>::instance();
}

Packer_generic& Config::get_packer()
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