#include "Application.h"

void my_audio_callback(void *userdata, Uint8 *stream, int len);

// variable declarations
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play


gui_MessageQueue::gui_MessageQueue()
{
	Reader = false;
	ReadMessage = false;
	ProcessedMessage = false;
	Busy = false;
}
gui_MessageQueue::~gui_MessageQueue()
{
}

void gui_MessageQueue::Push(TParameter* p)
{
	std::unique_lock<std::mutex> lk(m);
	ReadMessage = true;
	items.push_back(p);
	cv.notify_one();
	while (!ProcessedMessage)
	{
		cv.wait(lk);
	}
	ReadMessage = false;
	ProcessedMessage = false;
	items.pop_front();
}

Application::Application()
{
}


Application::~Application(void)
{
}

void Application::OnKeyPress(WPARAM w)
{
	GUI->KeyPress(w);
}

void Application::OnMouseClick(MouseEventArgs const& e)
{
	/*char buf2[32];
	itoa(e.Position.x, buf2, 10);
	MessageBox(NULL, buf2, "", MB_OK);*/
	if (e.Flags &MK_LBUTTON)
	{
		GUI->MouseClick(MouseEventArgs(Point(e.Position.x, e.Position.y), MK_LBUTTON));
	}
	if (e.Flags &MK_RBUTTON)
	{
		GUI->MouseClick(MouseEventArgs(Point(e.Position.x, e.Position.y), MK_RBUTTON));
	}
}

void Application::OnMouseDown(MouseEventArgs const& e)
{
	GUI->MouseDown(e);
}

void Application::OnMouseMove(MouseEventArgs const& e)
{
	GUI->MouseMove(e);
}

void Application::OnMouseWheel(MouseEventArgs const& e)
{
	GUI->MouseWheel(e);
}

void Application::OnUnderCursor(MouseEventArgs const& e)
{
	GUI->UnderCursor(e);
}

void Application::Render()
{
	UpdaterLock.lock();
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	glPushMatrix();
	GUI->RenderAt(Graph,0,0);
	Point Mouse=Application::Instance().mouse->GetMouseCoordinat();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	if (mouse->ShowCursor)
	{
		glBindTexture(GL_TEXTURE_2D, mouse->Cursor);
		Graph->DrawSprite(Mouse.x, Mouse.y, Mouse.x, Mouse.y + 48, Mouse.x + 48, Mouse.y + 48, Mouse.x + 48, Mouse.y);
	}
	glPopMatrix();
	UpdaterLock.unlock();
}

void Application::Initialization(HWND _hWnd)
{
	Ready = false;
	Graph = new GraphicalController();
	mouse = new MouseController_Windows(_hWnd);
	Command_SetCursor(Graph->Sprites[21]);
	Command_SetCursorVisibility(true);

	GUI = new ApplicationGUI();
	GUI->x = 0;
	GUI->y = 0;
	GUI->width = 1024;
	GUI->height = 1024;
	
	KeyPress += std::bind(&Application::OnKeyPress, this, std::placeholders::_1);
	mouse->MouseClick += std::bind(&Application::OnMouseClick, this, std::placeholders::_1);
	mouse->MouseDown += std::bind(&Application::OnMouseDown, this, std::placeholders::_1);
	mouse->MouseWheel += std::bind(&Application::OnMouseWheel, this, std::placeholders::_1);
	mouse->MouseMove += std::bind(&Application::OnMouseMove, this, std::placeholders::_1);
	UnderCursor += std::bind(&Application::OnUnderCursor, this, std::placeholders::_1);
	ActionManager = new TActionManager();

	Actions[0]=new ActionClass_Move();
	Actions[1] = new ActionClass_Push();
	Actions[2] = new ActionClass_Turn();
	Actions[3] = new Action_OpenInventory();

	GUI->MapViewer = new GUI_MapViewer(this);
	GUI->MapViewer->x = 0;
	GUI->MapViewer->y = 0;
	GUI->MapViewer->width = 1024;
	GUI->MapViewer->height = 1024;
	GUI->MapViewer->Map = new GameMap(this);
	GUI->MapViewer->Map->GenerateRoom();
	GUI->MapViewer->Player = new TPlayer(this);
	GUI->MapViewer->Map->AddObject(GUI->MapViewer->Player, GUI->MapViewer->Map->Items[9][9]);

	
	//TGCButton* Button;
	//Button = new TGCButton();
	//Button->x = 100;
	//Button->y = 100;
	//Button->width = 1024;
	//Button->height =1024;
	//TGCTextBox* TextBox1;
	//TextBox1 = new TGCTextBox();
	//TextBox1->Text = "";
	//TextBox1->x = 0;
	//TextBox1->y = 774 - 64;
	//TextBox1->width = 640;
	//TextBox1->height = 250;

	GUI_ActionManager* AMTextBox;
	AMTextBox = new GUI_ActionManager(ActionManager);
	AMTextBox->x = 650;
	AMTextBox->y = 710;
	AMTextBox->Resize(372, 263);
	GUI_ActionPanel* ActionPanel;
	ActionPanel = new GUI_ActionPanel(2,975,898,47);
	GUI_ActionButton* ActionButton;
	ActionButton = new GUI_ActionButton();
	ActionButton->Action = Actions[ActionKind_Move];
	ActionPanel->AddItemControl(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->Action = Actions[ActionKind_Push];
	ActionPanel->AddItemControl(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->Action = Actions[ActionKind_Turn];
	ActionPanel->AddItemControl(ActionButton);
	ActionButton = new GUI_ActionButton();
	ActionButton->Action = Actions[ActionKind_OpenInventory];
	ActionPanel->AddItemControl(ActionButton);
	GUI_Layer* MenuLayer;
	MenuLayer = new GUI_Layer();
	MenuLayer->x = 0;
	MenuLayer->y = 0;
	MenuLayer->width = 1024;
	MenuLayer->height = 1024;
	GUI_TextBox* TextBox = new GUI_TextBox();
	TextBox->x = 2;
	TextBox->y = 710;
	TextBox->Resize(646, 263);

	GUI->DescriptionBox = TextBox;

	GUI->Timer = new GUI_Timer(902, 975, 120, 47, 0);

	MenuLayer->add(AMTextBox);
	MenuLayer->add(ActionPanel);
	MenuLayer->add(TextBox);
	MenuLayer->add(GUI->Timer);
	
	//MenuLayer->add(new GUI_Item(0, 0, 100,21, "4565656"));
	GUI->add(MenuLayer);
	GUI->add(GUI->MapViewer);
	GUI->MapViewer->GUI = MenuLayer;
	MenuLayer->Name = "Menu";
	GUI->Name = "GUI";

	TTorch* Torch1;
	Torch1 = new TTorch(this);
	/*Torch1->Light->RGB[0] = 1.0;
	Torch1->Light->RGB[1] = 0.0;
	Torch1->Light->RGB[2] = 0.0;*/
	GUI->MapViewer->Map->AddNewObject(Torch1, GUI->MapViewer->Map->Items[1][1]);
	Torch1 = new TTorch(this);
	/*Torch1->Light->RGB[0] = 0.0;
	Torch1->Light->RGB[1] = 1.0;
	Torch1->Light->RGB[2] = 0.0;*/
	GUI->MapViewer->Map->AddNewObject(Torch1, GUI->MapViewer->Map->Items[58][38]);
	Torch1 = new TTorch(this);
	/*Torch1->Light->RGB[0] = 0.0;
	Torch1->Light->RGB[1] = 0.0;
	Torch1->Light->RGB[2] = 1.0;*/
	GUI->MapViewer->Map->AddNewObject(Torch1, GUI->MapViewer->Map->Items[58][1]);
	GUI->MapViewer->Map->AddNewObject(new TTorch(this), GUI->MapViewer->Map->Items[1][38]);
	GUI->MapViewer->Map->AddNewObject(new TTorch(this), GUI->MapViewer->Map->Items[29][1]);
	GUI->MapViewer->Map->AddNewObject(new TTorch(this), GUI->MapViewer->Map->Items[1][19]);
	GUI->MapViewer->Map->AddNewObject(new TTorch(this), GUI->MapViewer->Map->Items[29][38]);
	GUI->MapViewer->Map->AddNewObject(new TTorch(this), GUI->MapViewer->Map->Items[58][19]);
	GUI->MapViewer->Map->AddObject(new Elf(this), GUI->MapViewer->Map->Items[14][14]);
	GUI->MapViewer->Map->AddObject(new TBox(this), GUI->MapViewer->Map->Items[30][10]);
	Graph->setVSync(false);
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
		music = Mix_LoadWAV("C:\\ExplorersOfSaarum\\Click.wav");

		/* This begins playing the music - the first argument is a
		pointer to Mix_Music structure, and the second is how many
		times you want it to loop (use -1 for infinite, and 0 to
		have it just play once) */
	}
	Ready = true;
}

void Application::PlaySound1()
{
	Mix_PlayChannel(-1, music, 0);
}

void Application::Update(void)
{
	UpdaterLock.lock();
	if(!ActionManager->Actions.empty())
	{
		GameTask* A;
		A=ActionManager->Actions.front();
		A->Action->perfom(A->Parameter);
		ActionManager->Remove();
	}
	UpdaterLock.unlock();
}

bool Application::Command_SelectLocation(GameObject* Object, MapCell*& Cell)
{
	bool Result = false;
	GUI->MapViewer->CursorX = Object->Size.x;
	GUI->MapViewer->CursorY = Object->Size.y;
	GUI->DescriptionBox->AddItemControl(new GUI_Text("Выберите клетку."));
	bool Exit = false;
	while (Exit == false)
	{
		std::unique_lock<std::mutex> lk(MessageQueue.m);
		MessageQueue.Reader = true;
		while (!MessageQueue.ReadMessage)
		{
			MessageQueue.cv.wait(lk);
		}
		//MessageBox(NULL, "Co", "", MB_OK);
		MessageQueue.ProcessedMessage = true;
		if (MessageQueue.items.front()->kind == ParameterKind_MapCell)
		{
			Cell = static_cast<Parameter_MapCell*>(MessageQueue.items.front())->place;
			Exit = true;
			Result = true;
		}
		if (MessageQueue.items.front()->kind == ParameterKind_Cancel)
		{
			Exit = true;
		}
		MessageQueue.ReadMessage = false;
		lk.unlock();
		MessageQueue.cv.notify_one();
		MessageQueue.Reader = false;
	}
	GUI->MapViewer->CursorX = 1;
	GUI->MapViewer->CursorY = 1;
	return Result;
}

bool Application::Command_SelectObject(GameObject*& Object)
{
	bool Result = false;
	GUI->MapViewer->CursorX = 1;
	GUI->MapViewer->CursorY = 1;
	GUI->DescriptionBox->AddItemControl(new GUI_Text("Выберите обьект."));
	bool Exit = false;
	while (Exit == false)
	{
		std::unique_lock<std::mutex> lk(MessageQueue.m);
		MessageQueue.Reader = true;
		while (!MessageQueue.ReadMessage)
		{
			MessageQueue.cv.wait(lk);
		}
		MessageQueue.ProcessedMessage = true;

		if (MessageQueue.items.front()->kind == ParameterKind_GameObject)
		{
			Object = static_cast<Parameter_GameObject*>(MessageQueue.items.front())->object;
			Exit = true;
			Result = true;
		}
		if (MessageQueue.items.front()->kind == ParameterKind_Cancel)
		{
			Exit = true;
		}
		MessageQueue.ReadMessage = false;
		lk.unlock();
		MessageQueue.cv.notify_one();
		MessageQueue.Reader = false;
	}
	return Result;
}

bool Application::Command_OpenInventory(GameObject*& Object)
{
	//MessageBox(NULL, "In_1", "", MB_OK);
	bool Result = false;
	Property_Container* Property = static_cast<Property_Container*>(Object->FindProperty(PropertyKind_Container));
	if (Property != nullptr)
	{
		//MessageBox(NULL, "In_2", "", MB_OK);
		GUI_Window* Window = new GUI_Window(1024 / 2 - (Property->Width * 48 + 2) / 2, 1024 / 2 - (Property->Height * 48 + 2) / 2, Property->Width * 48 + 4, Property->Height * 48 + 27,Object->Name+"::"+Property->Name);
		GUI_Inventory* Inv = new GUI_Inventory(Property);
		Inv->x = 2;
		Inv->y = Window->height - Inv->height-2;
		Window->AddItemControl(Inv);
		GUI->AddFront(Window);
		Result = true;
	}
	return Result;
}

void Application::Command_SetCursor(GLuint _Tile)
{
	mouse->Cursor = _Tile;
}

void Application::Command_SetPickUpItem(GLuint _Tile)
{
	mouse->PickUpItem = _Tile;
}

void Application::Command_SetCursorVisibility(bool _Visibility)
{
	mouse->ShowCursor = _Visibility;
}

void Application::Command_SetPickUpItemVisibility(bool _Visibility)
{
	mouse->ShowPickUpItem = _Visibility;
}

bool Application::Command_CheckPosition(GameObject*& _Object, MapCell*& _Position, GameMap*& _Map)
{
	for (int i = 0; i<_Object->Size.y; i++)
	{
		for (int j = 0; j<_Object->Size.x; j++)
		{
			if (_Map->Items[_Position->y + i][_Position->x - j] == nullptr){ return false; }
			if (_Map->Items[_Position->y + i][_Position->x - j]->FindProperty(PropertyKind_Impassable, _Object) != nullptr)
			{
				return false;
			}
		}
	}
	return true;
}

void my_audio_callback(void *userdata, Uint8 *stream, int len) {

	if (audio_len == 0)
		return;

	len = (len > audio_len ? audio_len : len);
	SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	//SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

	audio_pos += len;
	audio_len -= len;
}