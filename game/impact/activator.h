#ifndef ACTIVATOR_H
#define ACTIVATOR_H

#include "object_tag.h"

namespace ObjectTag {

class Activator : public Object_tag {
public:
    Activator();
    std::list<GameObject*> m_link;
    Activator* clone() override;
    void apply_effect(GameObject* unit, Object_interaction* object) override;

    void reset_serialization_index() override;

    iPacker& get_packer() override;
};

}

#endif //ACTIVATOR_H