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
    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};
}

#endif //POISON_RESIST_H