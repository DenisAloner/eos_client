#ifndef INSTRUCTION_GET_OWNER_H
#define INSTRUCTION_GET_OWNER_H

#include "Definiton.h"

class Instruction_get_owner : public Object_interaction {
public:
    Object_interaction* m_value;
    bool m_result;

    Instruction_get_owner();
    Instruction_get_owner* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;
    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Instruction_get_owner::m_value, u"value"));
    }

    interaction_message_type_e get_interaction_message_type() override;
    std::u16string serialize_to_json_reference(JsonWriter& value) override;
    std::u16string serialize_to_json_pointer(JsonWriter& value) override;
};

#endif //INSTRUCTION_GET_OWNER_H
