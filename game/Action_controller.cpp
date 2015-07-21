#include "Action_controller.h"

Action_controller::Action_controller()
{
	m_action = nullptr;
	m_parameter = nullptr;
	m_decay = 0;
}


Action_controller::~Action_controller()
{
}

void Action_controller::set(Action* action, Parameter* parameter)
{
	m_action = action;
	m_parameter = parameter;
	m_decay = m_action->m_decay;
}

void Action_controller::update()
{
	if (m_action)
	{
		m_decay -= 1;
		if (m_decay <= 0)
		{
			m_action->perfom(m_parameter);
			/*if (m_action->m_error != "")
			{
				m_GUI->DescriptionBox->add_item_control(new GUI_Text(A->m_action->m_error));
			}*/
			m_action = nullptr;
		}
	}
}