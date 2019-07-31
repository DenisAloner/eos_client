#ifndef GUI_ACTIONBUTTON_H
#define	GUI_ACTIONBUTTON_H

#include "GUI_Object.h"
#include "Application.h"

class Application;
class Action;

class GUI_ActionButton :
	public GUI_Object
{
public:

	Action_helper_t m_value;

	GUI_ActionButton(int width, int height);
	~GUI_ActionButton();

	void on_mouse_click(MouseEventArgs const& e) override;
	void on_mouse_move(MouseEventArgs const& e) override;
	void render(GraphicalController* graph, int px, int py) override;
	bool check_region(MouseEventArgs const& e) override;
};

#endif //GUI_ACTIONBUTTON_H
