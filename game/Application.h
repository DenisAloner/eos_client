#ifndef APPLICATION_H
#define	APPLICATION_H

#include "game/graphics/GUI_Inventory.h"
#include "game/graphics/GUI_MiniMap.h"
#include "game/Event.h"
#include "game/Definiton.h"
#include "game/GraphicalController.h"
#include "game/Timer.h"
#include "game/Description_GUI.h"
#include "game/FileSystem.h"
#include "game/TileManager.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include "Game_object_manager.h"
#include "GUI_Window_manager.h"
#include "GUI_button.h"
#include "GUI_Button_list.h"
#include "game/Game_log.h"

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
class GameObjectManager;
class Object_part;
class Object_interaction;
class GUI_Window_manager;
class Game_world;

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

	static const std::size_t c_action_size = 18;

	int m_game_turn;
	bool m_ready;
	bool m_turn;

	dimension_t m_size;

	Timer* m_timer;
	MouseController* m_mouse;
	gui_MessageQueue m_message_queue;

	game_clipboard m_clipboard;

	std::mutex m_update_mutex;

	GameObjectManager* m_game_object_manager;

	ApplicationGUI* m_GUI;
	GraphicalController* m_graph;
	ActionManager* m_action_manager;

	Action* m_actions[c_action_size];

	AI_manager* m_ai_manager;
	
	Event<WPARAM> key_press;
	Event<MouseEventArgs> mouse_click;

	Mix_Chunk *music;

	GUI_Window_manager* m_window_manager;

	std::list<GUI_Object*> m_update_canvas;

	Game_log m_game_log;

	Game_world* m_world;

	~Application(void);

	static Application& instance()
	{
		static Application Singleton;
		return Singleton;
	}

	void initialize(dimension_t work_area_size);
	void start();
	void stop();

	void on_key_press(WPARAM w);
	void on_mouse_click(MouseEventArgs const& e);
	void on_mouse_down(MouseEventArgs const& e);
	void on_mouse_wheel(MouseEventArgs const& e);
	void on_mouse_move(MouseEventArgs const& e);
	void render();
	void update();
	void update_after_load();

	void new_game();
	void load_game();

	MapCell* command_select_location(GameObject* object);
	GameObject* command_select_object_on_map();
	GameObject* command_select_object();
	void command_main_menu_select();
	bool command_open_body(GameObject*& Object);
	void command_gui_show_characterization(GameObject*& object);
	bool command_check_position(GameObject*& object, MapCell*& position, GameMap*& map);
	void command_equip(GameObject*& unit, Object_part* part, GameObject*& object);
	void command_unequip(GameObject*& unit, Object_part* part, GameObject*& object);
	void command_set_cursor(GLuint _Tile);
	void command_set_pickup_item(GLuint _Tile);
	void command_set_cursor_visibility(bool _Visibility);
	void command_set_pickup_item_visibility(bool _Visibility);
	/*Parameter* command_select_transfer_source(Parameter_destination* parameter);*/
	Game_object_owner* command_select_transfer(Parameter_destination* parameter);
	Object_part* command_select_body_part();
	bool command_agreement();
	void console(std::string text);

	void on_turn();
	
	void PlaySound1();

private:
	std::shared_ptr<std::thread> m_game_thread;
	std::shared_ptr<std::thread> m_animation_thread;

	Application();
	Application(const Application& root);
	Application& operator=(const Application&);

	void process_game();

	bool check_action_completion(GameObject*& object);

	common_action_t m_common_action;
};

#endif //APPLICATION_H
