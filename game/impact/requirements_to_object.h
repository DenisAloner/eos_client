#ifndef REQUIREMENTS_TO_OBJECT_H
#define REQUIREMENTS_TO_OBJECT_H

#include "object_tag.h"

namespace ObjectTag {

class Requirements_to_object : public Object_tag {
public:
    Object_interaction* m_value;
    bool m_result;

    Requirements_to_object();
    Requirements_to_object* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    void apply_visitor(Visitor_generic& visitor) override;

    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Object_tag::properties(),
            std::make_tuple(
                make_property(&Requirements_to_object::m_value, u"value")));
    }

    IJSONSERIALIZABLE();
};

}

#endif //REQUIREMENTS_TO_OBJECT_H
