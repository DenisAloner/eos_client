#ifndef ACTIONCONTROLLER_H
#define	ACTIONCONTROLLER_H

#include "Action.h"
#include "Parameter.h"

class Action_controller
{
public:

	int m_decay;
	Action* m_action;
	Parameter* m_parameter;

	Action_controller();
	~Action_controller();

	void set(Action* action,Parameter* parameter);
	void update();
};

#endif //ACTIONCONTROLLER_H