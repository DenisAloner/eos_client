#include "ApplicationGUI.h"
#include "GUI_TextBox.h"
#include "GUI_button.h"
#include "graphics/GUI_ActionManager.h"

ApplicationGUI::ApplicationGUI(int x, int y, int width, int height)
    : GUI_Layer(x, y, width, height)
{
}

ApplicationGUI::ApplicationGUI(int x, int y, int width, int height, Player* p, GameMap* m, ActionManager* am, Game_log& gl)
    : GUI_Layer(x, y, width, height)
{
    map_viewer = new GUI_MapViewer(&Application::instance());
    map_viewer->m_position.x = 0;
    map_viewer->m_position.y = 0;
    map_viewer->m_size.w = m_size.w;
    map_viewer->m_size.h = m_size.h;

    map_viewer->m_map = m;
    map_viewer->m_player = p;

    /*GUI_ActionPanel* ActionPanel;
	ActionPanel = new GUI_ActionPanel(2, m_size.h - 49, 898, 47);*/
    //m_action_panel = ActionPanel;
    //ActionPanel->bind(p);

    auto menu_layer = new GUI_Layer(0, 0, m_size.w, m_size.h);

    const auto b = 2;
    const auto w1 = m_size.w / 5;
    const auto w2 = w1 * 2;
    const auto w3 = w2 + w2;
    const auto h1 = m_size.h / 4;
    const auto h2 = 47;

    auto console = new GUI_game_console(gl);
    console->m_position.x = b;
    console->m_position.y = m_size.h - h1;
    console->resize(w2 - b, h1 - b);

    const auto am_text_box = new GUI_ActionManager(w2 + b, m_size.h - h1, w2 - b, h1 - b, am);

    auto button = new GUI_button(w3 + b, m_size.h - (h2 + b), m_size.w - w3 - b * 2, h2, u"Ход");
    button->mouse_click += std::bind(&Application::on_turn, &Application::instance());

    m_action_bar = new GUI_Action_bar(w3 + b, y + b, m_size.w - w3 - b * 2, m_size.h - (h2 + b * 3));
    m_action_bar->bind(p);

    menu_layer->add(am_text_box);
    //MenuLayer->add(ActionPanel);
    menu_layer->add(console);
    menu_layer->add(button);
    menu_layer->add(m_action_bar);

    GUI_Layer::add(Application::instance().m_window_manager);
    GUI_Layer::add(menu_layer);
    GUI_Layer::add(map_viewer);
    map_viewer->m_gui = menu_layer;
    m_main_layer = menu_layer;
}

ApplicationGUI::~ApplicationGUI()
{
}
