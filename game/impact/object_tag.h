#ifndef OBJECT_TAG_H
#define OBJECT_TAG_H

#include "Definiton.h"

class Object_tag : public Object_interaction {
public:
    object_tag_e m_type;
    Object_tag(object_tag_e key);
    Object_tag* clone() override = 0;

    constexpr static auto properties() { return std::make_tuple(make_property(&Object_tag::m_type, u"type")); }
    interaction_message_type_e get_interaction_message_type() override;
};

#endif //OBJECT_TAG_H
