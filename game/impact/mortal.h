#ifndef MORTAL_H
#define MORTAL_H

#include "object_tag.h"

namespace ObjectTag {

class Mortal : public Object_tag {
public:
    Mortal();
    Mortal* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    void apply_visitor(Visitor_generic& visitor) override;

    iPacker& get_packer() override;

    /*constexpr static auto properties() { return Object_tag::properties(); }*/
};

}

#endif //MORTAL_H