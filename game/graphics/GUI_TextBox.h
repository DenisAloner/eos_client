#ifndef GUI_TEXTBOX_H
#define GUI_TEXTBOX_H

#include "Definiton.h"
#include "gui_scrollable_container.h"
#include "game/Game_log.h"

class GUI_TextBox : public GUI_Scrollable_container {
public:
    GUI_TextBox();
};

class GUI_game_console : public GUI_TextBox {
public:
    std::vector<GUI_TextFormat> m_styles;

    Game_log& m_log;
    explicit GUI_game_console(Game_log& log);

    virtual void on_log_add_item(game_log_message_t& e);
};

#endif //GUI_TEXTBOX_H
