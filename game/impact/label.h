#ifndef LABEL_H
#define LABEL_H

#include "object_tag.h"

namespace ObjectTag {

class Label : public Object_tag {
public:
    Label()
        : Object_tag(object_tag_e::none) {};
    Label(object_tag_e type);
    Label* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    void apply_visitor(Visitor_generic& visitor) override;

    iPacker& get_packer() override;

    constexpr static auto properties() { return Object_tag::properties(); }
    IVISITABLE();
};

}

#endif //LABEL_H
