#include "parameter_list.h"
#include "writer.h"

Parameter_list::Parameter_list(interaction_e subtype)
    : m_subtype(subtype)
{
    m_list_type = feature_list_type_e::parameter;
};

Parameter_list::Parameter_list()
{
    m_list_type = feature_list_type_e::parameter;
};

Parameter_list::Update_visitor::Update_visitor(Parameter_list& owner)
    : m_owner(owner)
{
}

void Parameter_list::Update_visitor::visit(Object_interaction& value)
{
    switch (value.get_interaction_message_type()) {
    case interaction_message_type_e::effect: {
	    auto& item = static_cast<Effect&>(value);
        switch (item.m_subtype) {
        case effect_e::value: {
            m_owner.m_value += item.m_value;
            break;
        }
        case effect_e::limit: {
            m_owner.m_limit += item.m_value;
            break;
        }
        default: {
            break;
        }
        }
        break;
    }
    default: {
        break;
    }
    }
}

bool Parameter_list::update()
{
    ////LOG(INFO) << "ÒÈÏ ÏÀÐÀÌÅÒÐÀ " << std::to_string((int)m_subtype);
    m_value = m_basic_value;
    m_limit = m_basic_limit;
    Update_visitor uh(*this);
    do_predicate(uh);
    return false;
}

Parameter_list* Parameter_list::clone()
{
	auto result = new Parameter_list(m_subtype);
    result->m_list_type = m_list_type;
    result->m_basic_limit = m_basic_limit;
    result->m_basic_value = m_basic_value;
    result->m_value = m_value;
    result->m_limit = m_limit;
    for (auto& m_item : m_items) {
        result->add(m_item->clone());
    }
    return result;
}

void Parameter_list::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
}

iPacker& Parameter_list::get_packer()
{
    return Packer<Parameter_list>::instance();
}

std::u16string Parameter_list::serialize_to_json_reference(JsonWriter& value)
{
    return value.write(*this);
}

std::u16string Parameter_list::serialize_to_json_pointer(JsonWriter& value)
{
    return value.write(this);
}
