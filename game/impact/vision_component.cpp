#include "vision_component.h"
#include "Application.h"

Vision_component::Vision_component()
{
    m_list_type = feature_list_type_e::vision_component;
};

void Vision_component::update_list(Object_interaction* list)
{

    switch (list->get_interaction_message_type()) {
    case interaction_message_type_e::list: {
        auto list_item = static_cast<Interaction_list*>(list);
        for (auto& m_item : list_item->m_items) {
            update_list(m_item);
        }
        break;
    }
    case interaction_message_type_e::interaction_timer: {
        const auto item = static_cast<Interaction_time*>(list);
        update_list(item->m_value);
        break;
    }
    default: {
        const auto item = static_cast<Effect*>(list);
        switch (item->m_subtype) {
        case effect_e::limit: {
            m_value.radius += item->m_value;
            break;
        }
        case effect_e::start_angle: {
            m_value.start_angle += item->m_value;
            break;
        }
        case effect_e::end_angle: {
            m_value.end_angle += item->m_value;
            break;
        }
        }
    }
    }
}

bool Vision_component::update()
{
    m_value = m_basic_value;
    update_list(this);
    return false;
}

Vision_component* Vision_component::clone()
{
    auto result = new Vision_component();
    result->m_basic_value = AI_FOV(m_basic_value.radius, m_basic_value.qualifier, m_basic_value.start_angle, m_basic_value.end_angle);
    result->m_value = AI_FOV(m_value.radius, m_value.qualifier, m_value.start_angle, m_value.end_angle);
    for (auto& m_item : m_items) {
        result->add(m_item->clone());
    }
    return result;
}

void Vision_component::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

iPacker& Vision_component::get_packer()
{
    return Packer<Vision_component>::instance();
}