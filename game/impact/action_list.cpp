#include "action_list.h"
#include "Visitors.h"

Action_list::Action_list()
{
    m_list_type = feature_list_type_e::action;
};

Interaction_list* Action_list::clone()
{
	auto result = new Action_list();
    for (auto& m_item : m_items) {
        result->m_items.push_back(m_item->clone());
    }
    return result;
}

void Action_list::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

iPacker& Action_list::get_packer()
{
    return Packer<Action_list>::instance();
}