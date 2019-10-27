#ifndef APPLICATIONGUI_H
#define APPLICATIONGUI_H

#include "ActionManager.h"
#include "GUI_Action_bar.h"
#include "Game_log.h"
#include "graphics/GUI_Layer.h"

class GUI_MapViewer;
class Player;
class GUI_Action_bar;
class GameMap;

class ApplicationGUI : public GUI_Layer {
public:
    ApplicationGUI(int x, int y, int width, int height);
    ApplicationGUI(int x, int y, int width, int height, Player* p, GameMap* m, ActionManager* am, Game_log& gl);
    ~ApplicationGUI();

    GUI_MapViewer* map_viewer;
    GUI_Layer* m_main_layer;
    GUI_Action_bar* m_action_bar;
};

#endif //APPLICATIONGUI_H
