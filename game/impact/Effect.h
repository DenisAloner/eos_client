#ifndef EFFECT_H
#define EFFECT_H

#include "Definiton.h"

class Effect : public Object_interaction {
public:
    int m_value;
    effect_e m_subtype;
    Effect();
    Effect* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Effect::m_value, u"value"),
            make_property(&Effect::m_subtype, u"subtype"));
    }

    interaction_message_type_e get_interaction_message_type() override;
    IJSONSERIALIZABLE();
};

#endif //EFFECT_H
