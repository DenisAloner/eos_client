#ifndef GUI_ITEM_H
#define GUI_ITEM_H

#include "GUI_Container.h"
#include "GUI_ItemButton.h"
#include "GameObject.h"

class GameTask;
class GameObject;

class GUI_Item : public GUI_Container {
public:
    Event<GUI_Object*> close;

    tag_t m_tag;

    GUI_Item(int x, int y, int width, int height, std::u16string text, tag_t tag);
    ~GUI_Item();

    void on_close_button_click();
    void resize(int width, int height) override;
    //virtual void OnMouseClick(MouseEventArgs const& e);
};

#endif //GUI_ITEM_H
