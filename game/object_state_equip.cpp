#include "object_state_equip.h"

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

iPacker& Object_state_equip::get_packer()
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
