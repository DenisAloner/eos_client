#ifndef POISON_RESIST_H
#define POISON_RESIST_H

#include "object_tag.h"

namespace ObjectTag {
class Poison_resist : public Object_tag {
public:
    Poison_resist();
    Poison_resist* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
    IVISITABLE();
};
}

#endif //POISON_RESIST_H