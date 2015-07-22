#include "Action_controller.h"

Action_controller::Action_controller()
{
	m_prefix.m_interaction_message_type = interaction_message_type_e::single;
	m_prefix.m_subtype = effect_prefix_e::action;
	m_prefix.m_value = &m_wrapper;
}


Action_controller::~Action_controller()
{
}

void Action_controller::set(GameObject* unit, Action* action, Parameter* parameter)
{
	m_wrapper.m_action = action;
	m_wrapper.m_parameter = parameter;
	m_wrapper.m_decay = m_wrapper.m_action->m_decay;
	m_prefix.apply_effect(unit, &m_wrapper);
}

void Action_controller::update()
{
	if (m_wrapper.m_action)
	{
		m_wrapper.m_decay -= 1;
		if (m_wrapper.m_decay <= 0)
		{
			m_wrapper.m_action->perfom(m_wrapper.m_parameter);
			/*if (m_action->m_error != "")
			{
				m_GUI->DescriptionBox->add_item_control(new GUI_Text(A->m_action->m_error));
			}*/
			m_wrapper.m_action = nullptr;
		}
	}
}