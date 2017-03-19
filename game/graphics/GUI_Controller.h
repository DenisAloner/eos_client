#ifndef GUI_CONTROLLER_H
#define	GUI_CONTROLLER_H

#include <Definiton.h>
#include <Windows.h>

class ApplicationGUI;
class gui_mapviewer_hint;

class GUI_Controller
{
public:

	std::list<gui_mapviewer_hint*> m_hints;
	bool m_dragged;

	ApplicationGUI* m_GUI;
	dimension_t m_size;

	GUI_Controller();
	~GUI_Controller();

	void key_press(WPARAM w);
	void mouse_click(MouseEventArgs const& e);
	void mouse_down(MouseEventArgs const& e);
	void mouse_move(MouseEventArgs const& e);
	void mouse_wheel(MouseEventArgs const& e);
};

#endif //GUI_CONTROLLER_H