#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

//#include "Definiton.h"
//#include "GUI_Object.h"
#include "GUI_Text.h"

class GUI_button : public GUI_Object {
public:
    GUI_Text* m_text;

    GUI_button(int x, int y, int width, int height, std::u16string text);
    ~GUI_button();

    void render(GraphicalController* graph, int px, int py) override;
};

class GUI_Mainmenu_button : public GUI_button

{
public:
    Parameter* m_parameter;
    GUI_Mainmenu_button(int x, int y, int width, int height, std::u16string text, parameter_type_e kind);

    void on_mouse_click(MouseEventArgs const& e) override;
};

#endif //GUI_BUTTON_H