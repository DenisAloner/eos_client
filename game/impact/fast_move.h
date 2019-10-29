#ifndef FAST_MOVE_H
#define FAST_MOVE_H

#include "object_tag.h"

namespace ObjectTag {

class Fast_move : public Object_tag {
public:
    Fast_move();
    Fast_move* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
};

}

#endif //FAST_MOVE_H
