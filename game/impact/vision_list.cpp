#include "vision_list.h"
#include "Visitors.h"
#include "vision_component.h"

Vision_list::Vision_list()
{
    m_list_type = feature_list_type_e::vision;
};

void Vision_list::update_list(Object_interaction* list)
{
    switch (list->get_interaction_message_type()) {
    case interaction_message_type_e::list: {
        switch (static_cast<Interaction_list*>(list)->m_list_type) {
        case feature_list_type_e::vision_component: {
	        auto list_item = static_cast<Vision_component*>(list);
            list_item->update();
            m_max_radius = std::max(m_max_radius, list_item->m_value.radius);
            break;
        }
        default: {
	        auto list_item = static_cast<Interaction_list*>(list);
            for (auto& m_item : list_item->m_items)
            {
                update_list(m_item);
            }
            break;
        }
        }
        break;
    }
    }
}

bool Vision_list::update()
{
    //LOG(INFO) << "ÒÈÏ ÏÀÐÀÌÅÒÐÀ " << std::to_string((int)m_subtype);
    m_max_radius = 0;
    update_list(this);
    return false;
}

Vision_list* Vision_list::clone()
{
	auto result = new Vision_list;
    result->m_list_type = m_list_type;
    for (auto& m_item : m_items)
    {
        result->add(m_item->clone());
    }
    return result;
}

void Vision_list::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

void Vision_list::equip(Object_interaction* item)
{
    switch (item->get_interaction_message_type()) {
    case interaction_message_type_e::list: {
        switch (static_cast<Interaction_list*>(item)->m_list_type) {
        case feature_list_type_e::vision_component: {
            m_items.push_back(item);
            break;
        }
        default: {
            auto list_item = static_cast<Interaction_list*>(item);
            for (auto& m_item : list_item->m_items) {
                equip(m_item);
            }
            break;
        }
        }
        break;
    }
    default: {
        for (auto& m_item : m_items) {
            static_cast<Vision_component*>(m_item)->equip(item);
        }
        break;
    }
    }
}

void Vision_list::unequip(Object_interaction* item)
{
    switch (item->get_interaction_message_type()) {
    case interaction_message_type_e::list: {
        switch (static_cast<Interaction_list*>(item)->m_list_type) {
        case feature_list_type_e::vision_component: {
            m_items.remove(item);
            break;
        }
        default: {
	        auto list_item = static_cast<Interaction_list*>(item);
            for (auto& m_item : list_item->m_items)
            {
                unequip(m_item);
            }
            break;
        }
        }
        break;
    }
    default: {
        for (auto& m_item : m_items)
        {
            static_cast<Vision_component*>(m_item)->unequip(item);
        }
        break;
    }
    }
}

iPacker& Vision_list::get_packer()
{
    return Packer<Vision_list>::instance();
}