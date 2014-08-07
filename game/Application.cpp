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
		m_object->m_size = size;
		break;
	}
	case command_e::layer:
	{
		m_object->m_layer = std::stoi(args);
		break;
	}
	case command_e::tile_manager_single:
	{
		m_object->m_tile_manager = new TileManager_Single();
		m_object->m_tile_manager->load_from_file(args);
		break;
	}
	case command_e::tile_manager_map:
	{
		m_object->m_tile_manager = new TileManager_Map();
		m_object->m_tile_manager->load_from_file(args);
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
		m_object->m_light = light;
		break;
	}
	case command_e::action_move:
	{
		m_object->m_actions.push_back(Application::instance().m_actions[action_e::move]);
		break;
	}
	case command_e::property_permit_move:
	{
		m_object->m_properties.push_back(new GameObjectProperty(property_e::permit_move));
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
		m_object->m_properties.push_back(new Property_Container(x, y, name));
		break;
	}
	}
}

void GameObjectManager::init()
{
	m_commands.insert(std::pair<std::string, command_e>("object", command_e::obj));
	m_commands.insert(std::pair<std::string, command_e>("size", command_e::size));
	m_commands.insert(std::pair<std::string, command_e>("layer", command_e::layer));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_single", command_e::tile_manager_single));
	m_commands.insert(std::pair<std::string, command_e>("tile_manager_map", command_e::tile_manager_map));
	m_commands.insert(std::pair<std::string, command_e>("light", command_e::light));
	m_commands.insert(std::pair<std::string, command_e>("action_move", command_e::action_move));
	m_commands.insert(std::pair<std::string, command_e>("property_permit_move", command_e::property_permit_move));
	m_commands.insert(std::pair<std::string, command_e>("property_container", command_e::property_container));

	GameObject* obj;
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
	(*obj) = (*config);
	return obj;
}

Application::Application()
{
}

Application::~Application(void)
{
}

void Application::on_key_press(WPARAM w)
{
	m_GUI->key_press(w);
}

void Application::on_mouse_click(MouseEventArgs const& e)
{
	/*char buf2[32];
	itoa(e.Position.x, buf2, 10);
	MessageBox(NULL, buf2, "", MB_OK);*/
	if (e.flags &MK_LBUTTON)
	{
		m_GUI->mouse_click(MouseEventArgs(position_t(e.position.x, e.position.y), MK_LBUTTON));
	}
	if (e.flags &MK_RBUTTON)
	{
		m_GUI->mouse_click(MouseEventArgs(position_t(e.position.x, e.position.y), MK_RBUTTON));
	}
}

void Application::on_mouse_down(MouseEventArgs const& e)
{
	m_GUI->mouse_down(e);
}

void Application::on_mouse_move(MouseEventArgs const& e)
{
	m_GUI->mouse_move(e);
}

void Application::on_mouse_wheel(MouseEventArgs const& e)
{
	m_GUI->mouse_wheel(e);
}

void Application::render()
{
	m_update_mutex.lock();
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	glPushMatrix();
	m_GUI->render(m_graph,0,0);
	position_t Mouse = Application::instance().m_mouse->get_mouse_position();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	if (m_mouse->m_show_cursor)
	{
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, m_mouse->m_cursor);
		m_graph->draw_sprite(Mouse.x, Mouse.y, Mouse.x, Mouse.y + 48, Mouse.x + 48, Mouse.y + 48, Mouse.x + 48, Mouse.y);
	}
	glPopMatrix();
	m_update_mutex.unlock();
}

void Application::initialization(HWND _hWnd)
{
	music = NULL;
	m_game_turn = 1;
	m_ready = false;
	m_graph = new GraphicalController();
	m_mouse = new MouseController_Windows(_hWnd);
	command_set_cursor(m_graph->m_cursor);
	command_set_cursor_visibility(true);

	m_GUI = new ApplicationGUI();
	m_GUI->m_position = position_t(0, 0);
	m_GUI->m_size = dimension_t(1024,1024);
	
	key_press += std::bind(&Application::on_key_press, this, std::placeholders::_1);
	m_mouse->mouse_click += std::bind(&Application::on_mouse_click, this, std::placeholders::_1);
	m_mouse->mouse_down += std::bind(&Application::on_mouse_down, this, std::placeholders::_1);
	m_mouse->mouse_wheel += std::bind(&Application::on_mouse_wheel, this, std::placeholders::_1);
	m_mouse->mouse_move += std::bind(&Application::on_mouse_move, this, std::placeholders::_1);
	m_action_manager = new ActionManager();

	m_actions[0] = new ActionClass_Move();
	m_actions[1] = new ActionClass_Push();
	m_actions[2] = new ActionClass_Turn();
	m_actions[3] = new Action_OpenInventory();
	m_actions[4] = new Action_CellInfo();
	m_game_object_manager.init();

	m_GUI->MapViewer = new GUI_MapViewer(this);
	m_GUI->MapViewer->m_position.x = 0;
	m_GUI->MapViewer->m_position.y = 0;
	m_GUI->MapViewer->m_size.w = 1024;
	m_GUI->MapViewer->m_size.h = 1024;
	
	m_GUI->MapViewer->m_map = new GameMap(dimension_t(200,200));
	m_GUI->MapViewer->m_map->generate_level();
	m_GUI->MapViewer->m_player = m_game_object_manager.new_object("elf");
	if (m_GUI->MapViewer->m_map->m_items[9][9] == nullptr)
	{
		m_GUI->MapViewer->m_map->m_items[9][9] = new MapCell(9, 9);
		m_GUI->MapViewer->m_map->add_object(m_game_object_manager.new_object("floor"), m_GUI->MapViewer->m_map->m_items[9][9]);
	}
	if (m_GUI->MapViewer->m_map->m_items[9][8] == nullptr)
	{
		m_GUI->MapViewer->m_map->m_items[9][8] = new MapCell(8, 9);
		m_GUI->MapViewer->m_map->add_object(m_game_object_manager.new_object("floor"), m_GUI->MapViewer->m_map->m_items[9][8]);
	}
	m_GUI->MapViewer->m_map->add_object(m_GUI->MapViewer->m_player, m_GUI->MapViewer->m_map->m_items[9][9]);

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
	MenuLayer->add(m_GUI->Timer);
	
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

	m_ready = true;
}

void Application::PlaySound1()
{
	Mix_PlayChannel(-1, music, 0);
}

void Application::update(void)
{
	m_update_mutex.lock();
	if(!m_action_manager->m_items.empty())
	{
		GameTask* A;
		A=m_action_manager->m_items.front();
		A->m_action->perfom(A->m_parameter);
		m_action_manager->remove();
	}
	m_GUI->MapViewer->m_map->calculate_lighting();
	m_update_mutex.unlock();
}

bool Application::command_select_location(GameObject* Object, MapCell*& Cell)
{
	bool Result = false;
	m_GUI->MapViewer->m_cursor_x = Object->m_size.x;
	m_GUI->MapViewer->m_cursor_y = Object->m_size.y;
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
		//MessageBox(NULL, "Co", "", MB_OK);
		m_message_queue.m_processed_message = true;
		if (m_message_queue.m_items.front()->m_kind == ParameterKind_MapCell)
		{
			Cell = static_cast<Parameter_MapCell*>(m_message_queue.m_items.front())->m_place;
			Exit = true;
			Result = true;
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

bool Application::command_select_object(GameObject*& Object)
{
	bool Result = false;
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

		if (m_message_queue.m_items.front()->m_kind == ParameterKind_GameObject)
		{
			Object = static_cast<Parameter_GameObject*>(m_message_queue.m_items.front())->m_object;
			Exit = true;
			Result = true;
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
	Property_Container* Property = static_cast<Property_Container*>(Object->find_property(property_e::container));
	if (Property != nullptr)
	{
		//MessageBox(NULL, "In_2", "", MB_OK);
		GUI_Window* Window = new GUI_Window(1024 / 2 - (Property->m_size.w * 48 + 2) / 2, 1024 / 2 - (Property->m_size.h * 48 + 2) / 2, Property->m_size.w * 48 + 4, Property->m_size.h * 48 + 27,Object->m_name+"::"+Property->m_name);
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

bool Application::command_check_position(GameObject*& _Object, MapCell*& _Position, GameMap*& _Map)
{
	for (int i = 0; i<_Object->m_size.y; i++)
	{
		for (int j = 0; j<_Object->m_size.x; j++)
		{
			if (_Map->m_items[_Position->y + i][_Position->x - j] == nullptr){ return false; }
			if (_Map->m_items[_Position->y + i][_Position->x - j]->find_property(property_e::permit_move, _Object) == nullptr)
			{
				return false;
			}
		}
	}
	return true;
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