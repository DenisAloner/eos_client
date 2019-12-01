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

    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
    /*constexpr static auto properties() { return Object_tag::properties(); }*/
};

}

#endif //MORTAL_H