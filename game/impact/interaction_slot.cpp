#include "interaction_slot.h"

void Interaction_slot::do_predicate(Visitor& helper)
{
    helper.visit(*this);
    m_value->do_predicate(helper);
}

void Interaction_slot::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_value && m_value->m_serialization_index != 0) {
        m_value->reset_serialization_index();
    }
}
