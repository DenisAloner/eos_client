#include "label.h"
#include "writer.h"

ObjectTag::Label::Label(object_tag_e type)
    : Object_tag(type) {};

ObjectTag::Label* ObjectTag::Label::clone()
{
	const auto effect = new ObjectTag::Label(m_type);
    return effect;
}

void ObjectTag::Label::apply_effect(GameObject* unit, Object_interaction* object) {}

void ObjectTag::Label::apply_visitor(Visitor_generic& visitor)
{
    visitor.visit(*this);
};

iPacker& ObjectTag::Label::get_packer()
{
    return Packer<Label>::instance();
}

std::u16string ObjectTag::Label::serialize_to_json_reference(JsonWriter& value)
{
    return value.write(*this);
}

std::u16string ObjectTag::Label::serialize_to_json_pointer(JsonWriter& value)
{
    return value.write(this);
}
