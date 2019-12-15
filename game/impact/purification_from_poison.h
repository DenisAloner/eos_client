#ifndef PURIFICATION_FROM_POISON_H
#define PURIFICATION_FROM_POISON_H

#include "object_tag.h"

namespace ObjectTag {
	
class Purification_from_poison : public Object_tag {
public:
    Purification_from_poison();
    Purification_from_poison* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
    IJSONSERIALIZABLE();
};
	
}

#endif //PURIFICATION_FROM_POISON_H