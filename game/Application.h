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


class Action;
class ActionManager;
class GameObject;
class GameMap;
class MapCell;
class Parameter;
class Action_OpenInventory;
class MouseController;
class GUI_ActionPanel;
class GUI_ActionButton;
class GUI_ActionManager;
class GUI_Layer;
class GUI_Object;
class GUI_PopMenu;
class GUI_Item;
class GUI_Window;
class GUI_Timer;
class GUI_MiniMap;


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

class GameObjectManager
{
public:

	enum command_e
	{
		obj,
		size,
		layer,
		tile_manager_single,
		tile_manager_map,
		tile_manager_rotating,
		light,
		action_move,
		property_permit_move,
		property_container
	};

	typedef std::map<std::string, GameObject*> items_t;
	typedef std::map<std::string, command_e> commands_t;
	items_t m_items;
	commands_t m_commands;

	GameObject* m_object;

	void parser(const std::string& command);
	GameObject* new_object(std::string unit_name);

	void init();

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

	GameObjectManager m_game_object_manager;

	ApplicationGUI* m_GUI;
	GraphicalController* m_graph;
	ActionManager* m_action_manager;

	Action* m_actions[4];

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

	bool command_select_location(GameObject* Object, MapCell*& Cell);
	bool command_select_object(GameObject*& Object);
	bool command_open_inventory(GameObject*& Object);
	bool command_check_position(GameObject*& object, MapCell*& position, GameMap*& map);
	void command_set_cursor(GLuint _Tile);
	void command_set_pickup_item(GLuint _Tile);
	void command_set_cursor_visibility(bool _Visibility);
	void command_set_pickup_item_visibility(bool _Visibility);
	
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
