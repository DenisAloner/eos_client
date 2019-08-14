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
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include "Game_object_manager.h"
#include "GUI_Window_manager.h"
#include "GUI_button.h"
#include "GUI_Button_list.h"
#include "game/Game_log.h"
#include <graphics/GUI_Controller.h>

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
class Instruction_slot_parameter;

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

	std::list<GameObject*>* m_source{};
	GameObject* m_item;
	 
	game_clipboard();
};

class Application
{
public:


	int m_game_turn{};
	bool m_ready{};
	bool m_turn;

	dimension_t m_size;

	Timer* m_timer;
	Timer* m_action_animation_timer;
	MouseController* m_mouse;
	gui_MessageQueue m_message_queue;

	game_clipboard m_clipboard;

	std::mutex m_update_mutex;

	std::mutex m_turn_mutex;
	std::condition_variable m_turn_cv;

	std::mutex m_UI_mutex;

	GameObjectManager* m_game_object_manager;

	GUI_Controller m_gui_controller;
	
	GraphicalController* m_graph;
	ActionManager* m_action_manager;

	Action* m_actions[action_e::max];

	AI_manager* m_ai_manager{};
	
	Event<WPARAM> key_press;

	GUI_Window_manager* m_window_manager;

	std::list<std::function<void()>> m_update_in_render_thread;

	HDC m_hDC;
	HGLRC m_hRC;
	HGLRC subhRC;

	Game_log m_game_log;

	Game_world* m_world;

	~Application();

	static Application& instance()
	{
		static Application singleton;
		return singleton;
	}

	void initialize(dimension_t work_area_size, HDC m_hDC, HGLRC hRC);
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
	void command_set_cursor(GLuint _Tile);
	void command_set_pickup_item(GLuint _Tile);
	void command_set_cursor_visibility(bool _Visibility);
	void command_set_pickup_item_visibility(bool _Visibility);
	void command_change_owner(Instruction_slot_parameter* parameter);
	/*Parameter* command_select_transfer_source(Parameter_destination* parameter);*/
	Game_object_owner* command_select_transfer(Parameter_destination* parameter);
	Object_part* command_select_body_part();
	bool command_agreement();
	void console(std::string text);

	void on_turn();
	
	Application(const Application& root) = delete;
	Application& operator=(const Application&) = delete;
	
private:
	std::shared_ptr<std::thread> m_game_thread;
	std::shared_ptr<std::thread> m_animation_thread;

	Application();


	void process_game();

	bool check_action_completion(GameObject*& object);

	common_action_t m_common_action;
};

#endif //APPLICATION_H