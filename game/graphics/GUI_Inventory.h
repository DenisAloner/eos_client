#ifndef GUI_INVENTORY_H
#define GUI_INVENTORY_H

#include "GameObject.h"
#include "Visitors.h"
#include <list>
#include "gui_scrollable_container.h"

class Property_Container;
class Parts_list;
class Inventory_cell;
class GameObject;
class Object_part;
class Attribute_map;

class GUI_Body;

class GUI_Part_slot : public GUI_Object {
public:
    Object_part* m_item;
    GUI_Body* m_owner;
    std::u16string m_name;

    GUI_Part_slot(int width, int height, Object_part* item, GUI_Body* owner);

    void render(GraphicalController* graph, int px, int py) override;
    void on_mouse_move(MouseEventArgs const& e) override;
    void on_mouse_down(MouseEventArgs const& e) override;
};

class GUI_Body : public GUI_Scrollable_container {
public:
    bool m_already_active;
    GUI_Body(Attribute_map* feature);

    std::size_t m_max_item_name;

    void on_mouse_down(MouseEventArgs const& e) override;
    void set_focus(bool state) override;
    void update(Attribute_map* feature);
};

class Visitor_container_hierarchy_getter : public Visitor_simple {
public:
    std::list<std::u16string> m_active;
    std::list<GUI_Part_slot*> m_result;

    GUI_Body* m_owner;

    Visitor_container_hierarchy_getter(GUI_Body* owner);

    void visit(Object_part& value) override;
};

#endif //GUI_INVENTORY_H
