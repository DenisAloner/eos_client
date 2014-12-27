#pragma once
#include "Effect.h"

class Object_tag :public Object_interaction
{
public:
	std::string m_text;
	Object_tag();
	virtual void description(std::list<std::string>* info, int level);
};

