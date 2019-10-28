#ifndef TAG_LIST_H
#define TAG_LIST_H

#include "interaction_list.h"

class Tag_list : public Interaction_list {
public:
    Tag_list();

    void apply_visitor(Visitor_generic& visitor) override;

    Tag_list* clone() override;

    bool update() override;

    iPacker& get_packer() override;

    constexpr static auto properties() { return Interaction_list::properties(); }

private:
    class Update_visitor : public Visitor {
    public:
        bool was_changed;

        Update_visitor();
        void visit(Object_interaction& value) override;
    };
};

#endif //TAG_LIST_H