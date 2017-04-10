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

void Visitor_simple::visit(Action_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end(); ++current)
	{
		(*current)->apply_visitor(*this);
	}
}

void Visitor_simple::visit(ObjectTag::Requirements_to_object& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
	}
}