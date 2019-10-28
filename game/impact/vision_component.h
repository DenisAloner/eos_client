#ifndef VISION_COMPONENT_H
#define VISION_COMPONENT_H

#include "interaction_list.h"

class Vision_component : public Interaction_list {
public:
    AI_FOV m_basic_value;
    AI_FOV m_value;
    Vision_component();
    bool update() override;
    Vision_component* clone() override;
    void apply_visitor(Visitor_generic& visitor) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Interaction_list::properties(),
            std::make_tuple(make_property(&Vision_component::m_basic_value, u"basic_value")));
    }

private:
    void update_list(Object_interaction* list);
};

#endif //VISION_COMPONENT_H
