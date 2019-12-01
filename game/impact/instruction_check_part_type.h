#ifndef INSTRUCTION_CHECK_PART_TYPE_H
#define INSTRUCTION_CHECK_PART_TYPE_H

#include "GameObject.h"

class Instruction_check_part_type : public Object_interaction {
public:
    body_part_e m_value;

    Instruction_check_part_type();
    Instruction_check_part_type* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Instruction_check_part_type::m_value, u"value"));
    }

    interaction_message_type_e get_interaction_message_type() override;
    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

#endif //INSTRUCTION_CHECK_PART_TYPE_H
