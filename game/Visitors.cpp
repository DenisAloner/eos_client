#include "Visitors.h"
#include "GameObject.h"


void Visitor_simple::visit(GameObject& value)
{
	if (value.m_active_state)
	{
		value.m_active_state->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Attribute_map& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current)
	{
		current->second->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Object_state& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current)
	{
		current->second->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Interaction_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current)
	{
		(*current)->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Parameter_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current)
	{
		(*current)->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Vision_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current)
	{
		(*current)->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Vision_component& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current)
	{
		(*current)->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Parts_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current)
	{
		(*current)->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Object_part& value)
{
	value.m_attributes.apply_visitor(*this);
}

void Visitor_simple::visit(Tag_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current)
	{
		(*current)->apply_visitor(*this);
	}
}

void Visitor_doublehandle::visit(GameObject& value)
{
	begin(value);
	Visitor_simple::visit(value);
	end(value);
}

void Visitor_doublehandle::visit(Attribute_map& value)
{
	begin(value);
	Visitor_simple::visit(value);
	end(value);
}

void Visitor_doublehandle::visit(Object_state& value)
{
	begin(value);
	Visitor_simple::visit(value);
	end(value);
}

void Visitor_doublehandle::visit(Interaction_list& value)
{
	begin(value);
	Visitor_simple::visit(value);
	end(value);
}

void Visitor_doublehandle::visit(Parameter_list& value)
{
	begin(value);
	Visitor_simple::visit(value);
	end(value);
}

void Visitor_doublehandle::visit(Vision_list& value)
{
	begin(value);
	Visitor_simple::visit(value);
	end(value);
}

void Visitor_doublehandle::visit(Vision_component& value)
{
	begin(value);
	Visitor_simple::visit(value);
	end(value);
}

void Visitor_doublehandle::visit(Parts_list& value)
{
	begin(value);
	Visitor_simple::visit(value);
	end(value);
}

void Visitor_doublehandle::visit(Object_part& value)
{
	begin(value);
	Visitor_simple::visit(value);
	end(value);
}

void Visitor_doublehandle::visit(Tag_list& value)
{
	begin(value);
	Visitor_simple::visit(value);
	end(value);
}

void Visitor_doublehandle::begin(GameObject& value)
{
}

void Visitor_doublehandle::end(GameObject& value)
{
}

void Visitor_doublehandle::begin(Attribute_map& value)
{
}

void Visitor_doublehandle::end(Attribute_map& value)
{
}

void Visitor_doublehandle::begin(Object_state& value)
{
}

void Visitor_doublehandle::end(Object_state& value)
{
}

void Visitor_doublehandle::begin(Interaction_list& value)
{
}

void Visitor_doublehandle::end(Interaction_list& value)
{
}

void Visitor_doublehandle::begin(Parameter_list& value)
{
}

void Visitor_doublehandle::end(Parameter_list& value)
{
}

void Visitor_doublehandle::begin(Vision_list& value)
{
}

void Visitor_doublehandle::end(Vision_list& value)
{
}

void Visitor_doublehandle::begin(Vision_component& value)
{
}

void Visitor_doublehandle::end(Vision_component& value)
{
}

void Visitor_doublehandle::begin(Object_part& value)
{
}

void Visitor_doublehandle::end(Object_part& value)
{
}

void Visitor_doublehandle::begin(Parts_list& value)
{
}

void Visitor_doublehandle::end(Parts_list& value)
{
}

void Visitor_doublehandle::begin(Tag_list& value)
{
}

void Visitor_doublehandle::end(Tag_list& value)
{
}
