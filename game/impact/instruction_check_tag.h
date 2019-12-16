#ifndef INSTRUCTION_CHECK_TAG_H
#define INSTRUCTION_CHECK_TAG_H

#include "GameObject.h"

class Instruction_check_tag : public Object_interaction {
public:
    object_tag_e m_value;

    Instruction_check_tag();
    Instruction_check_tag* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    void apply_visitor(Visitor_generic& visitor) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Instruction_check_tag::m_value, u"value"));
    }

    interaction_message_type_e get_interaction_message_type() override;
	
    IVISITABLE();
};


#endif //INSTRUCTION_CHECK_TAG_H
