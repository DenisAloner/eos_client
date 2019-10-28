#ifndef PARTS_LIST_H
#define PARTS_LIST_H

#include "interaction_list.h"

class Parts_list : public Interaction_list {
public:
    Parts_list();
    Parts_list* clone() override;
    bool update() override;

    void apply_visitor(Visitor_generic& visitor) override;
    void equip(Object_interaction* item) override;

    iPacker& get_packer() override;

    constexpr static auto properties() { return Interaction_list::properties(); }

private:
    class Update_visitor : public Visitor {
    public:
        Parts_list& m_owner;
        bool was_changed;

        Update_visitor(Parts_list& owner);
        void visit(Object_interaction& value) override;
    };
};

#endif //PARTS_LIST_H