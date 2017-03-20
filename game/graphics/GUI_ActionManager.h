#ifndef GUI_ACTIONMANAGER_H
#define	GUI_ACTIONMANAGER_H

#include "GUI_Container.h"
#include "GUI_Item.h"
#include "ActionManager.h"

class Application;
class ActionManager;
class GameTask;
class GUI_Object;

//struct tag_t;
struct ActionManagerEventArgs;

class GUI_ActionManager : public GUI_Scrollable_container
{
public:
	ActionManager* m_item;

	GUI_ActionManager(int x, int y, int width, int height,ActionManager* ActionManager);
	~GUI_ActionManager(void);

	virtual void on_item_add(tag_t const& e);
	virtual void on_item_remove(tag_t const& e);
	void remove_item_from_source(GUI_Object* object);
	void add_item_control(GUI_Object* object) override;
	virtual void remove_item_control(GUI_Object* object);
};

#endif //GUI_ACTIONMANAGER_H
