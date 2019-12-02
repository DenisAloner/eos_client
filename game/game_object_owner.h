#ifndef GAME_OBJECT_OWNER_H
#define GAME_OBJECT_OWNER_H

#include "Definiton.h"

class Game_object_owner : public virtual iSerializable {
public:
    entity_e m_kind;
    Game_object_owner* m_owner;

    Game_object_owner();

    Game_object_owner* get_owner();
    Game_object_owner* get_owner(entity_e kind);

    iPacker& get_packer() override = 0;
    
    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&Game_object_owner::m_kind, u"kind"),
            make_property(&Game_object_owner::m_owner, u"owner"));
    }

    void reset_serialization_index() override;
};

#endif
