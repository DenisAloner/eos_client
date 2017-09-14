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

void Visitor_simple::visit(Instruction_slot_link& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Interaction_copyist& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Interaction_prefix& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Interaction_addon& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Interaction_time& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Interaction_timer& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
	}
}

void Visitor_simple::visit(Instruction_slot_parameter& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
	}
}

void Visitor_generic_updater::visit(GameObject& value)
{
	if (value.m_active_state)
	{
		value.m_active_state->apply_visitor(*this);
		if (result) { value.m_active_state = nullptr; }
	}
	result = false;
}

void Visitor_generic_updater::visit(Attribute_map& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end();)
	{
		result = false;
		current->second->apply_visitor(*this);
		if (result)
		{
			current = value.m_items.erase(current);
		}
		else
			++current;
	}
	result = value.m_items.empty();
}

void Visitor_generic_updater::visit(Object_state& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end();)
	{
		result = false;
		current->second->apply_visitor(*this);
		if (result)
		{
			current = value.m_items.erase(current);
		}
		else
			++current;
	}
	result = value.m_items.empty();
}

void Visitor_generic_updater::visit(Interaction_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end();)
	{
		result = false;
		(*current)->apply_visitor(*this);
		if (result)
		{
			current = value.m_items.erase(current);
		}
		else
			++current;
	}
	result = value.m_items.empty();
}

void Visitor_generic_updater::visit(Parameter_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end();)
	{
		result = false;
		(*current)->apply_visitor(*this);
		if (result)
		{
			current = value.m_items.erase(current);
		}
		else
			++current;
	}
	result = value.m_items.empty();
}

void Visitor_generic_updater::visit(Vision_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end();)
	{
		result = false;
		(*current)->apply_visitor(*this);
		if (result)
		{
			current = value.m_items.erase(current);
		}
		else
			++current;
	}
	result = value.m_items.empty();
}

void Visitor_generic_updater::visit(Vision_component& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end();)
	{
		result = false;
		(*current)->apply_visitor(*this);
		if (result)
		{
			current = value.m_items.erase(current);
		}
		else
			++current;
	}
	result = value.m_items.empty();
}

void Visitor_generic_updater::visit(Parts_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end();)
	{
		result = false;
		(*current)->apply_visitor(*this);
		if (result)
		{
			current = value.m_items.erase(current);
		}
		else
			++current;
	}
	//result = value.m_items.empty();
	result = false;
}

void Visitor_generic_updater::visit(Object_part& value)
{
	value.m_attributes.apply_visitor(*this);
}

void Visitor_generic_updater::visit(Tag_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end();)
	{
		result = false;
		(*current)->apply_visitor(*this);
		if (result)
		{
			current = value.m_items.erase(current);
		}
		else
			++current;
	}
	result = value.m_items.empty();
}

void Visitor_generic_updater::visit(Action_list& value)
{
	for (auto current = value.m_items.begin(); current != value.m_items.end();)
	{
		result = false;
		(*current)->apply_visitor(*this);
		if (result)
		{
			current = value.m_items.erase(current);
		}
		else
			++current;
	}
	result = value.m_items.empty();
}

void Visitor_generic_updater::visit(ObjectTag::Requirements_to_object& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
		if (result) { value.m_value = nullptr; }
	}
	result=value.m_value;
}

void Visitor_generic_updater::visit(Instruction_slot_link& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
		if (result) { value.m_value = nullptr; }
	}
	result = value.m_value;
}

void Visitor_generic_updater::visit(Interaction_copyist& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
		if (result) { value.m_value = nullptr; }
	}
	result = value.m_value;
}

void Visitor_generic_updater::visit(Interaction_prefix& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
		if (result) { value.m_value = nullptr; }
	}
	result = value.m_value;
}

void Visitor_generic_updater::visit(Interaction_addon& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
		if (result) { value.m_value = nullptr; }
	}
	result = value.m_value;
}

void Visitor_generic_updater::visit(Interaction_time& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
		if (result) { value.m_value = nullptr; }
	}
	result = value.m_value;
}

void Visitor_generic_updater::visit(Interaction_timer& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
		if (result) { value.m_value = nullptr; }
	}
	result = value.m_value;
}

void Visitor_generic_updater::visit(Instruction_slot_parameter& value)
{
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
		if (result) { value.m_value = nullptr; }
	}
	result = value.m_value;
}

void Visitor_onturn_updater::visit(Interaction_time& value)
{
	value.m_turn -= 1;
	if (value.m_value)
	{
		if (value.m_turn != 0)
		{
			value.m_value->apply_visitor(*this);
			if (result) { value.m_value = nullptr; }
		}
		else
		{
			value.m_value = nullptr;
		}
	}
	result = value.m_value;
}

void Visitor_onturn_updater::visit(Interaction_timer& value)
{
	value.m_turn += 1;
	if (value.m_turn > value.m_period)
	{
		value.m_turn = 0;
	}
	if (value.m_value)
	{
		value.m_value->apply_visitor(*this);
	}
	result = value.m_value;
}
