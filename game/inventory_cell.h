#ifndef INVENTORY_CELL_H
#define INVENTORY_CELL_H

#include "game_object_owner.h"

class GameObject;

class InventoryCell : public Game_object_owner {
public:
    GameObject* m_item;
    explicit InventoryCell(GameObject* item = nullptr);

    void reset_serialization_index() override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return std::make_tuple(
            make_property(&InventoryCell::m_kind, u"kind"),
            make_property(&InventoryCell::m_owner, u"owner"),
            make_property(&InventoryCell::m_item, u"item"));
    }

    IVISITABLE();
};


#endif
