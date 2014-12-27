#include "Object_tag.h"

Object_tag::Object_tag()
{
}

void Object_tag::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<" + m_text + ">");
}