#ifndef INSTRUCTION_GAME_OWNER_H
#define INSTRUCTION_GAME_OWNER_H

#include "GameObject.h"

class Instruction_game_owner : public Object_interaction {
public:
    Game_object_owner* m_value;
    bool m_result;

    Instruction_game_owner();
    Object_interaction* clone() override;;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    iPacker& get_packer() override;
    interaction_message_type_e get_interaction_message_type() override;
    IJSONSERIALIZABLE();
};


#endif //INSTRUCTION_GAME_OWNER_H
