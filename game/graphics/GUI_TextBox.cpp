#include "GUI_TextBox.h"

GUI_TextBox::GUI_TextBox()
    : GUI_Scrollable_container(0, 0, 0, 0)
{
}

GUI_game_console::GUI_game_console(Game_log& log)
    : m_log(log)
{
    m_styles.push_back(GUI_TextFormat(10, 19, RGBA_t(0.0, 0.8, 0.0, 1.0)));
    m_styles.push_back(GUI_TextFormat(10, 19, RGBA_t(0.3, 0.8, 0.8, 1.0)));
    m_styles.push_back(GUI_TextFormat(10, 19, RGBA_t(0.8, 0.0, 0.0, 1.0)));
    m_log.add_item += std::bind(&GUI_game_console::on_log_add_item, this, std::placeholders::_1);
};

void GUI_game_console::on_log_add_item(game_log_message_t& e)
{
    add_item_control(new GUI_Text(e.value, &m_styles[e.type]));
}