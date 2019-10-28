#ifndef VISION_LIST_H
#define VISION_LIST_H

#include "interaction_list.h"

class Vision_list : public Interaction_list {
public:
    int m_max_radius;

    Vision_list();
    bool update() override;
    Vision_list* clone() override;
    void apply_visitor(Visitor_generic& visitor) override;

    void equip(Object_interaction* item) override;
    void unequip(Object_interaction* item) override;

    iPacker& get_packer() override;

    constexpr static auto properties() { return Interaction_list::properties(); }

private:
    void update_list(Object_interaction* list);
};

#endif //VISION_LIST_H
