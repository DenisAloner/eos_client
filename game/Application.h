#ifndef APPLICATION_H
#define	APPLICATION_H

#include "game/graphics/GUI_Inventory.h"
#include "game/graphics/GUI_MiniMap.h"
#include "game/Event.h"
#include "game/Definiton.h"
#include "game/GraphicalController.h"
#include "game/Timer.h"
#include "game/Description_GUI.h"
#include "game/Property_Container.h"
#include "game/FileSystem.h"
#include "game/TileManager.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include "Game_object_manager.h"

class Action;
class ActionManager;
class GameObject;
class Game_object_owner;
class GameMap;
class MapCell;
class Parameter;
class P_object;
class Action_OpenInventory;
class MouseController;
class GUI_ActionPanel;
class GUI_ActionButton;
class GUI_ActionManager;
class GUI_Layer;
class GUI_Object;
class GUI_Item;
class GUI_Window;
class GUI_Timer;
class GUI_MiniMap;
class Parameter_destination;
class AI_manager;
class Reaction_manager;
class GameObjectManager;

class gui_MessageQueue
{
public:

	std::mutex m_mutex;
	std::condition_variable m_condition_variable;
	std::list<Parameter*> m_items;
	bool m_read_message;
	bool m_processed_message;
	bool m_reader;
	bool m_busy;

	gui_MessageQueue();
	~gui_MessageQueue();

	void push(Parameter* p);
};

class game_clipboard
{
public:

	std::list<GameObject*>* m_source;
	GameObject* m_item;
	 
	game_clipboard();
};

class Application
{
public:

	int m_game_turn;
	bool m_ready;

	Timer* m_timer;
	MouseController* m_mouse;
	gui_MessageQueue m_message_queue;

	game_clipboard m_clipboard;

	std::mutex m_update_mutex;

	GameObjectManager* m_game_object_manager;

	ApplicationGUI* m_GUI;
	GraphicalController* m_graph;
	ActionManager* m_action_manager;

	Action* m_actions[10];

	AI_manager* m_ai_manager;
	Reaction_manager* m_reaction_manager;

	Event<WPARAM> key_press;
	Event<MouseEventArgs> mouse_click;

	Mix_Chunk *music;

	~Application(void);

	static Application& instance()
	{
		static Application Singleton;
		return Singleton;
	}

	void initialize();
	void start();
	void stop();

	void on_key_press(WPARAM w);
	void on_mouse_click(MouseEventArgs const& e);
	void on_mouse_down(MouseEventArgs const& e);
	void on_mouse_wheel(MouseEventArgs const& e);
	void on_mouse_move(MouseEventArgs const& e);
	void render();
	void update();

	Parameter* command_select_location(GameObject* object);
	Parameter* command_select_object_on_map();
	bool command_open_inventory(GameObject*& Object);
	bool command_open_body(GameObject*& Object);
	bool command_check_position(GameObject*& object, MapCell*& position, GameMap*& map);
	void command_set_cursor(GLuint _Tile);
	void command_set_pickup_item(GLuint _Tile);
	void command_set_cursor_visibility(bool _Visibility);
	void command_set_pickup_item_visibility(bool _Visibility);
	P_object* command_select_transfer_source(Parameter_destination* parameter);
	Parameter* command_select_transfer(Parameter_destination* parameter);
	
	void PlaySound1();

private:
	std::shared_ptr<std::thread> m_game_thread;
	std::shared_ptr<std::thread> m_animation_thread;

	Application();
	Application(const Application& root);
	Application& operator=(const Application&);

	void process_game();
};

#endif //APPLICATION_H
