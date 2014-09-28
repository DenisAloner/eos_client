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

void GameObjectManager::parser(const std::string& command)
{
	std::size_t found = 0;
	std::string args;
	command_e key;
	found = command.find(" ");
	if (found != std::string::npos)
	{
		//MessageBox(NULL, command.substr(0, found).c_str(), "", MB_OK);
		key = m_commands.find(command.substr(0, found))->second;
		args = command.substr(found + 1);
	}
	else {
		key = m_commands.find(command)->second;
	}
	switch (key)
	{
	case command_e::obj:
	{
		m_object = new GameObject();
		m_object->m_name = args;
		m_items.insert(std::pair<std::string, GameObject*>(args, m_object));
		break;
	}
	case command_e::state_default:
	{
		m_object->m_active_state = new Game_state();
		m_object->m_active_state->m_state = state_e::default;
		m_object->m_state.push_back(m_object->m_active_state);
		break;
	}
	case command_e::state_on:
	{
		m_object->m_active_state = new Game_state();
		m_object->m_active_state->m_state = state_e::on;
		m_object->m_state.push_back(m_object->m_active_state);
		break;
	}
	case command_e::state_off:
	{
		m_object->m_active_state = new Game_state();
		m_object->m_active_state->m_state = state_e::off;
		m_object->m_state.push_back(m_object->m_active_state);
		break;
	}
	case command_e::ai:
	{
		AI_configuration* ai = new AI_configuration;
		std::size_t pos = 0;
		found = args.find(" ");
		ai->m_fov_radius = std::stoi(args.substr(0, found));
		pos = found + 1;
		found = args.find(" ", pos);
		ai->m_fov_qualifier = Application::instance().m_ai_manager->m_fov_qualifiers[std::stoi(args.substr(pos, found - pos))];
		pos = found + 1;
		ai->m_path_qualifier = Application::instance().m_ai_manager->m_path_qualifiers[std::stoi(args.substr(pos))];
		m_object->m_active_state->m_ai = ai;
		break;
	}
	case command_e::size:
	{
		std::size_t pos = 0;
		game_object_size_t size;
		found = args.find(" ");
		size.x = std::stoi(args.substr(0, found));
		pos = found + 1;
		found = args.find(" ", pos);
		size.y = std::stoi(args.substr(pos, found - pos));
		pos = found + 1;
		size.z = std::stoi(args.substr(pos));
		m_object->m_active_state->m_size = size;
		m_object->set_direction(ObjectDirection_Down);
		break;
	}
	case command_e::weight:
	{
		m_object->m_weight = std::stof(args);
		break;
	}
	case command_e::layer:
	{
		m_object->m_active_state->m_layer = std::stoi(args);
		break;
	}
	case command_e::icon:
	{
		m_object->m_active_state->m_icon = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + args + ".bmp");
		break;
	}
	case command_e::tile_manager_single:
	{
		m_object->m_active_state->m_tile_manager = new TileManager_Single();
		m_object->m_active_state->m_tile_manager->load_from_file(args, ObjectDirection_Down, 0);
		break;
	}
	case command_e::tile_manager_map:
	{	
		if (m_object->m_active_state->m_tile_manager == nullptr)
		{
			m_object->m_active_state->m_tile_manager = new TileManager_Map();
		}
		ObjectDirection dir;
		int frame;
		std::string name;
		std::size_t pos = 0;
		found = args.find(" ");
		name = args.substr(0, found);
		pos = found + 1;
		found = args.find(" ", pos);
		dir = static_cast<ObjectDirection>(std::stoi(args.substr(pos, found - pos)));
		pos = found + 1;
		frame = std::stoi(args.substr(pos));
		m_object->m_active_state->m_tile_manager->load_from_file(name, dir, frame);
		break;
	}
	case command_e::tile_manager_rotating:
	{
		if (m_object->m_active_state->m_tile_manager == nullptr)
		{
			m_object->m_active_state->m_tile_manager = new TileManager_rotating();
		}
		ObjectDirection dir;
		std::string name;
		std::size_t pos = 0;
		found = args.find(" ");
		name = args.substr(0, found);
		pos = found + 1;
		found = args.find(" ", pos);
		dir = static_cast<ObjectDirection>(std::stoi(args.substr(pos, found - pos)));
		m_object->m_active_state->m_tile_manager->load_from_file(name, dir, 0);
		break;
	}
	case command_e::light:
	{
		std::size_t pos = 0;
		light_t* light = new light_t( 0, 0, 0);
		found = args.find(" ");
		light->R = std::stoi(args.substr(0, found));
		pos = found + 1;
		found = args.find(" ", pos);
		light->G = std::stoi(args.substr(pos, found - pos));
		pos = found + 1;
		light->B = std::stoi(args.substr(pos));
		m_object->m_active_state->m_light = light;
		break;
	}
	case command_e::action_move:
	{
		m_object->m_active_state->m_actions.push_back(Application::instance().m_actions[action_e::move]);
		break;
	}
	case command_e::property_permit_move:
	{
		m_object->m_active_state->m_properties.push_back(new GameObjectProperty(property_e::permit_move));
		break;
	}
	case command_e::property_permit_pick:
	{
		m_object->m_active_state->m_properties.push_back(new GameObjectProperty(property_e::permit_pick));
		break;
	}
	case command_e::property_container:
	{
		std::size_t pos = 0;
		std::string name;
		int x;
		int y;
		found = args.find(" ");
		name = args.substr(0, found);
		pos = found + 1;
		found = args.find(" ", pos);
		x = std::stoi(args.substr(pos, found - pos));
		pos = found + 1;
		y = std::stoi(args.substr(pos));
		m_object->m_active_state->m_properties.push_back(new Property_Container(x, y, name));
		break;
	}
	case command_e::property_strenght:
	{
		m_object->m_active_state->m_properties.push_back(new GameObjectParameter(property_e::strength, std::stof(args)));
		break;
	}
	case command_e::property_health:
	{
		m_object->m_active_state->m_properties.push_back(new Game_object_feature(property_e::health, std::stoi(args)));
		break;
	}
	}
}

game_clipboard::game_clipboard():m_item(nullptr){}

void GameObjectManager::init()
{
	m_commands.insert(std::pair<std::string, command_e>("object", command_e::obj));
	m_commands.insert(std::pair<std::string, command_e>("state_default", command_e::state_default));
	m_commands.insert(std::pair<std::string, command_e>("state_on", command_e::state_on));
	m_commands.insert(std::pair<std::string, command_e>("state_off", command_e::state_off));
	m_commands.insert(std::pair<std::string, command_e>("size", command_e::size));
	m_commands.insert(std::pair<std::string, command_e>("ai", command_e::ai));
	m_commands.insert(std::pair<std::string, command_e>("weight", command_e::weight));
	m_commands.insert(std::pair<std::string, command_e>("layer", command_e::layer));
	m_commands.insert(std::pair<std::string, command_e>("icon", command_e::icon));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_single", command_e::tile_manager_single));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_map", command_e::tile_manager_map));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_rotating", command_e::tile_manager_rotating));
	m_commands.insert(std::pair<std::string, command_e>("light", command_e::light));
	m_commands.insert(std::pair<std::string, command_e>("action_move", command_e::action_move));
	m_commands.insert(std::pair<std::string, command_e>("property_permit_move", command_e::property_permit_move));
	m_commands.insert(std::pair<std::string, command_e>("property_permit_pick", command_e::property_permit_pick));
	m_commands.insert(std::pair<std::string, command_e>("property_container", command_e::property_container));
	m_commands.insert(std::pair<std::string, command_e>("property_strength", command_e::property_strenght));
	m_commands.insert(std::pair<std::string, command_e>("property_health", command_e::property_health));

	bytearray buffer;
	FileSystem::instance().load_from_file(FileSystem::instance().m_resource_path + "Configs\\Objects.txt", buffer);
	std::string config(buffer);
	std::size_t pos = 0;
	std::size_t found = 0;
	while (pos != std::string::npos)
	{
		found = config.find("\r\n", pos);
		if (found != std::string::npos)
		{
			if (found - pos > 0)
			{
				parser(config.substr(pos, found - pos));

			}
			pos = found + 2;
		}
		else {
			parser(config.substr(pos));
			pos = std::string::npos;
		}
	}

};

GameObject* GameObjectManager::new_object(std::string unit_name)
{
	GameObject* obj = new GameObject();
	items_t::iterator it = m_items.find(unit_name);
	if (it == m_items.end())
	{
		LOG(FATAL) << "������� `" << unit_name << "` ����������� � m_items";
	}
	GameObject* config = m_items.find(unit_name)->second;
	obj->m_direction = config->m_direction;
	obj->m_name = config->m_name;
	obj->m_weight = config->m_weight;
	Game_state* state;
	for (std::list<Game_state*>::iterator item = config->m_state.begin(); item != config->m_state.end(); item++)
	{
		state = new Game_state();
		obj->m_state.push_back(state);
		(*state) = (*(*item));
	}
	obj->m_active_state = obj->m_state.front();
	return obj;
}


Application::Application()
: m_timer(new Timer(8, 75))
{
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
//#warning TODO ���������� �� ���������� �����!
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

	m_GUI = new ApplicationGUI();
	m_GUI->m_position = position_t(0, 0);
	m_GUI->m_size = dimension_t(1024, 1024);
	
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
	m_ai_manager = new AI_manager();
	m_game_object_manager.init();

	m_GUI->MapViewer = new GUI_MapViewer(this);
	m_GUI->MapViewer->m_position.x = 0;
	m_GUI->MapViewer->m_position.y = 0;
	m_GUI->MapViewer->m_size.w = 1024;
	m_GUI->MapViewer->m_size.h = 1024;
	
	m_GUI->MapViewer->m_map = new GameMap(dimension_t(200,200));
	m_GUI->MapViewer->m_map->generate_level();
	

	int index = rand() % m_GUI->MapViewer->m_map->m_link_rooms.size();
	GameMap::block_t* room = *std::next(m_GUI->MapViewer->m_map->m_link_rooms.begin(), index);
	int rx = rand() % room->rect.w;
	int ry = rand() % room->rect.h;
	GameObject* obj = m_game_object_manager.new_object("elf");
	obj->set_direction(ObjectDirection_Left);
	m_GUI->MapViewer->m_map->add_object(obj, m_GUI->MapViewer->m_map->m_items[room->rect.y + ry][room->rect.x + rx]);
	m_GUI->MapViewer->m_player = new Player(obj, m_GUI->MapViewer->m_map);
	
	GUI_ActionManager* AMTextBox;
	AMTextBox = new GUI_ActionManager(m_action_manager);
	AMTextBox->m_position.x = 650;
	AMTextBox->m_position.y = 710;
	AMTextBox->resize(372, 263);
	GUI_ActionPanel* ActionPanel;
	ActionPanel = new GUI_ActionPanel(2,975,898,47);
	GUI_ActionButton* ActionButton;
	ActionButton = new GUI_ActionButton();
	ActionButton->m_action = m_actions[action_e::move];
	ActionPanel->add_item_control(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->m_action = m_actions[action_e::push];
	ActionPanel->add_item_control(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->m_action = m_actions[action_e::turn];
	ActionPanel->add_item_control(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->m_action = m_actions[action_e::open_inventory];
	ActionPanel->add_item_control(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->m_action = m_actions[action_e::cell_info];
	ActionPanel->add_item_control(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->m_action = m_actions[action_e::set_motion_path];
	ActionPanel->add_item_control(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->m_action = m_actions[action_e::pick];
	ActionPanel->add_item_control(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->m_action = m_actions[action_e::open];
	ActionPanel->add_item_control(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->m_action = m_actions[action_e::hit];
	ActionPanel->add_item_control(ActionButton);
	GUI_Layer* MenuLayer;
	MenuLayer = new GUI_Layer();
	MenuLayer->m_position.x = 0;
	MenuLayer->m_position.y = 0;
	MenuLayer->m_size.w = 1024;
	MenuLayer->m_size.h = 1024;
	GUI_TextBox* TextBox = new GUI_TextBox();
	TextBox->m_position.x = 2;
	TextBox->m_position.y = 710;
	TextBox->resize(646, 263);

	m_GUI->DescriptionBox = TextBox;

	m_GUI->Timer = new GUI_Timer(902, 975, 120, 47, 0);

	MenuLayer->add(AMTextBox);
	MenuLayer->add(ActionPanel);
	MenuLayer->add(TextBox);
	GUI_Window* MiniMap = new GUI_Window(0, 0, 400, 400, "����-�����");
	GUI_MiniMap* mini_map = new GUI_MiniMap(position_t(5, 30), dimension_t(MiniMap->m_size.w - 10, MiniMap->m_size.h - 35), m_GUI->MapViewer);
	MiniMap->add_item_control(mini_map);
	MenuLayer->add(MiniMap);

	MiniMap = new GUI_Window(300, 0, 400, 400, "���� ������");
	GUI_FOV* fov = new GUI_FOV(position_t(5, 30), dimension_t(MiniMap->m_size.w - 10, MiniMap->m_size.h - 35), m_GUI->MapViewer->m_player->m_fov);
	MiniMap->add_item_control(fov);

	MenuLayer->add(m_GUI->Timer);
	MenuLayer->add(MiniMap);
	
	
	//MenuLayer->add(new GUI_Item(0, 0, 100,21, "4565656"));
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

	int ru;
	for (int i = 0; i <15; i++){
		index = rand() % m_GUI->MapViewer->m_map->m_link_rooms.size();
		room = *std::next(m_GUI->MapViewer->m_map->m_link_rooms.begin(), index);
		ru = rand() % 3;
		rx = rand() % room->rect.w;
		ry = rand() % room->rect.h;
		switch (ru)
		{
		case 0:
		{
			obj = m_game_object_manager.new_object("snake");
			break;
		}
		case 1:
		{
			obj = m_game_object_manager.new_object("bear");
			break;
		}
		case 2:
		{
			obj = m_game_object_manager.new_object("ghost");
			break;
		}
		}
		obj->set_direction(ObjectDirection_Left);
		m_GUI->MapViewer->m_map->add_ai_object(obj, m_GUI->MapViewer->m_map->m_items[room->rect.y + ry][room->rect.x + rx]);
	}

	m_ready = true;
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
	m_update_mutex.lock();
	if(!m_action_manager->m_items.empty())
	{
		for (std::map<GameObject*, std::list<GameTask*> >::iterator current = m_action_manager->m_items.begin(); current != m_action_manager->m_items.end(); current++)
		{
			if (!current->second.empty())
			{
				GameTask* A;
				A = current->second.front();
				A->m_action->perfom(A->m_parameter);
				if (A->m_action->m_error != "")
				{
					m_GUI->DescriptionBox->add_item_control(new GUI_Text(A->m_action->m_error));
				}
				m_action_manager->remove(current->first);
			}
		}
	}
	m_GUI->MapViewer->m_map->calculate_lighting();
	m_GUI->MapViewer->m_map->calculate_ai();
	LOG(INFO) << "Player radius " << m_GUI->MapViewer->m_player->m_object->m_active_state->m_ai->m_fov_radius;
	m_GUI->MapViewer->m_player->m_fov->calculate(m_GUI->MapViewer->m_player->m_object->m_active_state->m_ai->m_fov_radius, m_GUI->MapViewer->m_player->m_object, m_GUI->MapViewer->m_map);
	Application::instance().m_GUI->MapViewer->update();
	Application::instance().m_GUI->MapViewer->m_map->m_update = true;
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
	m_GUI->DescriptionBox->add_item_control(new GUI_Text("�������� ������."));
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
	m_GUI->DescriptionBox->add_item_control(new GUI_Text("�������� ������."));
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

bool Application::command_open_inventory(GameObject*& Object)
{
	//MessageBox(NULL, "In_1", "", MB_OK);
	bool Result = false;
	Property_Container* Property = static_cast<Property_Container*>(Object->m_active_state->find_property(property_e::container));
	if (Property != nullptr)
	{
		//MessageBox(NULL, "In_2", "", MB_OK);
		GUI_Window* Window = new GUI_Window(1024 / 2 - (Property->m_size.w * 64 + 2) / 2, 1024 / 2 - (Property->m_size.h * 64 + 2) / 2, Property->m_size.w * 64 + 4, Property->m_size.h * 64 + 27,Object->m_name+"::"+Property->m_name);
		GUI_Inventory* Inv = new GUI_Inventory(Property);
		Inv->m_position.x = 2;
		Inv->m_position.y = Window->m_size.h - Inv->m_size.h-2;
		Window->add_item_control(Inv);
		m_GUI->add_front(Window);
		Result = true;
	}
	return Result;
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
	std::function<bool(GameObject*)> qualifier = object->m_active_state->m_ai->m_path_qualifier;
	for (int i = 0; i<object->m_active_state->m_size.y; i++)
	{
		for (int j = 0; j<object->m_active_state->m_size.x; j++)
		{
			if (map->m_items[position->y + i][position->x - j] == nullptr){ return false; }
			for (std::list<GameObject*>::iterator item = map->m_items[position->y + i][position->x - j]->m_items.begin(); item != map->m_items[position->y + i][position->x - j]->m_items.end(); item++)
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
	int time = 1;
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
			m_GUI->DescriptionBox->add_item_control(new GUI_Text("��� - " + std::to_string(m_game_turn) + ".", new GUI_TextFormat(10, 19, RGBA_t(0.0, 0.8, 0.0, 1.0))));
			m_game_turn += 1;
		}
		std::chrono::milliseconds Duration(50);
		std::this_thread::sleep_for(Duration);
	}
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
	m_GUI->DescriptionBox->add_item_control(new GUI_Text("�������� ����������."));
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
	m_GUI->DescriptionBox->add_item_control(new GUI_Text("�������� ������."));
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
			P_inventory_cell* temp = static_cast<P_inventory_cell*>(m_message_queue.m_items.front());
			if (temp->m_cell->m_item)
			{
				Result = new P_object();
				Result->m_object = temp->m_cell->m_item;
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