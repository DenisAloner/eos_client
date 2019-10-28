#ifndef INTERACTION_SLOT_H
#define INTERACTION_SLOT_H

#include "Definiton.h"

class Interaction_slot : public Object_interaction {
public:
    Object_interaction* m_value;
    void do_predicate(Visitor& helper) override;
    void reset_serialization_index() override;

    iPacker& get_packer() override = 0;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Interaction_slot::m_value, u"value"));
    }
};

#endif //INTERACTION_SLOT_H 
