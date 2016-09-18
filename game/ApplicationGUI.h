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
#include "GUI_Action_bar.h"


class GUI_MapViewer;
class GUI_ActionPanel;
class Player;
class GUI_Action_bar;

class ApplicationGUI : public GUI_Layer
{
public:

	ApplicationGUI(int x, int y, int width, int height);
	ApplicationGUI(int x, int y, int width, int height, Player* p, GameMap* m,ActionManager* am, Game_log& gl);
	~ApplicationGUI();

	GUI_MapViewer* MapViewer;
	GUI_Layer* m_main_layer;
	GUI_ActionPanel* m_action_panel;
	GUI_Action_bar* m_action_bar;
};

#endif //APPLICATIONGUI_H
