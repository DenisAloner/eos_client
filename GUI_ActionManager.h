#pragma once
#include "GUI_Container.h"
#include "TActionManager.h"
#include "GUI_Item.h"

class Application;
class GUI_Object;
class TActionManager;
struct ActionManagerEventArgs;
class GameTask;

class GUI_ActionManager :
	public GUI_Container

{
public:

	TActionManager* Item;

	GUI_ActionManager(TActionManager* ActionManager);
	~GUI_ActionManager(void);

	/*virtual void Render(GraphicalController* Graph);*/
	virtual void OnItemAdd(GameTask* const& e);
	virtual void OnItemRemove(GameTask* const& e);
	void RemoveItemFromSource(GUI_Object* object);
	virtual void AddItemControl(GUI_Object* object);
	virtual void RemoveItemControl(GUI_Object* object);
};

