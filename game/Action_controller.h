#ifndef ACTIONCONTROLLER_H
#define	ACTIONCONTROLLER_H

#include "Action.h"
#include "Parameter.h"

class Action_controller
{
public:


	Action_wrapper m_wrapper;
	bool done;

	Action_controller();
	~Action_controller();

	void set(GameObject* unit, Action* action, Parameter* parameter);
	void update();

private:
	Interaction_prefix m_prefix;
};

#endif //ACTIONCONTROLLER_H