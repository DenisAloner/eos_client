#ifndef GUI_INVENTORY_H
#define GUI_INVENTORY_H

#include "GameObject.h"
#include "Visitors.h"
#include <list>
#include "gui_scrollable_container.h"
#include "object_part.h"

class Property_Container;
class Parts_list;
class InventoryCell;
//class GameObject;
//class ObjectPart;
class AttributeMap;

class GUI_Body;

class GUI_Part_slot : public GUI_Object {
public:
    ObjectPart* m_item;
    GUI_Body* m_owner;
    std::u16string m_name;

    GUI_Part_slot(int width, int height, ObjectPart* item, GUI_Body* owner);

    void render(GraphicalController* graph, int px, int py) override;
    void on_mouse_move(MouseEventArgs const& e) override;
    void on_mouse_down(MouseEventArgs const& e) override;
};

class GUI_Body : public GUI_Scrollable_container {
public:
    bool m_already_active;
    GUI_Body(AttributeMap* feature);

    std::size_t m_max_item_name;

    void on_mouse_down(MouseEventArgs const& e) override;
    void set_focus(bool state) override;
    void update(AttributeMap* feature);
};

class Visitor_container_hierarchy_getter : public Visitor_simple {
public:
    std::list<std::u16string> m_active;
    std::list<GUI_Part_slot*> m_result;

    GUI_Body* m_owner;

    Visitor_container_hierarchy_getter(GUI_Body* owner);

    void visit(ObjectPart& value) override;
};

#endif //GUI_INVENTORY_H
