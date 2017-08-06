#include "game/Application.h"
#include "game/MouseController.h"
#include "game/ActionManager.h"
#include "game/GameObject.h"
#include "game/graphics/GUI_ActionManager.h"
#include "game/graphics/GUI_ActionButton.h"
#include "game/graphics/GUI_Window.h"
#include "game/graphics/GUI_MapViewer.h"
#include "game/graphics/GUI_TextBox.h"
#include "game/graphics/GUI_Timer.h"
#include "utils/log.h"
#include <chrono>
#include <graphics/GUI_Description_window.h>

void my_audio_callback(void *userdata, uint8_t *stream, uint32_t len);

// variable declarations
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

gui_MessageQueue::gui_MessageQueue()
{
	m_reader = false;
	m_read_message = false;
	m_processed_message = false;
	m_busy = false;
}
gui_MessageQueue::~gui_MessageQueue()
{
}

void gui_MessageQueue::push(Parameter* p)
{
	std::unique_lock<std::mutex> lk(m_mutex);
	m_read_message = true;
	m_items.push_back(p);
	m_condition_variable.notify_one();
	while (!m_processed_message)
	{
		m_condition_variable.wait(lk);
	}
	m_read_message = false;
	m_processed_message = false;
	m_items.pop_front();
}

game_clipboard::game_clipboard():m_item(nullptr){}

Application::Application()
: m_timer(new Timer(24, 100)), m_action_animation_timer(new Timer(24, 100))
{
	m_turn = false;
}

Application::~Application(void)
{
	stop();
}

void Application::on_key_press(WPARAM w)
{
	m_update_mutex.lock();
	m_gui_controller.key_press(w);
	m_update_mutex.unlock();
}

void Application::on_mouse_click(MouseEventArgs const& e)
{
	m_update_mutex.lock();
	m_gui_controller.mouse_click(e);
	m_update_mutex.unlock();
}

void Application::on_mouse_down(MouseEventArgs const& e)
{
	m_update_mutex.lock();
	m_gui_controller.mouse_down(e);
	m_update_mutex.unlock();
}

void Application::on_mouse_move(MouseEventArgs const& e)
{
	m_update_mutex.lock();
	m_gui_controller.mouse_move(e);
	m_update_mutex.unlock();
}

void Application::on_mouse_wheel(MouseEventArgs const& e)
{
	m_update_mutex.lock();
	m_gui_controller.mouse_wheel(e);
	m_update_mutex.unlock();
}

void Application::render()
{
	m_update_mutex.lock();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	std::thread::id this_id = std::this_thread::get_id();
	std::hash<std::thread::id> hasher;
	
	//OutputDebugString((std::to_string(hasher(this_id)) + " render\n").c_str());
	
	for (auto current = m_update_in_render_thread.begin(); current != m_update_in_render_thread.end(); ++current)
	{
		(*current)();
	}
	m_update_in_render_thread.clear();

	//if (Application::instance().m_GUI->MapViewer->m_map->m_update)
	//{
	//	Application::instance().m_GUI->MapViewer->m_map->update(VoidEventArgs());
	//	Application::instance().m_GUI->MapViewer->m_map->m_update = false;
	//}
	if (m_gui_controller.m_GUI)
	{
		m_gui_controller.m_GUI->render(m_graph, 0, 0);
	}
	const position_t mouse = Application::instance().m_mouse->get_mouse_position();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	if (m_mouse->m_show_cursor)
	{
		if (m_clipboard.m_item)
		{
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, m_clipboard.m_item->m_active_state->m_icon);
			//m_graph->draw_sprite(mouse.x - 32, mouse.y - 32, mouse.x - 32, mouse.y + 32, mouse.x + 32, mouse.y + 32, mouse.x + 32, mouse.y - 32);
			GraphicalController::rectangle_t rect(mouse.x - 32, mouse.y - 32, 64, 64);
			m_graph->draw_sprite(rect);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glColor4f(1.0, 0.9, 0.0, 0.75);
			glBegin(GL_LINES);
			glVertex2d(mouse.x - 32, mouse.y - 32);
			glVertex2d(mouse.x - 32, mouse.y + 32);
			glVertex2d(mouse.x - 32, mouse.y + 32);
			glVertex2d(mouse.x + 32, mouse.y + 32);
			glVertex2d(mouse.x + 32, mouse.y + 32);
			glVertex2d(mouse.x + 32, mouse.y - 32);
			glVertex2d(mouse.x + 32, mouse.y - 32);
			glVertex2d(mouse.x - 32, mouse.y - 32);
			glEnd();
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
		}
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, m_mouse->m_cursor);
		GraphicalController::rectangle_t rect(mouse.x, mouse.y, 48, 48);
		m_graph->draw_sprite(rect);

		/*glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		m_graph->draw_rectangle(rect);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);*/
	}
	m_update_mutex.unlock();
}

void Application::initialize(dimension_t work_area_size, HDC m_hDC, HGLRC hRC)
{
	this->m_hDC = m_hDC;
	this->m_hRC = hRC;
	subhRC = wglCreateContext(m_hDC);
	wglShareLists(this->m_hRC, subhRC);
	m_size = work_area_size;
	m_gui_controller.m_size = m_size;
	music = NULL;
	m_game_turn = 1;
	m_ready = false;
	m_graph = new GraphicalController(m_size);
	m_mouse = new MouseController();
	command_set_cursor(m_graph->m_cursor);
	command_set_cursor_visibility(true);

	key_press += std::bind(&Application::on_key_press, this, std::placeholders::_1);
	m_mouse->mouse_click += std::bind(&Application::on_mouse_click, this, std::placeholders::_1);
	m_mouse->mouse_down += std::bind(&Application::on_mouse_down, this, std::placeholders::_1);
	m_mouse->mouse_wheel += std::bind(&Application::on_mouse_wheel, this, std::placeholders::_1);
	m_mouse->mouse_move += std::bind(&Application::on_mouse_move, this, std::placeholders::_1);
	m_action_manager = new ActionManager();

	m_actions[action_e::move] = new ActionClass_Move();
	m_actions[action_e::move_step] = new action_move_step();
	m_actions[action_e::push] = new ActionClass_Push();
	m_actions[action_e::turn] = new ActionClass_Turn();
	m_actions[action_e::open_inventory] = new Action_OpenInventory();
	m_actions[action_e::cell_info] = new Action_CellInfo();
	m_actions[action_e::set_motion_path] = new action_set_motion_path();
	m_actions[action_e::pick] = new Action_pick();
	m_actions[action_e::open] = new Action_open();
	m_actions[action_e::hit] = new Action_hit();
	m_actions[action_e::hit_melee] = new action_hit_melee();
	m_actions[action_e::equip] = new Action_equip();
	m_actions[action_e::show_parameters] = new Action_show_parameters();
	m_actions[action_e::use] = new Action_use();
	m_actions[action_e::save] = new Action_save();
	m_actions[action_e::autoexplore] = new Action_autoexplore();
	m_actions[action_e::shoot] = new Action_shoot();
	m_actions[action_e::load] = new Action_load();
	m_actions[action_e::move_out] = new Action_move_out();

	for (size_t i = 0; i < action_e::max; i++)
	{
		m_actions[i]->m_index = i;
	}

	m_ai_manager = new AI_manager();
	m_game_object_manager = new GameObjectManager();
	m_game_object_manager->init();
	LOG(INFO) << "Менеджер игровых объектов успешно инициализирован";

	m_gui_controller.m_GUI = new ApplicationGUI(0, 0, m_size.w, m_size.h);
	m_window_manager = new GUI_Window_manager(0, 0, m_size.w, m_size.h);
	m_gui_controller.m_GUI->add(m_window_manager);
	m_gui_controller.m_GUI->add(new GUI_Image((m_size.w - 1024) / 2, (m_size.h - 1024) / 2, 1024, 1024, m_graph->m_logo));

	GUI_Window* MainMenu = new GUI_Window(0, 0, 400, 400, u"Главное меню");
	MainMenu->m_position = position_t((m_size.w - MainMenu->m_size.w)/2, (m_size.h - MainMenu->m_size.h) / 2);
	GUI_Button_list* menu = new GUI_Button_list(0, 0, 400, 400);
	GUI_Mainmenu_button* button;
	button = new GUI_Mainmenu_button(0, 0, 396, 47, u"Новая игра", parameter_type_e::new_game);
	button->mouse_click += std::bind(&GUI_Window::on_close, MainMenu, MainMenu);
	menu->add_item_control(button);
	button = new GUI_Mainmenu_button(0, 0, 396, 47, u"Загрузка игры", parameter_type_e::load_game);
	button->mouse_click += std::bind(&GUI_Window::on_close, MainMenu, MainMenu);
	menu->add_item_control(button);
	button = new GUI_Mainmenu_button(0, 0, 396, 47, u"Выход", parameter_type_e::game_quit);
	button->mouse_click += std::bind(&GUI_Window::on_close, MainMenu, MainMenu);
	menu->add_item_control(button);
	MainMenu->add(menu);

	m_graph->set_VSync(false);
	
	m_ready = true;
	start();
}


void Application::new_game()
{
	m_world = new Game_world();
	GameMap* map = new GameMap(dimension_t(40, 40));
	map->generate_room();
	m_world->m_maps.push_back(map);
	int rx = 10;
	int ry = 10;
	GameObject* obj;
	obj = m_game_object_manager->new_object("human");
	map->add_to_map(obj, map->m_items[ry][rx]);
	m_world->m_player = new Player(obj, map);
	m_window_manager = new GUI_Window_manager(0, 0, m_size.w, m_size.h);

	obj->reset_serialization_index();
	std::string json = Parser::UTF16_to_CP866(Parser::to_json<GameObject*>(obj));
	LOG(INFO) << json;

	m_gui_controller.m_GUI = new ApplicationGUI(0, 0, m_size.w, m_size.h, m_world->m_player, map, m_action_manager, m_game_log);

	GUI_Window* MiniMap = new GUI_Window(0, 0, 400, 400, u"Мини-карта");
	rectangle_t cr = MiniMap->client_rect();
	GUI_MiniMap* mini_map = new GUI_MiniMap(position_t(0, 0), dimension_t(cr.w, cr.h), m_gui_controller.m_GUI->MapViewer);
	MiniMap->add(mini_map);

	MiniMap = new GUI_Window(300, 0, 400, 400, u"Поле зрения player");
	cr = MiniMap->client_rect();
	GUI_FOV* fov = new GUI_FOV(position_t(0, 0), dimension_t(cr.w, cr.h), m_world->m_player->m_object);
	MiniMap->add(fov);

	/*obj = m_game_object_manager->new_object("bat");
	obj->set_direction(object_direction_e::top);
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 8][rx-2]);

	obj = m_game_object_manager->new_object("bat");
	obj->set_direction(object_direction_e::top);
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 8][rx - 4]);*/


		/*obj = m_game_object_manager->new_object("bow");
		obj->set_direction(object_direction_e::top);
		m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry-1][rx - 1]);

		obj = m_game_object_manager->new_object("arrow");
		obj->set_direction(object_direction_e::top);
		m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 1][rx - 1]);*/


			/*obj = m_game_object_manager->new_object("blue potion");
			obj->set_direction(object_direction_e::top);
			m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry-2][rx]);*/

			obj = m_game_object_manager->new_object("bat");
			obj->set_direction(object_direction_e::top);
			map->add_to_map(obj, map->m_items[ry - 2][rx-2]);

		
			obj = m_game_object_manager->new_object("bag");
			obj->set_direction(object_direction_e::top);
			map->add_to_map(obj, map->m_items[ry - 2][rx-1]);

		
			obj = m_game_object_manager->new_object("ring");
			obj->set_direction(object_direction_e::top);
			map->add_to_map(obj, map->m_items[ry - 2][rx + 1]);

		
			obj = m_game_object_manager->new_object("boot");
			obj->set_direction(object_direction_e::top);
			map->add_to_map(obj, map->m_items[ry - 2][rx + 2]);

			obj = m_game_object_manager->new_object("fountain");
			obj->set_direction(object_direction_e::top);
			map->add_to_map(obj, map->m_items[ry + 10][rx + 2]);

			
	
			/*MiniMap = new GUI_Window(500, 0, 400, 400, "Поле зрения bat");
			fov = new GUI_FOV(position_t(5, 30), dimension_t(MiniMap->m_size.w - 10, MiniMap->m_size.h - 35), obj);
			MiniMap->add(fov);*/

			/*MenuLayer->add(m_GUI->Timer);
			MenuLayer->add(MiniMap);*/

	update_after_load();
}

void Application::load_game()
{

	m_world = Serialization_manager::instance().load("save");
	m_window_manager = new GUI_Window_manager(0, 0, m_size.w, m_size.h);
	m_gui_controller.m_GUI = new ApplicationGUI(0, 0, m_size.w, m_size.h, m_world->m_player, m_world->m_maps.front(), m_action_manager, m_game_log);
	
	//GUI_Window* MiniMap = new GUI_Window(0, 0, 400, 400, "Мини-карта");
	//GUI_MiniMap* mini_map = new GUI_MiniMap(position_t(5, 5), dimension_t(MiniMap->m_size.w - 10, MiniMap->m_size.h - 30), m_GUI->MapViewer);
	//MiniMap->add(mini_map);
	////MenuLayer->add(MiniMap);

	//MiniMap = new GUI_Window(300, 0, 400, 400, "Поле зрения player");
	//GUI_FOV* fov = new GUI_FOV(position_t(5, 5), dimension_t(MiniMap->m_size.w - 10, MiniMap->m_size.h - 30), m_GUI->MapViewer->m_player->m_object);
	//MiniMap->add(fov);

	/*obj = m_game_object_manager->new_object("bat");
	obj->set_direction(object_direction_e::top);
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 8][rx-2]);

	obj = m_game_object_manager->new_object("bat");
	obj->set_direction(object_direction_e::top);
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 8][rx - 4]);

	obj = m_game_object_manager->new_object("bat");
	obj->set_direction(object_direction_e::top);
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 8][rx - 6]);

	obj = m_game_object_manager->new_object("bat");
	obj->set_direction(object_direction_e::top);
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry-8][rx]);*/


	/*MiniMap = new GUI_Window(500, 0, 400, 400, "Поле зрения bat");
	fov = new GUI_FOV(position_t(5, 30), dimension_t(MiniMap->m_size.w - 10, MiniMap->m_size.h - 35), obj);
	MiniMap->add(fov);*/

	/*MenuLayer->add(m_GUI->Timer);
	MenuLayer->add(MiniMap);*/

	update_after_load();
}


void Application::update_after_load()
{
	//m_update_mutex.lock();
	m_world->calculate_lighting();
	m_gui_controller.m_GUI->MapViewer->update();
	for (auto m = m_world->m_maps.begin(); m != m_world->m_maps.end(); ++m)
	{
		(*m)->update(VoidEventArgs());
	}
	//m_update_mutex.unlock();
	//m_update_mutex.lock();
	if (m_world->m_player->m_object->m_active_state->m_ai)
	{
		GameObject* object = m_world->m_player->m_object;
		GameMap* map = static_cast<MapCell*>(object->m_owner)->m_map;
		static_cast<AI_enemy*>(m_world->m_player->m_object->m_active_state->m_ai)->calculate_FOV(m_world->m_player->m_object, map);
	}
	//m_update_mutex.unlock();
}

void Application::start()
{
	m_game_thread.reset(new std::thread(std::bind(&Application::process_game, this)));
	m_animation_thread.reset(new std::thread(&Timer::cycle, m_timer));
}

void Application::stop()
{
	if (m_animation_thread)
	{
		m_animation_thread->detach();
		m_animation_thread.reset();
	}
	if (m_game_thread)
	{
		m_game_thread->detach();
		m_game_thread.reset();
	}
}

void Application::PlaySound1()
{
	Mix_PlayChannel(-1, music, 0);
}

bool Application::check_action_completion(GameObject*& object)
{
	if (object->m_active_state->m_ai)
	{
		return !object->m_active_state->m_ai->m_action_controller->m_action;
	}
	return true;
}

void Application::update()
{
	//if (!m_action_manager->m_items.empty())
	//{
	//	std::map<int, std::list<GameTask*> > task_order;
	//	for (auto current = m_action_manager->m_items.begin(); current != m_action_manager->m_items.end(); current++)
	//	{
	//		if (!current->second.empty())
	//		{
	//			GameTask* A;
	//			A = current->second.front();
	//			task_order[A->m_action->m_decay].push_back(A);
	//			m_action_manager->remove(current->first);
	//		}
	//	}
	//	//LOG(INFO) << "----------------------";
	//	//for (auto current = task_order.begin(); current != task_order.end(); current++)
	//	//{
	//	//	for (auto task = current->second.begin(); task != current->second.end(); task++)
	//	//	{
	//	//		GameTask* A;
	//	//		A = (*task);
	//	//		LOG(INFO) << current->first << " - " << A->m_action->m_name;
	//	//	}
	//	//}
	//	for (auto current = task_order.begin(); current != task_order.end(); current++)
	//	{
	//		for (auto task = current->second.begin(); task != current->second.end(); task++)
	//		{
	//			GameTask* A;
	//			A = (*task);
	//			A->m_action->perfom(A->m_parameter);
	//			if (A->m_action->m_error != "")
	//			{
	//				m_GUI->DescriptionBox->add_item_control(new GUI_Text(A->m_action->m_error));
	//			}
	//		}
	//	}
	//}
	GameTask* A;
	
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
	std::chrono::milliseconds elapsed;
	while (true)
	{ 
		A = m_action_manager->get_task();
		if (A)
		{
			m_world->m_player->m_object->m_active_state->m_ai->m_action_controller->set(m_world->m_player->m_object, A->m_action, A->m_parameter);
			m_world->m_player->m_object->m_active_state->m_tile_manager->m_animation = A->m_action->m_animation;
		}
		do
		{
			m_update_mutex.lock();
			if (m_world->m_player->m_object->m_active_state->m_ai)
			{
				m_world->m_player->m_object->m_active_state->m_ai->m_action_controller->update();
			}
			//start = std::chrono::high_resolution_clock::now();
			m_world->m_object_manager.calculate_ai();
			end = std::chrono::high_resolution_clock::now();
			elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			//LOG(INFO) << "Просчет ИИ: " << std::to_string(elapsed.count());
			if (m_world->m_player->m_object->m_active_state->m_ai)
			{
				GameObject* object = m_world->m_player->m_object;
				GameMap* map = static_cast<MapCell*>(object->m_owner)->m_map;

				AI_enemy* ai = static_cast<AI_enemy*>(object->m_active_state->m_ai);
				ai->calculate_FOV(object, map);
				Vision_list* vl = static_cast<Vision_list*>(object->m_active_state->get_list(interaction_e::vision));
				
				int radius = vl->m_max_radius;

				int xc = object->cell()->x;
				int yc = object->cell()->y;
				int dx = object->m_active_state->m_size.x;
				int dy = object->m_active_state->m_size.y;

				int xs = ((dx - 1) >> 1);
				int ys = ((dy - 1) >> 1);

				int x_start = xc - radius + xs;
				x_start = max(x_start, 0);
				x_start = min(x_start, map->m_size.w - 1);
				int x_end = xc + radius + xs;
				x_end = max(x_end, 0);
				x_end = min(x_end, map->m_size.w - 1);
				int y_start = yc - radius - ys;
				y_start = max(y_start, 0);
				y_start = min(y_start, map->m_size.h - 1);
				int y_end = yc + radius - ys;
				y_end = max(y_end, 0);
				y_end = min(y_end, map->m_size.h - 1);

				for (int y = y_start; y < y_end + 1; y++)
				{
					for (int x = x_start; x < x_end + 1; x++)
					{
						if (ai->m_fov->m_map[y - y_start][x - x_start].visible)
						{
							map->m_items[y][x]->m_notable = true;
						}
					}
				}
			}
			//start = std::chrono::high_resolution_clock::now();
			m_world->m_object_manager.update_buff();
			end = std::chrono::high_resolution_clock::now();
			elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			//LOG(INFO) <<"Просчет баффов: " <<std::to_string(elapsed.count());
			//start = std::chrono::high_resolution_clock::now();
			m_world->calculate_lighting();
			end = std::chrono::high_resolution_clock::now();
			elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			//LOG(INFO) << "Просчет освещения: " << std::to_string(elapsed.count());
			//start = std::chrono::high_resolution_clock::now();
			m_gui_controller.m_GUI->MapViewer->update();
			end = std::chrono::high_resolution_clock::now();
			elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			//LOG(INFO) << "Просчет карты: " << std::to_string(elapsed.count());
			for (auto m = m_world->m_maps.begin(); m != m_world->m_maps.end(); ++m)
			{
				(*m)->update(VoidEventArgs());
			}
			m_update_mutex.unlock();
			std::u16string temp= u"Ход - " + Parser::CP866_to_UTF16(std::to_string(m_game_turn));
			m_game_log.add(game_log_message_t(game_log_message_type_e::message_time,temp));
			m_game_turn += 1;
			std::chrono::milliseconds Duration(1);
			std::this_thread::sleep_for(Duration);
		} while (!check_action_completion(m_world->m_player->m_object));
		m_update_mutex.lock();
		if (m_action_manager->m_is_remove) { m_action_manager->remove(); }
		if (m_action_manager->m_items.empty())
		{
			m_update_mutex.unlock();
			m_world->m_player->m_object->m_active_state->m_tile_manager->m_animation = animation_e::idle;
			return;
		}
		m_update_mutex.unlock();
	}
}

void Application::command_main_menu_select()
{
	LOG(INFO) << "Ожидание выбора в главном меню";
	bool Exit = false;
	while (Exit == false)
	{
		std::unique_lock<std::mutex> lk(m_message_queue.m_mutex);
		m_message_queue.m_reader = true;
		while (!m_message_queue.m_read_message)
		{
			m_message_queue.m_condition_variable.wait(lk);
		}
		m_message_queue.m_processed_message = true;
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::new_game)
		{
			new_game();
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::load_game)
		{
			load_game();
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::game_quit)
		{
			stop();
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			Exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	LOG(INFO) << "Окончание ожидания выбора в главном меню";
}

MapCell* Application::command_select_location(GameObject* object)
{
	MapCell* Result = nullptr;
	/*if (object)
	{
		m_GUI->MapViewer->m_cursor_x = object->m_active_state->m_size.x;
		m_GUI->MapViewer->m_cursor_y = object->m_active_state->m_size.y;
	}
	else
	{
		m_GUI->MapViewer->m_cursor_x = 1;
		m_GUI->MapViewer->m_cursor_y = 1;
	}*/
	m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Выберите клетку")));
	bool Exit = false;
	while (Exit == false)
	{
		std::unique_lock<std::mutex> lk(m_message_queue.m_mutex);
		m_message_queue.m_reader = true;
		while (!m_message_queue.m_read_message)
		{
			m_message_queue.m_condition_variable.wait(lk);
		}
		m_message_queue.m_processed_message = true;
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::owner)
		{
			Parameter* p = m_message_queue.m_items.front();
			if ((*p)[0].m_owner->m_kind == entity_e::cell)
			{
				Result = static_cast<MapCell*>((*p)[0].m_owner);
				Exit = true;
			}
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			Exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	/*m_GUI->MapViewer->m_cursor_x = 1;
	m_GUI->MapViewer->m_cursor_y = 1;*/
	return Result;
}

GameObject* Application::command_select_object_on_map()
{
	GameObject* Result = nullptr;
	/*m_GUI->MapViewer->m_cursor_x = 1;
	m_GUI->MapViewer->m_cursor_y = 1;*/
	m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Выберите обьект")));
	bool Exit = false;
	while (Exit == false)
	{
		std::unique_lock<std::mutex> lk(m_message_queue.m_mutex);
		m_message_queue.m_reader = true;
		while (!m_message_queue.m_read_message)
		{
			m_message_queue.m_condition_variable.wait(lk);
		}
		m_message_queue.m_processed_message = true;
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::object)
		{
			Parameter* p = m_message_queue.m_items.front();
			if((*p)[0].m_object->m_owner->Game_object_owner::m_kind==entity_e::cell)
			{
				Result = (*p)[0].m_object;
				Exit = true;
			}
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			Exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	return Result;
}

GameObject* Application::command_select_object()
{
	GameObject* Result = nullptr;
	/*m_GUI->MapViewer->m_cursor_x = 1;
	m_GUI->MapViewer->m_cursor_y = 1;*/
	m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Выберите обьект")));
	bool Exit = false;
	while (Exit == false)
	{
		std::unique_lock<std::mutex> lk(m_message_queue.m_mutex);
		m_message_queue.m_reader = true;
		while (!m_message_queue.m_read_message)
		{
			m_message_queue.m_condition_variable.wait(lk);
		}
		m_message_queue.m_processed_message = true;
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::object)
		{
			Result = (*m_message_queue.m_items.front())[0].m_object;
			//if (static_cast<P_object*>(Result)->m_object->m_owner->Game_object_owner::m_kind == entity_e::cell)
			{
				Exit = true;
			}
			//else Result = nullptr;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::owner)
		{
			Parameter* temp = m_message_queue.m_items.front();
			switch ((*temp)[0].m_owner->m_kind)
			{
			case entity_e::inventory_cell:
			{
				if (static_cast<Inventory_cell*>((*temp)[0].m_owner)->m_item)
				{
					Result = static_cast<Inventory_cell*>((*temp)[0].m_owner)->m_item;
					Exit = true;
				}
				else Result = nullptr;
				break;
			}
			case entity_e::body_part:
			{
				if (static_cast<Object_part*>((*temp)[0].m_owner)->m_item)
				{
					Result = static_cast<Object_part*>((*temp)[0].m_owner)->m_item;
					Exit = true;
				}
				else Result = nullptr;
				break;
			}
			}
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			Exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	return Result;
}

bool Application::command_open_body(GameObject*& Object)
{
	bool Result = false;
	Parts_list* Property = static_cast<Parts_list*>(Object->get_effect(interaction_e::body));
	if (Property != nullptr)
	{
		GUI_body_window* Window = new GUI_body_window(m_size.w / 2 - (192 + 2) / 2, m_size.h / 2 - (4 * 64 + 2) / 2, 192 + 4, 4 * 64 + 27, Object->m_name + u"::body",Object);
		/*GUI_Body* Inv = new GUI_Body(static_cast<Interaction_feature*>(Object->get_feature(object_feature_e::interaction_feature)));
		Inv->m_position.x = 2;
		Inv->m_position.y = Window->m_size.h - Inv->m_size.h - 2;
		Window->add_item_control(Inv);*/
		//m_GUI->add_front(Window);
		Result = true;
	}
	return Result;
}

void Application::command_gui_show_characterization(GameObject*& object)
{
	GUI_Description_window* Window = new GUI_Description_window(m_size.w / 2 - (192 + 2) / 2, m_size.h / 2 - (4 * 64 + 2) / 2, 800 + 4, 8 * 64 + 27, object->m_name + u"::Характеристика",object);
	//m_GUI->add_front(Window);
}

void Application::command_set_cursor(GLuint _Tile)
{
	m_mouse->m_cursor = _Tile;
}

void Application::command_set_pickup_item(GLuint _Tile)
{
	m_mouse->m_pickup_item = _Tile;
}

void Application::command_set_cursor_visibility(bool _Visibility)
{
	m_mouse->m_show_cursor = _Visibility;
}

void Application::command_set_pickup_item_visibility(bool _Visibility)
{
	m_mouse->m_show_pickup_item = _Visibility;
}

bool Application::command_check_position(GameObject*& object, MapCell*& position, GameMap*& map)
{
	std::function<bool(GameObject*)> qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicat;
	for (int i = 0; i<object->m_active_state->m_size.y; i++)
	{
		for (int j = 0; j<object->m_active_state->m_size.x; j++)
		{
			if (map->m_items[position->y + i][position->x + j] == nullptr){ return false; }
			for (std::list<GameObject*>::iterator item = map->m_items[position->y - i][position->x + j]->m_items.begin(); item != map->m_items[position->y - i][position->x + j]->m_items.end(); ++item)
			{
				if (((*item) != object) && qualifier((*item)))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void Application::process_game()
{
	command_main_menu_select();
	std::unique_lock<std::mutex> lk(m_turn_mutex);
	while(true)
	{
		m_turn_cv.wait(lk);
		std::chrono::time_point<std::chrono::steady_clock> start;
		std::chrono::time_point<std::chrono::steady_clock> end;
		std::chrono::milliseconds elapsed;
		start = std::chrono::high_resolution_clock::now();
		update();
		m_turn = false;
		end = std::chrono::high_resolution_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		LOG(INFO) << "Просчет 10 ходов: " << std::to_string(elapsed.count());
	}
}

void Application::on_turn()
{
	std::lock_guard<std::mutex> lk(m_turn_mutex);
	m_turn_cv.notify_one();
}

void my_audio_callback(void *userdata, uint8_t *stream, uint32_t len) {

	if (audio_len == 0)
		return;

	len = (len > audio_len ? audio_len : len);
	SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	//SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

	audio_pos += len;
	audio_len -= len;
}

Game_object_owner* Application::command_select_transfer(Parameter_destination* parameter){
	Game_object_owner* Result = nullptr;
	m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Выберите назначение")));
	bool Exit = false;
	while (Exit == false)
	{
		std::unique_lock<std::mutex> lk(m_message_queue.m_mutex);
		m_message_queue.m_reader = true;
		while (!m_message_queue.m_read_message)
		{
			m_message_queue.m_condition_variable.wait(lk);
		}
		m_message_queue.m_processed_message = true;
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::owner)
		{
			Result = (*m_message_queue.m_items.front())[0].m_owner;
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			Exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	return Result;
}

//Parameter* Application::command_select_transfer_source(Parameter_destination* parameter){
//	Parameter* Result = nullptr;
//	m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::string("Выберите объект")));
//	bool Exit = false;
//	while (Exit == false)
//	{
//		std::unique_lock<std::mutex> lk(m_message_queue.m_mutex);
//		m_message_queue.m_reader = true;
//		while (!m_message_queue.m_read_message)
//		{
//			m_message_queue.m_condition_variable.wait(lk);
//		}
//		m_message_queue.m_processed_message = true;
//		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_object)
//		{
//			Result = m_message_queue.m_items.front();
//			Exit = true;
//		}
//		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_owner)
//		{
//			Parameter* temp = m_message_queue.m_items.front();
//			switch (temp->m_cell->m_kind)
//			{
//			case entity_e::inventory_cell:
//				{
//					if (static_cast<Inventory_cell*>(temp->m_cell)->m_items)
//					{
//						Result = new P_object();
//						Result->m_object = static_cast<Inventory_cell*>(temp->m_cell)->m_items;
//						Exit = true;
//					}
//					else Result = nullptr;
//					break;
//				}
//			case entity_e::body_part:
//			{
//				if (static_cast<Object_part*>(temp->m_cell)->m_items)
//				{
//					Result = new P_object();
//					Result->m_object = static_cast<Object_part*>(temp->m_cell)->m_items;
//					Exit = true;
//				}
//				else Result = nullptr;
//				break;
//			}
//			}
//		}
//		if (m_message_queue.m_items.front()->m_kind == ParameterKind_Cancel)
//		{
//			Exit = true;
//		}
//		m_message_queue.m_read_message = false;
//		lk.unlock();
//		m_message_queue.m_condition_variable.notify_one();
//		m_message_queue.m_reader = false;
//	}
//	return Result;
//}

Object_part* Application::command_select_body_part()
{
	Object_part* Result = nullptr;
	m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Выберите слот")));
	bool Exit = false;
	while (Exit == false)
	{
		std::unique_lock<std::mutex> lk(m_message_queue.m_mutex);
		m_message_queue.m_reader = true;
		while (!m_message_queue.m_read_message)
		{
			m_message_queue.m_condition_variable.wait(lk);
		}
		m_message_queue.m_processed_message = true;
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::owner)
		{
			Parameter* p = m_message_queue.m_items.front();
			if ((*p)[0].m_owner->m_kind == entity_e::body_part)
			{
				Result = static_cast<Object_part*>((*p)[0].m_owner);
				Exit = true;
			}
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			Exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	return Result;
}

bool Application::command_agreement()
{
	bool result;
	bool Exit = false;
	while (Exit == false)
	{
		std::unique_lock<std::mutex> lk(m_message_queue.m_mutex);
		m_message_queue.m_reader = true;
		while (!m_message_queue.m_read_message)
		{
			m_message_queue.m_condition_variable.wait(lk);
		}
		m_message_queue.m_processed_message = true;
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::accept)
		{
			result= true;
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			result= false;
			Exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	return result;
}

void Application::command_change_owner(Instruction_slot_parameter* parameter)
{
	Parameter* p = static_cast<Instruction_slot_parameter*>(parameter)->m_parameter;
	Object_state* obj_state = (*p)[1].m_object->get_state(object_state_e::equip);
	//if (obj_state)
	//{
	//	if (object->m_active_state)
	//	{
	//		for (auto kind = object->m_active_state->m_items.begin(); kind != object->m_active_state->m_items.end(); kind++)
	//		{
	//			if (kind->first != interaction_e::damage&&kind->first != interaction_e::action)
	//			{
	//				//unit->add_effect(kind->first, kind->second);
	//				unit->add_from(kind->first, static_cast<Interaction_list*>(kind->second));
	//			}
	//		}
	//	}
	//}
	if (obj_state)
	{
		Object_tag* list = obj_state->get_tag(object_tag_e::equippable);
		if (list)
		{
			//LOG(INFO) << "step 0  "<<std::to_string((int)static_cast<ObjectTag::Equippable*>(list)->m_value->m_interaction_message_type);
			list->apply_effect((*p)[0].m_object, parameter);
		}
		//if (object->m_active_state)
		//{
		//	for (auto kind = object->m_active_state->m_items.begin(); kind != object->m_active_state->m_items.end(); kind++)
		//	{
		//		if (kind->first != interaction_e::damage&&kind->first != interaction_e::action)
		//		{
		//			//unit->add_effect(kind->first, kind->second);
		//			unit->add_from(kind->first, static_cast<Interaction_list*>(kind->second));
		//		}
		//	}
		//}
	}
	(*p)[0].m_object->update_interaction();
	(*p)[0].m_object->event_update(VoidEventArgs());
	//???? if (m_GUI->MapViewer->m_player->m_object == unit) { update_action_panel(); }
}

void Application::console(std::string text)
{
	//m_GUI->DescriptionBox->add_item_control(new GUI_Text(text,new GUI_TextFormat(8,17,RGBA_t(1.0,1.0,1.0,1.0))));
}
