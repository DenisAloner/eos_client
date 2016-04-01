#ifndef APPLICATIONGUI_H
#define	APPLICATIONGUI_H

#include "Description_GUI.h"
#include "graphics/GUI_Layer.h"
#include "graphics/GUI_ActionPanel.h"


class GUI_MapViewer;

class ApplicationGUI : public GUI_Layer
{
public:

	ApplicationGUI(int x, int y, int width, int height);
	~ApplicationGUI();

	GUI_MapViewer* MapViewer;
	GUI_TextBox* DescriptionBox;
	GUI_ActionPanel* m_action_panel;
};

#endif //APPLICATIONGUI_H
