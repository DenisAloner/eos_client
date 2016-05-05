#ifndef APPLICATIONGUI_H
#define	APPLICATIONGUI_H

#include "Description_GUI.h"
#include "graphics/GUI_Layer.h"
#include "graphics/GUI_ActionPanel.h"
#include "GameObject.h"
#include "GameMap.h"
#include "GUI_ActionPanel.h"
#include "GUI_ActionManager.h"
#include "ActionManager.h"
#include "Game_log.h"


class GUI_MapViewer;
class GUI_ActionPanel;
class Player;

class ApplicationGUI : public GUI_Layer
{
public:

	ApplicationGUI(int x, int y, int width, int height);
	ApplicationGUI(int x, int y, int width, int height, Player* p, GameMap* m,ActionManager* am, Game_log& gl);
	~ApplicationGUI();

	GUI_MapViewer* MapViewer;
	//GUI_TextBox* DescriptionBox;
	GUI_ActionPanel* m_action_panel;
};

#endif //APPLICATIONGUI_H
