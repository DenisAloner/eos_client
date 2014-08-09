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

	TActionManager* m_item;

	GUI_ActionManager(TActionManager* ActionManager);
	~GUI_ActionManager(void);

	/*virtual void Render(GraphicalController* Graph);*/
	virtual void on_item_add(GameTask* const& e);
	virtual void on_item_remove(GameTask* const& e);
	void remove_item_from_source(GUI_Object* object);
	virtual void add_item_control(GUI_Object* object);
	virtual void remove_item_control(GUI_Object* object);
};

