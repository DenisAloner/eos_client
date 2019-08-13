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
#include <chrono>
#include <graphics/GUI_Description_window.h>

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

game_clipboard::game_clipboard() :m_item(nullptr) {}

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

	for (auto& current : m_update_in_render_thread)
	{
		current();
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
	const auto mouse = instance().m_mouse->get_mouse_position();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	if (m_mouse->m_show_cursor)
	{
		if (m_clipboard.m_item)
		{
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, m_clipboard.m_item->m_active_state->m_icon->m_value);
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

void Application::initialize(const dimension_t work_area_size, const HDC m_hDC, const HGLRC hRC)
{
	this->m_hDC = m_hDC;
	this->m_hRC = hRC;
	subhRC = wglCreateContext(m_hDC);
	wglShareLists(this->m_hRC, subhRC);
	m_size = work_area_size;
	m_gui_controller.m_size = m_size;
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
	m_actions[action_e::rotate_view] = new Action_rotate_view();
	m_actions[action_e::change_z_level] = new Action_change_z_level();

	for (size_t i = 0; i < action_e::max; i++)
	{
		m_actions[i]->m_index = i;
	}

	m_ai_manager = new AI_manager();
	m_game_object_manager = new GameObjectManager();
	const auto gw = new Game_world; //TODO костыль, продумать исправление
	SerializationContext pc(*gw); //TODO костыль, продумать исправление
	m_game_object_manager->init(pc);
	Logger::instance().info ( "Менеджер игровых объектов успешно инициализирован");

	m_gui_controller.m_GUI = new ApplicationGUI(0, 0, m_size.w, m_size.h);
	m_window_manager = new GUI_Window_manager(0, 0, m_size.w, m_size.h);
	m_gui_controller.m_GUI->add(m_window_manager);
	m_gui_controller.m_GUI->add(new GUI_Image((m_size.w - 1024) / 2, (m_size.h - 1024) / 2, 1024, 1024, m_graph->m_logo));

	auto main_menu = new GUI_Window(0, 0, 400, 400, utf8_to_cp1251_2("Главное меню"));
	main_menu->m_position = position_t((m_size.w - main_menu->m_size.w) / 2, (m_size.h - main_menu->m_size.h) / 2);
	auto menu = new GUI_Button_list(0, 0, 400, 400);
	auto button = new GUI_Mainmenu_button(0, 0, 396, 47, u"Новая игра", parameter_type_e::new_game);
	button->mouse_click += std::bind(&GUI_Window::on_close, main_menu, main_menu);
	menu->add_item_control(button);
	button = new GUI_Mainmenu_button(0, 0, 396, 47, u"Загрузка игры", parameter_type_e::load_game);
	button->mouse_click += std::bind(&GUI_Window::on_close, main_menu, main_menu);
	menu->add_item_control(button);
	button = new GUI_Mainmenu_button(0, 0, 396, 47, u"Выход", parameter_type_e::game_quit);
	button->mouse_click += std::bind(&GUI_Window::on_close, main_menu, main_menu);
	menu->add_item_control(button);
	main_menu->add(menu);

	m_graph->set_VSync(false);

	m_ready = true;
	start();
}


void Application::new_game()
{
	m_world = new Game_world();
	auto map = new GameMap(dimension3_t(128, 128, 10));

	map->generate_room();
	m_world->m_maps.push_back(map);
	int rx = 8;
	int ry = 8;

	auto obj = m_game_object_manager->new_object("human");
	obj->set_direction(object_direction_e::down);
	map->add_to_map(obj, map->get(1, ry, rx));
	m_world->m_player = new Player(obj, map);
	m_window_manager = new GUI_Window_manager(0, 0, m_size.w, m_size.h);

	//SerializationContext pc(*m_world);
	//pc.reset();
	//obj->reset_serialization_index();
	//const auto json = obj->get_packer().parser_to_json(*obj, pc);
	//Logger::instance().info(Parser::utf16_to_cp1251(json));

	/*std::u16string json;
	SerializationContext pc(*m_world);
	pc.reset();
	m_world->reset_serialization_index();
	json  = m_world->serialize(pc);
	char16_t marker = 0xFEFF;*/

	//LOG(INFO) << Parser::UTF16_to_CP1251(json);


	/*std::ofstream in_file(FileSystem::instance().m_resource_path + "Saves\\test.json", std::ios::binary);
	in_file.write(reinterpret_cast<const char*>(&marker), sizeof(marker));
	in_file.write(reinterpret_cast<const char*>(&json[0]), json.size() * 2);
	in_file.close();

	std::ifstream out_file(FileSystem::instance().m_resource_path + "Saves\\test.json", std::ios::binary | std::ios::ate);
	const auto size = out_file.tellg();
	std::string value(size, '\0');
	out_file.seekg(0);
	out_file.read(&value[0], size);
	out_file.close();
	value.erase(0, 2);
	std::u16string out(value.length()/2, '0');

	auto* a = reinterpret_cast<char*>(&value[0]);
	auto* b = reinterpret_cast<char*>(&json[0]);
	for (int i = 0; i < value.size(); ++i)
	{
		b[i] = a[i];
	}
	auto* world = new Game_world();
	SerializationContext pc1(*world);
	pc.reset();
	world->reset_serialization_index();
	world->deserialize(json, pc);*/


	/*GameMap* tmp;
	Parser::reset_object_counter();
	std::size_t pos=0;
	Parser::from_binary<GameMap*>(test, tmp, pos);

	tmp->reset_serialization_index();
	Parser::reset_object_counter();
	json = Parser::UTF16_to_CP1251(Parser::parser_to_json<GameMap*>(tmp));
	LOG(INFO) << json;*/

	/*Object_state* m = new Object_state();
	m->m_ai = nullptr;
	m->m_icon = nullptr;
	m->m_tile_manager = GameObjectManager::m_config.m_tile_managers.get_by_string("belt");
	LOG(INFO)  << std::to_string(GameObjectManager::m_config.m_tile_managers.get_by_string("belt")->m_index);
	m->m_light = nullptr;
	m->m_optical = nullptr;
	m->m_state = object_state_e::equip;
	m->m_size.x = 300;

	Interaction_list* l = new Interaction_list();

	Effect* e=new Effect();

	e->m_value = 65;
	e->m_subtype = effect_e::limit;

	l->add(e);
	l->add(e);
	l->add(nullptr);

	m->m_items[interaction_e::body] = l;

	GameObject* o = new GameObject();
	o->m_name = u"test object";
	o->m_state.push_back(m);
	o->m_active_state = m;

	Parser::reset_object_counter();
	o->reset_serialization_index();

	std::string test = o->to_binary();

	std::size_t temp=0;
	GameObject* tmp = new GameObject();

	Parser::reset_object_counter();
	tmp->reset_serialization_index();

	tmp->from_binary_const(test);

	tmp->reset_serialization_index();
	Parser::reset_object_counter();

	LOG(INFO) << " | " << std::to_string(m->m_size.x);
	json = Parser::UTF16_to_CP1251(Parser::parser_to_json<GameObject*>(tmp));
	LOG(INFO) << json;*/

	m_gui_controller.m_GUI = new ApplicationGUI(0, 0, m_size.w, m_size.h, m_world->m_player, map, m_action_manager, m_game_log);

	auto MiniMap = new GUI_Window(0, 0, 400, 400, u"Мини-карта");
	auto cr = MiniMap->client_rect();
	auto* mini_map = new GUI_MiniMap(position_t(0, 0), dimension_t(cr.w, cr.h), m_gui_controller.m_GUI->MapViewer);
	MiniMap->add(mini_map);

	MiniMap = new GUI_Window(300, 0, 400, 400, u"Поле зрения player");
	cr = MiniMap->client_rect();
	auto fov = new GUI_FOV(position_t(0, 0), dimension_t(cr.w, cr.h), m_world->m_player->m_object);
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
	///////////////////////
			/*obj = m_game_object_manager->new_object("bat");
			obj->set_direction(object_direction_e::top);
			map->add_to_map(obj, map->get(0,ry - 2,rx-2));*/

	rx = 8;
	ry = 8;

	/*obj = m_game_object_manager->new_object("darkeye");
	obj->set_direction(object_direction_e::down);
	map->add_to_map(obj, map->get(1, ry, rx-2));*/

	/*obj = m_game_object_manager->new_object("bag");
	obj->set_direction(object_direction_e::top);
	map->add_to_map(obj, map->get(ry - 2,rx-1));

	obj = m_game_object_manager->new_object("ring");
	obj->set_direction(object_direction_e::top);
	map->add_to_map(obj, map->get(ry - 2,rx + 1));

	obj = m_game_object_manager->new_object("boot");
	obj->set_direction(object_direction_e::top);
	map->add_to_map(obj, map->get(ry - 2,rx + 2));

	obj = m_game_object_manager->new_object("fountain");
	obj->set_direction(object_direction_e::top);
	map->add_to_map(obj, map->get(ry + 10,rx + 2));*/

	/*MiniMap = new GUI_Window(500, 0, 400, 400, "Поле зрения bat");
	fov = new GUI_FOV(position_t(5, 30), dimension_t(MiniMap->m_size.w - 10, MiniMap->m_size.h - 35), obj);
	MiniMap->add(fov);*/

	/*MenuLayer->add(m_GUI->Timer);
	MenuLayer->add(MiniMap);*/

	update_after_load();
}

void Application::load_game()
{
	std::ifstream file(FileSystem::instance().m_resource_path + "Saves\\save.bin", std::ios::binary | std::ios::ate);
	const auto size = file.tellg();
	std::string value(size, '\0');
	file.seekg(0);
	file.read(&value[0], size);
	file.close();
	auto* world = new Game_world();
	SerializationContext pc(*world);
	pc.reset();
	world->reset_serialization_index();
	world->bin_deserialize(value, pc);
	m_world = world;
	m_window_manager = new GUI_Window_manager(0, 0, m_size.w, m_size.h);
	m_gui_controller.m_GUI = new ApplicationGUI(0, 0, m_size.w, m_size.h, m_world->m_player, m_world->m_maps.front(), m_action_manager, m_game_log);
	update_after_load();
}


void Application::update_after_load()
{
	//m_update_mutex.lock();
	m_world->calculate_lighting();
	m_gui_controller.m_GUI->MapViewer->update();
	for (auto& m_map : m_world->m_maps)
	{
		m_map->update(VoidEventArgs());
	}
	//m_update_mutex.unlock();
	//m_update_mutex.lock();
	if (m_world->m_player->m_object->m_active_state->m_ai)
	{
		const auto object = m_world->m_player->m_object;
		const auto map = static_cast<MapCell*>(object->m_owner)->m_map;
		static_cast<AI_enemy*>(m_world->m_player->m_object->m_active_state->m_ai)->calculate_fov(m_world->m_player->m_object, map);
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
	while (true)
	{
		const auto a = m_action_manager->get_task();
		if (a)
		{
			m_world->m_player->m_object->m_active_state->m_ai->m_action_controller->set(m_world->m_player->m_object, a->m_action, a->m_parameter);
			m_world->m_player->m_object->m_active_state->m_tile_manager->m_animation = a->m_action->m_animation;
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
			//LOG(INFO) << "Просчет ИИ: " << std::to_string(elapsed.count());
			if (m_world->m_player->m_object->m_active_state->m_ai)
			{
				auto object = m_world->m_player->m_object;
				auto map = static_cast<MapCell*>(object->m_owner)->m_map;

				auto ai = static_cast<AI_enemy*>(object->m_active_state->m_ai);
				ai->calculate_fov(object, map);
				auto vl = static_cast<Vision_list*>(object->m_active_state->get_list(interaction_e::vision));

				int radius = vl->m_max_radius;

				int xc = object->cell()->x;
				int yc = object->cell()->y;
				int zc = object->cell()->z;
				int dx = object->m_active_state->m_size.dx;
				int dy = object->m_active_state->m_size.dy;
				int dz = object->m_active_state->m_size.dz;

				int xs = ((dx - 1) >> 1);
				int ys = ((dy - 1) >> 1);
				int zs = ((dz - 1) >> 1);

				int x_start = xc - radius + xs;
				x_start = max(x_start, 0);
				x_start = min(x_start, map->m_size.dx - 1);
				int x_end = xc + radius + xs;
				x_end = max(x_end, 0);
				x_end = min(x_end, map->m_size.dx - 1);
				int y_start = yc - radius - ys;
				y_start = max(y_start, 0);
				y_start = min(y_start, map->m_size.dy - 1);
				int y_end = yc + radius - ys;
				y_end = max(y_end, 0);
				y_end = min(y_end, map->m_size.dy - 1);
				int z_start = zc - radius + zs;
				z_start = max(z_start, 0);
				z_start = min(z_start, map->m_size.dz - 1);
				int z_end = zc + radius + zs;
				z_end = max(z_end, 0);
				z_end = min(z_end, map->m_size.dz - 1);

				for (auto z = z_start; z < z_end + 1; z++)
				{
					for (auto y = y_start; y < y_end + 1; y++)
					{
						for (auto x = x_start; x < x_end + 1; x++)
						{
							if (ai->m_fov->m_map[y - y_start][x - x_start].visible)
							{
								map->get(z, y, x).m_notable = true;
							}
						}
					}
				}
			}
			//start = std::chrono::high_resolution_clock::now();
			m_world->m_object_manager.update_buff();

			//LOG(INFO) <<"Просчет баффов: " <<std::to_string(elapsed.count());
			//start = std::chrono::high_resolution_clock::now();
			m_world->calculate_lighting();

			//LOG(INFO) << "Просчет освещения: " << std::to_string(elapsed.count());
			//start = std::chrono::high_resolution_clock::now();
			m_gui_controller.m_GUI->MapViewer->update();

			//LOG(INFO) << "Просчет карты: " << std::to_string(elapsed.count());
			for (auto& m_map : m_world->m_maps)
			{
				m_map->update(VoidEventArgs());
			}
			m_update_mutex.unlock();
			auto temp = u"Ход - " + cp1251_to_utf16(std::to_string(m_game_turn));
			m_game_log.add(game_log_message_t(message_time, temp));
			m_game_turn += 1;
			std::chrono::milliseconds duration(1);
			std::this_thread::sleep_for(duration);
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
	Logger::instance().info(  "Ожидание выбора в главном меню");
	auto exit = false;
	while (!exit)
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
			exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::load_game)
		{
			load_game();
			exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::game_quit)
		{
			stop();
			exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	Logger::instance().info ( "Окончание ожидания выбора в главном меню");
}

MapCell* Application::command_select_location(GameObject* object)
{
	MapCell* result = nullptr;
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
	m_game_log.add(game_log_message_t(message_action_interaction, std::u16string(u"Выберите клетку")));
	auto exit = false;
	while (!exit)
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
			auto p = m_message_queue.m_items.front();
			if ((*p)[0].m_owner->m_kind == entity_e::cell)
			{
				result = static_cast<MapCell*>((*p)[0].m_owner);
				exit = true;
			}
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	/*m_GUI->MapViewer->m_cursor_x = 1;
	m_GUI->MapViewer->m_cursor_y = 1;*/
	return result;
}

GameObject* Application::command_select_object_on_map()
{
	GameObject* result = nullptr;
	/*m_GUI->MapViewer->m_cursor_x = 1;
	m_GUI->MapViewer->m_cursor_y = 1;*/
	m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Выберите обьект")));
	auto exit = false;
	while (!exit)
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
			auto p = m_message_queue.m_items.front();
			if ((*p)[0].m_object->m_owner->Game_object_owner::m_kind == entity_e::cell)
			{
				result = (*p)[0].m_object;
				exit = true;
			}
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	return result;
}

GameObject* Application::command_select_object()
{
	GameObject* Result = nullptr;
	/*m_GUI->MapViewer->m_cursor_x = 1;
	m_GUI->MapViewer->m_cursor_y = 1;*/
	m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Выберите обьект")));
	auto exit = false;
	while (!exit)
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
				exit = true;
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
					exit = true;
				}
				else Result = nullptr;
				break;
			}
			case entity_e::body_part:
			{
				if (static_cast<Object_part*>((*temp)[0].m_owner)->m_item)
				{
					Result = static_cast<Object_part*>((*temp)[0].m_owner)->m_item;
					exit = true;
				}
				else Result = nullptr;
				break;
			}
			}
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			exit = true;
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
	auto result = false;
	const auto property = static_cast<Parts_list*>(Object->get_effect(interaction_e::body));
	if (property != nullptr)
	{
		auto Window = new GUI_body_window(m_size.w / 2 - (192 + 2) / 2, m_size.h / 2 - (4 * 64 + 2) / 2, 192 + 4, 4 * 64 + 27, Object->m_name + u"::body", Object);
		/*GUI_Body* Inv = new GUI_Body(static_cast<Interaction_feature*>(Object->get_feature(object_feature_e::interaction_feature)));
		Inv->m_position.x = 2;
		Inv->m_position.y = Window->m_size.h - Inv->m_size.h - 2;
		Window->add_item_control(Inv);*/
		//m_GUI->add_front(Window);
		result = true;
	}
	return result;
}

void Application::command_gui_show_characterization(GameObject*& object)
{
	GUI_Description_window* Window = new GUI_Description_window(m_size.w / 2 - (192 + 2) / 2, m_size.h / 2 - (4 * 64 + 2) / 2, 800 + 4, 8 * 64 + 27, object->m_name + u"::Характеристика", object);
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
	const auto qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicate;
	for (auto z = 0; z < object->m_active_state->m_size.dz; z++)
	{
		for (auto y = 0; y < object->m_active_state->m_size.dy; y++)
		{
			for (auto x = 0; x < object->m_active_state->m_size.dx; x++)
			{
				//if (map->get(position->y + i,position->x + j) == nullptr){ return false; }
				for (auto& m_item : map->get(position->z + z, position->y - y, position->x + x).m_items)
				{
					if (m_item != object && qualifier(m_item))
					{
						return false;
					}
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
	while (true)
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
		Logger::instance().info ("Просчет 10 ходов: " + std::to_string(elapsed.count()));
	}
}

void Application::on_turn()
{
	std::lock_guard<std::mutex> lk(m_turn_mutex);
	m_turn_cv.notify_one();
}

Game_object_owner* Application::command_select_transfer(Parameter_destination* parameter) {
	Game_object_owner* result = nullptr;
	m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Выберите назначение")));
	auto exit = false;
	while (!exit)
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
			result = (*m_message_queue.m_items.front())[0].m_owner;
			exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	return result;
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
	auto exit = false;
	while (!exit)
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
			const auto p = m_message_queue.m_items.front();
			if ((*p)[0].m_owner->m_kind == entity_e::body_part)
			{
				Result = static_cast<Object_part*>((*p)[0].m_owner);
				exit = true;
			}
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			exit = true;
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
	auto exit = false;
	while (exit == false)
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
			result = true;
			exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == parameter_type_e::cancel)
		{
			result = false;
			exit = true;
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
	auto p = static_cast<Instruction_slot_parameter*>(parameter)->m_parameter;
	auto obj_state = (*p)[1].m_object->get_state(object_state_e::equip);
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
		auto list = obj_state->get_tag(object_tag_e::equippable);
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
