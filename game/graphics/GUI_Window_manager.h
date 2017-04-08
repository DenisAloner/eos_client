#ifndef GUI_WINDOW_MANAGER_H
#define	GUI_WINDOW_MANAGER_H

#include "GUI_Window.h"

class GUI_Window_manager :
	public GUI_Layer
{
public:

	GUI_Window_manager(int x, int y, int width, int height);
	~GUI_Window_manager();

	void on_item_get_focus(GUI_Object* sender) override;

};

#endif // GUI_WINDOW_MANAGER_H