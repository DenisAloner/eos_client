#include "tag_list.h"
#include "Visitors.h"

Tag_list::Tag_list()
{
    m_list_type = feature_list_type_e::tag;
}

void Tag_list::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

Tag_list* Tag_list::clone()
{
    auto result = new Tag_list();
    for (auto& m_item : m_items) {
        result->m_items.push_back(m_item->clone());
    }
    return result;
}

Tag_list::Update_visitor::Update_visitor()
    : was_changed(false)
{
}

void Tag_list::Update_visitor::visit(Object_interaction& value)
{
    if (was_changed) {
        return;
    }
}

bool Tag_list::update()
{
    Update_visitor uh;
    do_predicate(uh);
    return uh.was_changed;
}

iPacker& Tag_list::get_packer()
{
    return Packer<Tag_list>::instance();
}