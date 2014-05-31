#pragma once

#include "Event.h"
#include "Definiton.h"
#include "GUI_Object.h"
#include "GUI_Layer.h"
#include "TActionManager.h"
#include <mutex>
#include "GraphicalController.h"
#include "GameObject.h"
#include "GameMap.h"
#include <condition_variable>
#include "TParameter.h"
#include "Timer.h"
#include "GUI_ActionPanel.h"
#include "GUI_ActionButton.h"
#include "GUI_ActionManager.h"
#include "GUI_PopMenu.h"
#include "TGCTextBox.h"
#include "TGCButton.h"
#include "MouseController.h"
#include "Description_GUI.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "GUI_Item.h"
#include "GUI_Window.h"
#include "TGCButton.h"
#include "GUI_Timer.h"
#include "Property_Container.h"
#include "GUI_Inventory.h"

class TAction;
class TActionManager;
class GameObject;
class MapCell;
class TParameter;
class TGCTextBox;
class TGCButton;
class GUI_ActionPanel;
class GUI_ActionButton;
class GUI_ActionManager;
class GUI_Layer;
class GUI_Object;
class GUI_PopMenu;
class GameMap;
class GUI_Item;
class GUI_Window;
class TGCButton;
class GUI_Timer;
class Action_OpenInventory;
class MouseController;


class gui_MessageQueue
{
public:

	std::mutex m;
	std::condition_variable cv;
	std::list<TParameter*> items;
	bool ReadMessage;
	bool ProcessedMessage;
	bool Reader;
	bool Busy;

	gui_MessageQueue();
	~gui_MessageQueue();

	void Push(TParameter* p);
};

class Application
{
public:

	int GameTurn = 1;
	bool Ready = false;

	Timer* m_timer;
	MouseController* mouse;
	gui_MessageQueue MessageQueue;

	std::mutex UpdaterLock;

	ApplicationGUI* GUI;
	GraphicalController* Graph;
	TActionManager* ActionManager;

	TAction* Actions[4];

	Event<WPARAM> KeyPress;
	Event<MouseEventArgs> MouseClick;
	Event<MouseEventArgs> UnderCursor;

	Mix_Chunk *music = NULL;

	~Application(void);

	static Application& Instance()
	{
		static Application Singleton;
		return Singleton;
	}

	void Initialization(HWND _hWnd);
	void OnKeyPress(WPARAM w);
	void OnMouseClick(MouseEventArgs const& e);
	void OnMouseDown(MouseEventArgs const& e);
	void OnMouseWheel(MouseEventArgs const& e);
	void OnMouseMove(MouseEventArgs const& e);
	void OnUnderCursor(MouseEventArgs const& e);
	void Render();
	void Update(void);

	bool Command_SelectLocation(GameObject* Object, MapCell*& Cell);
	bool Command_SelectObject(GameObject*& Object);
	bool Command_OpenInventory(GameObject*& Object);
	bool Command_CheckPosition(GameObject*& _Object, MapCell*& _Position,GameMap*& _Map);
	void Command_SetCursor(GLuint _Tile);
	void Command_SetPickUpItem(GLuint _Tile);
	void Command_SetCursorVisibility(bool _Visibility);
	void Command_SetPickUpItemVisibility(bool _Visibility);
	
	void PlaySound1();

private:
	Application();
	Application(const Application& root);
	Application& operator=(const Application&);
	
};

