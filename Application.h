#pragma once

#include "Event.h"
#include "Definiton.h"
#include "GUI_Object.h"
#include "GUI_Layer.h"
#include "TActionManager.h"
#include <mutex>
#include "GraphicalController.h"
#include "GameObject.h"
#include "GameMap.h"
#include <condition_variable>
#include "TParameter.h"
#include "Timer.h"
#include "GUI_ActionPanel.h"
#include "GUI_ActionButton.h"
#include "GUI_ActionManager.h"
#include "GUI_PopMenu.h"
#include "MouseController.h"
#include "Description_GUI.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "GUI_Item.h"
#include "GUI_Window.h"
#include "GUI_Timer.h"
#include "Property_Container.h"
#include "GUI_Inventory.h"

class TAction;
class TActionManager;
class GameObject;
class MapCell;
class TParameter;
class GUI_ActionPanel;
class GUI_ActionButton;
class GUI_ActionManager;
class GUI_Layer;
class GUI_Object;
class GUI_PopMenu;
class GameMap;
class GUI_Item;
class GUI_Window;
class TGCButton;
class GUI_Timer;
class Action_OpenInventory;
class MouseController;


class gui_MessageQueue
{
public:

	std::mutex m_mutex;
	std::condition_variable m_condition_variable;
	std::list<TParameter*> m_items;
	bool m_read_message;
	bool m_processed_message;
	bool m_reader;
	bool m_busy;

	gui_MessageQueue();
	~gui_MessageQueue();

	void push(TParameter* p);
};

class Application
{
public:

	int m_game_turn;
	bool m_ready;

	Timer* m_timer;
	MouseController* m_mouse;
	gui_MessageQueue m_message_queue;

	std::mutex m_update_mutex;

	ApplicationGUI* m_GUI;
	GraphicalController* m_graph;
	TActionManager* m_action_manager;

	TAction* m_actions[4];

	Event<WPARAM> key_press;
	Event<MouseEventArgs> mouse_click;
	Event<MouseEventArgs> under_cursor;

	Mix_Chunk *music;

	~Application(void);

	static Application& instance()
	{
		static Application Singleton;
		return Singleton;
	}

	void initialization(HWND _hWnd);
	void on_key_press(WPARAM w);
	void on_mouse_click(MouseEventArgs const& e);
	void on_mouse_down(MouseEventArgs const& e);
	void on_mouse_wheel(MouseEventArgs const& e);
	void on_mouse_move(MouseEventArgs const& e);
	void on_under_cursor(MouseEventArgs const& e);
	void render();
	void update(void);

	bool command_select_location(GameObject* Object, MapCell*& Cell);
	bool command_select_object(GameObject*& Object);
	bool command_open_inventory(GameObject*& Object);
	bool command_check_position(GameObject*& _Object, MapCell*& _Position,GameMap*& _Map);
	void command_set_cursor(GLuint _Tile);
	void command_set_pickup_item(GLuint _Tile);
	void command_set_cursor_visibility(bool _Visibility);
	void command_set_pickup_item_visibility(bool _Visibility);
	
	void PlaySound1();

private:
	Application();
	Application(const Application& root);
	Application& operator=(const Application&);
	
};

