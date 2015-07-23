#include "game/Application.h"
#include "game/ApplicationGUI.h"
#include "game/MouseController.h"
#include "game/ActionManager.h"
#include "game/GameObject.h"
#include "game/graphics/GUI_ActionManager.h"
#include "game/graphics/GUI_ActionPanel.h"
#include "game/graphics/GUI_ActionButton.h"
#include "game/graphics/GUI_Window.h"
#include "game/graphics/GUI_MapViewer.h"
#include "game/graphics/GUI_TextBox.h"
#include "game/graphics/GUI_Timer.h"
#include "utils/log.h"
#include <chrono>
#include <Action_controller.h>


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
: m_timer(new Timer(8, 300))
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
	m_GUI->key_press(w);
	m_update_mutex.unlock();
}

void Application::on_mouse_click(MouseEventArgs const& e)
{
	m_update_mutex.lock();
	m_GUI->mouse_click(e);
	m_update_mutex.unlock();
}

void Application::on_mouse_down(MouseEventArgs const& e)
{
	m_update_mutex.lock();
	m_GUI->mouse_down(e);
	m_update_mutex.unlock();
}

void Application::on_mouse_move(MouseEventArgs const& e)
{
	m_update_mutex.lock();
	m_GUI->mouse_move(e);
	m_update_mutex.unlock();
}

void Application::on_mouse_wheel(MouseEventArgs const& e)
{
	m_update_mutex.lock();
	m_GUI->mouse_wheel(e);
	m_update_mutex.unlock();
}

void Application::render()
{
	m_update_mutex.lock();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	if (Application::instance().m_GUI->MapViewer->m_map->m_update)
	{
		Application::instance().m_GUI->MapViewer->m_map->update(VoidEventArgs());
		Application::instance().m_GUI->MapViewer->m_map->m_update = false;
	}
	m_GUI->render(m_graph, 0, 0);
	const position_t mouse = Application::instance().m_mouse->get_mouse_position();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	if (m_mouse->m_show_cursor)
	{
		if (m_clipboard.m_item)
		{
			glColor4d(1.0, 1.0, 1.0, 0.5);
			glBindTexture(GL_TEXTURE_2D, m_clipboard.m_item->m_active_state->m_icon);
			m_graph->draw_sprite(mouse.x - 32, mouse.y - 32, mouse.x - 32, mouse.y + 32, mouse.x + 32, mouse.y + 32, mouse.x + 32, mouse.y - 32);
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
//#warning TODO Избавиться от магических чисел!
		m_graph->draw_sprite(mouse.x, mouse.y, mouse.x, mouse.y + 48, mouse.x + 48, mouse.y + 48, mouse.x + 48, mouse.y);
		
	}
	m_update_mutex.unlock();
}

void Application::initialize()
{
	music = NULL;
	m_game_turn = 1;
	m_ready = false;
	m_graph = new GraphicalController();
	m_mouse = new MouseController();
	command_set_cursor(m_graph->m_cursor);
	command_set_cursor_visibility(true);

	m_GUI = new ApplicationGUI(0,0,1024,1024);
	
	key_press += std::bind(&Application::on_key_press, this, std::placeholders::_1);
	m_mouse->mouse_click += std::bind(&Application::on_mouse_click, this, std::placeholders::_1);
	m_mouse->mouse_down += std::bind(&Application::on_mouse_down, this, std::placeholders::_1);
	m_mouse->mouse_wheel += std::bind(&Application::on_mouse_wheel, this, std::placeholders::_1);
	m_mouse->mouse_move += std::bind(&Application::on_mouse_move, this, std::placeholders::_1);
	m_action_manager = new ActionManager();

	m_actions[action_e::move] = new action_move_step();
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
	m_ai_manager = new AI_manager();
	m_game_object_manager = new GameObjectManager();
	m_game_object_manager->init();
	LOG(INFO) << "Менеджер игровых объектов успешно инициализирован";

	m_GUI->MapViewer = new GUI_MapViewer(this);
	m_GUI->MapViewer->m_position.x = 0;
	m_GUI->MapViewer->m_position.y = 0;
	m_GUI->MapViewer->m_size.w = 1024;
	m_GUI->MapViewer->m_size.h = 1024;
	
	m_GUI->MapViewer->m_map = new GameMap(dimension_t(200,200));
	m_GUI->MapViewer->m_map->generate_level();
	//m_GUI->MapViewer->m_map->generate_room();

	/*int rx = 100;
	int ry = 100;
	GameObject* obj = m_game_object_manager->new_object("iso_unit");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry][rx]);
	m_GUI->MapViewer->m_player = new Player(obj, m_GUI->MapViewer->m_map);*/


	int index = rand() % m_GUI->MapViewer->m_map->m_link_rooms.size();
	GameMap::block_t* room = *std::next(m_GUI->MapViewer->m_map->m_link_rooms.begin(), index);
	int rx = rand() % room->rect.w;
	int ry = rand() % room->rect.h;
	GameObject* obj = m_game_object_manager->new_object("iso_unit");
	m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[room->rect.y + ry][room->rect.x + rx]);
	m_GUI->MapViewer->m_player = new Player(obj, m_GUI->MapViewer->m_map);

	rx = room->rect.x + rx;
	ry = room->rect.y + ry;

	m_window_manager = new GUI_Window_manager(0, 0, 1024, 1024);

	GUI_ActionManager* AMTextBox;
	AMTextBox = new GUI_ActionManager(m_action_manager);
	AMTextBox->m_position.x = 650;
	AMTextBox->m_position.y = 710;
	AMTextBox->resize(372, 263);
	GUI_ActionPanel* ActionPanel;
	ActionPanel = new GUI_ActionPanel(2,975,898,47);
	m_GUI->m_action_panel = ActionPanel;
	update_action_panel();
	GUI_Layer* MenuLayer;
	MenuLayer = new GUI_Layer(0,0,1024,1024);
	GUI_TextBox* TextBox = new GUI_TextBox();
	TextBox->m_position.x = 2;
	TextBox->m_position.y = 710;
	TextBox->resize(646, 263);

	m_GUI->DescriptionBox = TextBox;

	//m_GUI->Timer = new GUI_Timer(902, 975, 120, 47, 0);

	GUI_button* button = new GUI_button(902, 975, 120, 47, "Ход");
	button->mouse_click += std::bind(&Application::on_turn, this);

	MenuLayer->add(AMTextBox);
	MenuLayer->add(ActionPanel);
	MenuLayer->add(TextBox);
	MenuLayer->add(button);

	GUI_Window* MiniMap = new GUI_Window(0, 0, 400, 400, "Мини-карта");
	GUI_MiniMap* mini_map = new GUI_MiniMap(position_t(5, 30), dimension_t(MiniMap->m_size.w - 10, MiniMap->m_size.h - 35), m_GUI->MapViewer);
	MiniMap->add(mini_map);
	//MenuLayer->add(MiniMap);

	MiniMap = new GUI_Window(300, 0, 400, 400, "Поле зрения");
	GUI_FOV* fov = new GUI_FOV(position_t(5, 30), dimension_t(MiniMap->m_size.w - 10, MiniMap->m_size.h - 35), m_GUI->MapViewer->m_player->m_fov);
	MiniMap->add(fov);

	//MenuLayer->add(m_GUI->Timer);
	//MenuLayer->add(MiniMap);
	
	
	//MenuLayer->add(new GUI_Item(0, 0, 100,21, "4565656"));
	m_GUI->add(m_window_manager);
	m_GUI->add(MenuLayer);
	m_GUI->add(m_GUI->MapViewer);
	m_GUI->MapViewer->m_GUI = MenuLayer;

	m_graph->set_VSync(false);
	//if (SDL_Init(SDL_INIT_AUDIO) < 0)
	//{
	//	MessageBox(NULL, "Error", "Audio", MB_OK);
	//}
	//static Uint32 wav_length; // length of our sample
	//static Uint8 *wav_buffer; // buffer containing our audio file
	//static SDL_AudioSpec wav_spec; // the specs of our piece of music
	//static Uint32 wav_length2; // length of our sample
	//static Uint8 *wav_buffer2; // buffer containing our audio file
	//static SDL_AudioSpec wav_spec2; // the specs of our piece of music
	//if (SDL_LoadWAV("C:\\ExplorersOfSaarum\\123.wav", &wav_spec, &wav_buffer, &wav_length) == NULL){
	//	MessageBox(NULL, "Error", "Audio", MB_OK);
	//}
	//if (SDL_LoadWAV("C:\\ExplorersOfSaarum\\456.wav", &wav_spec2, &wav_buffer2, &wav_length2) == NULL){
	//	MessageBox(NULL, "Error", "Audio", MB_OK);
	//}
	//wav_spec.callback = my_audio_callback;
	//wav_spec.userdata = NULL;
	//// set our global static variables
	//audio_pos = wav_buffer; // copy sound buffer
	//audio_len = wav_length; // copy file length

	///* Open the audio device */
	//if (SDL_OpenAudio(NULL, NULL) < 0){
	//	MessageBox(NULL, "Error", "Audio", MB_OK);
	//	exit(-1);
	//}

	///* Start playing */
	//SDL_PauseAudio(0);
	//SDL_Delay(900);
	//SDL_MixAudio(audio_pos, wav_buffer2, audio_len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	//// wait until we're don't playing
	///*while (audio_len > 0) {
	//}*/
	//// shut everything down
	///*SDL_CloseAudio();
	//SDL_FreeWAV(wav_buffer);*/

	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
	int audio_channels = 2;
	int audio_buffers = 4096;

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		MessageBox(NULL, "Error", "Audio", MB_OK);
	}
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		MessageBox(NULL, "Error", "Audio", MB_OK);
		exit(1);
	}
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	if (music == NULL) {

		/* Actually loads up the music */
		music = Mix_LoadWAV((FileSystem::instance().m_resource_path+"Sounds\\Click.wav").c_str());

		/* This begins playing the music - the first argument is a
		pointer to Mix_Music structure, and the second is how many
		times you want it to loop (use -1 for infinite, and 0 to
		have it just play once) */
	}
	//obj = m_game_object_manager->new_object("snake");
	//obj->set_direction(object_direction_e_Left);
	//m_GUI->MapViewer->m_map->add_ai_object(obj, m_GUI->MapViewer->m_map->m_items[room->rect.y + ry][room->rect.x + rx + 1]);
	//obj = m_game_object_manager->new_object("plant");
	//obj->set_direction(object_direction_e_Left);
	//m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[room->rect.y + ry - 3][room->rect.x + rx]);
	//obj = m_game_object_manager->new_object("sword");
	//m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[room->rect.y + ry][room->rect.x + rx - 2]);
	//obj = m_game_object_manager->new_object("dagger");
	//m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[room->rect.y + ry][room->rect.x + rx - 3]);
	//obj = m_game_object_manager->new_object("smoke");
	//obj->set_direction(object_direction_e_Left);
	//m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[room->rect.y + ry][room->rect.x + rx - 4]);
	//obj = m_game_object_manager->new_object("trap");
	//m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[room->rect.y + ry][room->rect.x + rx + 2]);
	 
	obj = m_game_object_manager->new_object("dagger");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry ][rx - 1]);

	obj = m_game_object_manager->new_object("bat");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 7][rx]);

	obj = m_game_object_manager->new_object("rat");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 7][rx-3]);

	obj = m_game_object_manager->new_object("rat");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 7][rx+3]);

	/*obj = m_game_object_manager->new_object("bear");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 7][rx]);*/

	//obj = m_game_object_manager->new_object("skeleton");
	//m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 5][rx - 4]);
	
	/*obj = m_game_object_manager->new_object("skull");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 5][rx + 1]);
	obj = m_game_object_manager->new_object("slave_skull");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 5][rx + 2]);
	obj = m_game_object_manager->new_object("slave_skull");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 5][rx]);*/


	/*obj = m_game_object_manager->new_object("iso_torch");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry + 2][rx + 5]);*/

	/*obj = m_game_object_manager->new_object("wall");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 1][rx + 3]);
	obj = m_game_object_manager->new_object("wall");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 1][rx + 2]);
*/
	/*obj = m_game_object_manager->new_object("snake");
	m_GUI->MapViewer->m_map->add_to_map(obj, m_GUI->MapViewer->m_map->m_items[ry - 1][rx + 5]);*/

	/*obj = m_game_object_manager->new_object("ice");
	m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[ry][rx + 10]);

	obj = m_game_object_manager->new_object("smoke");
	obj->set_direction(object_direction_e_Left);
	m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[ry-5][rx]);

	obj = m_game_object_manager->new_object("wall");
	m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[ry - 7][rx]);

	obj = m_game_object_manager->new_object("smoke");
	obj->set_direction(object_direction_e_Left);
	m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[ry - 6][rx-1]);*/


	/*obj = m_game_object_manager->new_object("torch");
	m_GUI->MapViewer->m_map->add_new_object(obj, m_GUI->MapViewer->m_map->m_items[ry + 15][rx]);*/

	/*int ru;
	for (int i = 0; i <15; i++){
		index = rand() % m_GUI->MapViewer->m_map->m_link_rooms.size();
		room = *std::next(m_GUI->MapViewer->m_map->m_link_rooms.begin(), index);
		ru = rand() % 4;
		rx = rand() % room->rect.w;
		ry = rand() % room->rect.h;
		switch (ru)
		{
		case 0:
		{
			obj = m_game_object_manager->new_object("snake");
			break;
		}
		case 1:
		{
			obj = m_game_object_manager->new_object("bear");
			break;
		}
		case 2:
		{
			obj = m_game_object_manager->new_object("ghost");
			break;
		}
		case 3:
		{
			obj = m_game_object_manager->new_object("slime");
			break;
		}
		}
		obj->set_direction(object_direction_e_Left);
		m_GUI->MapViewer->m_map->add_ai_object(obj, m_GUI->MapViewer->m_map->m_items[room->rect.y + ry][room->rect.x + rx]);
	}*/

	m_ready = true;
}

void Application::get_action_predicat(Object_interaction* object)
{
	if (object->m_interaction_message_type == interaction_message_type_e::action)
	{
		GUI_ActionButton* ActionButton = new GUI_ActionButton();
		ActionButton->m_action = static_cast<Action*>(object);
		m_GUI->m_action_panel->add_item_control(ActionButton);
	}
}

void Application::update_action_panel()
{
	GUI_ActionButton* ActionButton;
	GUI_ActionPanel* panel = m_GUI->m_action_panel;
	while (!panel->m_items.empty())
	{
		panel->remove(panel->m_items.front());
	}
	auto player_action_list = m_GUI->MapViewer->m_player->m_actions;
	for (auto item = player_action_list.begin(); item != player_action_list.end(); item++)
	{
		ActionButton = new GUI_ActionButton();
		ActionButton->m_action = (*item);
		m_GUI->m_action_panel->add_item_control(ActionButton);
	}
	Attribute_map* list = m_GUI->MapViewer->m_player->m_object->m_active_state;
	for (auto item = list->m_item.begin(); item != list->m_item.end(); item++)
	{
		item->second->do_predicat(std::bind(&Application::get_action_predicat, this, std::placeholders::_1));
	}
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
	do
	{
		m_update_mutex.lock();
		m_GUI->MapViewer->m_player->m_object->m_active_state->m_ai->m_action_controller->update();
		Application::instance().m_game_object_manager->calculate_ai(m_GUI->MapViewer->m_map);
		Application::instance().m_game_object_manager->update_buff();
		m_GUI->MapViewer->m_map->calculate_lighting2();
		Application::instance().m_GUI->MapViewer->update();
		Application::instance().m_GUI->MapViewer->m_map->m_update = true;
		m_update_mutex.unlock();
		std::chrono::milliseconds Duration(1);
		std::this_thread::sleep_for(Duration);
	} while (m_GUI->MapViewer->m_player->m_object->m_active_state->m_ai->m_action_controller->m_wrapper.m_action);
	m_update_mutex.lock();
	if (m_GUI->MapViewer->m_player->m_object->m_active_state->m_ai)
	{
		m_GUI->MapViewer->m_player->m_fov->calculate(static_cast<AI_enemy*>(m_GUI->MapViewer->m_player->m_object->m_active_state->m_ai)->m_fov_radius, m_GUI->MapViewer->m_player->m_object, m_GUI->MapViewer->m_map);
	}
	m_update_mutex.unlock();
}

Parameter* Application::command_select_location(GameObject* object)
{
	Parameter* Result = nullptr;
	if (object)
	{
		m_GUI->MapViewer->m_cursor_x = object->m_active_state->m_size.x;
		m_GUI->MapViewer->m_cursor_y = object->m_active_state->m_size.y;
	}
	else
	{
		m_GUI->MapViewer->m_cursor_x = 1;
		m_GUI->MapViewer->m_cursor_y = 1;
	}
	m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выберите клетку."));
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
		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_cell)
		{
			Result = m_message_queue.m_items.front();
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == ParameterKind_Cancel)
		{
			Exit = true;
		}
		m_message_queue.m_read_message = false;
		lk.unlock();
		m_message_queue.m_condition_variable.notify_one();
		m_message_queue.m_reader = false;
	}
	m_GUI->MapViewer->m_cursor_x = 1;
	m_GUI->MapViewer->m_cursor_y = 1;
	return Result;
}

Parameter* Application::command_select_object_on_map()
{
	Parameter* Result = nullptr;
	m_GUI->MapViewer->m_cursor_x = 1;
	m_GUI->MapViewer->m_cursor_y = 1;
	m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выберите обьект."));
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
		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_object)
		{
			Result = m_message_queue.m_items.front();
			if(static_cast<P_object*>(Result)->m_object->m_owner->Game_object_owner::m_kind==entity_e::cell)
			{
				Exit = true;
			}
			else Result = nullptr;
		}
		if (m_message_queue.m_items.front()->m_kind == ParameterKind_Cancel)
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
		GUI_body_window* Window = new GUI_body_window(1024 / 2 - (192 + 2) / 2, 1024 / 2 - (4 * 64 + 2) / 2, 192 + 4, 4 * 64 + 27, Object->m_name + "::body",Object);
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
	GUI_description_window* Window = new GUI_description_window(1024 / 2 - (192 + 2) / 2, 1024 / 2 - (4 * 64 + 2) / 2, 800 + 4, 8 * 64 + 27, object->m_name + "::Характеристика",object);
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
	std::function<bool(GameObject*)> qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier;
	for (int i = 0; i<object->m_active_state->m_size.y; i++)
	{
		for (int j = 0; j<object->m_active_state->m_size.x; j++)
		{
			if (map->m_items[position->y + i][position->x + j] == nullptr){ return false; }
			for (std::list<GameObject*>::iterator item = map->m_items[position->y - i][position->x + j]->m_items.begin(); item != map->m_items[position->y - i][position->x + j]->m_items.end(); item++)
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
	/*int time = 1;
	while (true)
	{
		if (time == 1)
		{
			time = 15;
		}
		else
		{
			time -= 1;
		}
		m_GUI->Timer->Update(time);
		if (time == 15)
		{
			update();
			m_GUI->DescriptionBox->add_item_control(new GUI_Text("Ход - " + std::to_string(m_game_turn) + ".", new GUI_TextFormat(10, 19, RGBA_t(0.0, 0.8, 0.0, 1.0))));
			m_game_turn += 1;
		}
		std::chrono::milliseconds Duration(250);
		std::this_thread::sleep_for(Duration);
	}*/
	update();
	while (true)
	{
		if (m_turn)
		{
			update();
			m_GUI->DescriptionBox->add_item_control(new GUI_Text("Ход - " + std::to_string(m_game_turn) + ".", new GUI_TextFormat(10, 19, RGBA_t(0.0, 0.8, 0.0, 1.0))));
			m_game_turn += 1;
			m_turn = false;
		}
	}
}

void Application::on_turn()
{
	m_turn = true;
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

Parameter* Application::command_select_transfer(Parameter_destination* parameter){
	Parameter* Result = nullptr;
	m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выберите назначение."));
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
		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_body_part)
		{
			Result = m_message_queue.m_items.front();
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_inventory_cell)
		{
			Result = m_message_queue.m_items.front();
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_cell)
		{
			Result = m_message_queue.m_items.front();
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == ParameterKind_Cancel)
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

P_object* Application::command_select_transfer_source(Parameter_destination* parameter){
	P_object* Result = nullptr;
	m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выберите объект."));
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
		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_object)
		{
			Result = static_cast<P_object*>(m_message_queue.m_items.front());
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_inventory_cell)
		{
			P_object_owner* temp = static_cast<P_object_owner*>(m_message_queue.m_items.front());
			if (static_cast<Inventory_cell*>(temp->m_cell)->m_item)
			{
				Result = new P_object();
				Result->m_object = static_cast<Inventory_cell*>(temp->m_cell)->m_item;
				Exit = true;
			}
			else Result = nullptr;
		}
		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_body_part)
		{
			P_object_owner* temp = static_cast<P_object_owner*>(m_message_queue.m_items.front());
			if (static_cast<Object_part*>(temp->m_cell)->m_item)
			{
				Result = new P_object();
				Result->m_object = static_cast<Object_part*>(temp->m_cell)->m_item;
				Exit = true;
			}
			else Result = nullptr;
		}
		if (m_message_queue.m_items.front()->m_kind == ParameterKind_Cancel)
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

Parameter* Application::command_select_body_part()
{
	Parameter* Result = nullptr;
	m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выберите назначение."));
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
		if (m_message_queue.m_items.front()->m_kind == ParameterKind::parameter_kind_body_part)
		{
			Result = m_message_queue.m_items.front();
			Exit = true;
		}
		if (m_message_queue.m_items.front()->m_kind == ParameterKind_Cancel)
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

void Application::command_equip(GameObject*& unit, Object_part* part, GameObject*& object)
{
	part->m_item = object;
	object->m_owner = part;
	Object_state* obj_state = object->get_state(object_state_e::equip);
	if (obj_state)
	{
		if (object->m_active_state)
		{
			for (auto kind = object->m_active_state->m_item.begin(); kind != object->m_active_state->m_item.end(); kind++)
			{
				if (kind->first != interaction_e::damage)
				{
					//unit->add_effect(kind->first, kind->second);
					unit->add_from(kind->first, static_cast<Interaction_list*>(kind->second));
				}
			}
		}
	}
	unit->update_interaction();
	unit->event_update(VoidEventArgs());
	if (m_GUI->MapViewer->m_player->m_object == unit) { update_action_panel(); }
}

void Application::command_unequip(GameObject*& unit, Object_part* part, GameObject*& object)
{
	part->m_item = nullptr;
	object->m_owner = nullptr;
	Object_state* obj_state = object->get_state(object_state_e::equip);
	if (obj_state)
	{
		if (object->m_active_state)
		{
			for (auto kind = object->m_active_state->m_item.begin(); kind != object->m_active_state->m_item.end(); kind++)
			{
				if (kind->first != interaction_e::damage)
				{
					//unit->remove_effect(kind->first, kind->second);
					unit->remove_from(kind->first, static_cast<Interaction_list*>(kind->second));
				}
			}
		}
	}
	unit->update_interaction();
	unit->event_update(VoidEventArgs());
	if (m_GUI->MapViewer->m_player->m_object == unit) { update_action_panel(); }
}

void Application::console(std::string text)
{
	m_GUI->DescriptionBox->add_item_control(new GUI_Text(text,new GUI_TextFormat(8,17,RGBA_t(1.0,1.0,1.0,1.0))));
}

