#ifndef APPLICATIONGUI_H
#define	APPLICATIONGUI_H

#include "Description_GUI.h"
#include "graphics/GUI_Layer.h"


class GUI_MapViewer;
class GUI_Timer;

class ApplicationGUI : public GUI_Layer
{
public:
	ApplicationGUI();
	~ApplicationGUI();

	GUI_MapViewer* MapViewer;
	GUI_TextBox* DescriptionBox;
	GUI_Timer* Timer;
};

#endif //APPLICATIONGUI_H
