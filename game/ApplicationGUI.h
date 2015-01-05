#ifndef APPLICATIONGUI_H
#define	APPLICATIONGUI_H

#include "Description_GUI.h"
#include "graphics/GUI_Layer.h"
#include "graphics/GUI_ActionPanel.h"


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
	GUI_ActionPanel* m_action_panel;
};

#endif //APPLICATIONGUI_H
