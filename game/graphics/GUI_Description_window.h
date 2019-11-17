#ifndef GUI_DESCRIPTION_WINDOW_H
#define GUI_DESCRIPTION_WINDOW_H

#include "GUI_Container.h"
#include "GUI_TextBox.h"
#include "GUI_Window.h"
#include "Visitors.h"
#include <list>

class Visitor_object_description_getter : public Visitor_simple {
public:
    Tree<std::u16string>* m_value;

    std::list<Tree<std::u16string>*> m_active;

    Visitor_object_description_getter();

    void handle_simple(std::u16string& value);
    void handle_simple(std::u16string const& value);
    void handle_complex(std::u16string& value);
    void handle_complex(std::u16string const& value);

    void visit(Object_state& value) override;
    void visit(ObjectTag::Label& value) override;
    void visit(ObjectTag::Requirements_to_object& value) override;
    void visit(ObjectTag::Mortal& value) override;
    void visit(Action_pick& value) override;

    void visit(GameObject& value) override;
    void visit(Attribute_map& value) override;
    void visit(Interaction_list& value) override;
    void visit(Parameter_list& value) override;
    void visit(Vision_list& value) override;
    void visit(Vision_component& value) override;
    void visit(Parts_list& value) override;
    void visit(ObjectPart& value) override;
    void visit(Tag_list& value) override;
    void visit(Action_list& value) override;
    void visit(Instruction_check_tag& value) override;
    void visit(Instruction_slot_link& value) override;
    void visit(Slot_set_state& value) override;
    void visit(Interaction_copyist& value) override;
    void visit(Interaction_prefix& value) override;
    void visit(Interaction_addon& value) override;
    void visit(Interaction_time& value) override;
    void visit(Interaction_timer& value) override;
    void visit(Effect& value) override;
    void visit(Instruction_slot_parameter& value) override;
};

class GUI_TreeElement : public GUI_Object {
public:
    enum class kind_e {
        node,
        element
    };

    enum class element_position_e {
        begin,
        middle,
        end,
        single
    };

    kind_e m_kind;
    element_position_e m_element_position;

    int level;
    GUI_TextFormat* m_format;
    std::u16string m_text;

    std::list<GUI_Object*>::iterator m_next;
    GUI_TreeElement* m_owner;
    bool m_hide;

    GUI_TreeElement(std::u16string text, GUI_TextFormat* format, int level);
    void render(GraphicalController* graph, int px, int py) override;
};

class GUI_TreeView : public GUI_Scrollable_container {
public:
    GameObject* m_object;
    std::list<std::u16string> m_text;

    GUI_TreeView(int x, int y, int width, int height, GameObject*& object);
    void on_mouse_click(MouseEventArgs const& e) override;
    void update() override;
    void render(GraphicalController* graph, int px, int py) override;

    void update_info();
    void change_node();

private:
    void add_tree(Tree<std::u16string>& value, int level, GUI_TreeElement* owner);
    void bind_tree(Tree<std::u16string>& value, std::list<GUI_Object*>::iterator& pos);
};

class GUI_Description_window : public GUI_Window {
public:
    GameObject* m_object;
    GUI_TreeView* m_textbox;
    GUI_Description_window(int x, int y, int width, int height,const std::u16string& name,gui_style_t& style, GameObject*& object);
    void update_info();
};

#endif GUI_DESCRIPTION_WINDOW_H