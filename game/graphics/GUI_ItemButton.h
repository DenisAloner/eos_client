#ifndef GUI_ITEMBUTTON_H
#define GUI_ITEMBUTTON_H

#include "GUI_Object.h"

class GUI_ItemButton : public GUI_Object {
public:
    GUI_ItemButton();
    ~GUI_ItemButton();

    //bool check_region(MouseEventArgs const& e) override;
    void render(GraphicalController* graph, int px, int py) override;
    void on_mouse_move(MouseEventArgs const& e) override;
};

#endif //GUI_ITEMBUTTON_H
