#ifndef ACTION_LIST_H
#define ACTION_LIST_H

#include "interaction_list.h"

class Action_list : public Interaction_list {
public:
    Action_list();
    Interaction_list* clone() override;

    void apply_visitor(Visitor_generic& visitor) override;

    iPacker& get_packer() override;

    constexpr static auto properties() { return Interaction_list::properties(); }

	IJSONSERIALIZABLE();
};

#endif //ACTION_LIST_H