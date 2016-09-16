#include "ApplicationGUI.h"


ApplicationGUI::ApplicationGUI(int x, int y, int width, int height) :GUI_Layer(x, y, width, height) {}

ApplicationGUI::ApplicationGUI(int x, int y, int width, int height, Player* p, GameMap* m, ActionManager* am,Game_log& gl) :GUI_Layer(x, y, width, height)
{
	MapViewer = new GUI_MapViewer(&Application::instance());
	MapViewer->m_position.x = 0;
	MapViewer->m_position.y = 0;
	MapViewer->m_size.w = m_size.w;
	MapViewer->m_size.h = m_size.h;

	MapViewer->m_map = m;
	MapViewer->m_player = p;

	/*GUI_ActionPanel* ActionPanel;
	ActionPanel = new GUI_ActionPanel(2, m_size.h - 49, 898, 47);*/
	//m_action_panel = ActionPanel;
	//ActionPanel->bind(p);

	GUI_Layer* MenuLayer;
	MenuLayer = new GUI_Layer(0, 0, m_size.w, m_size.h);

	int b = 2;
	int w1 = m_size.w / 5;
	int w2 = w1 * 2;
	int w3 = w2 + w2;
	int h1 = m_size.h / 4;
	int h2 = 47;

	GUI_game_console* console = new GUI_game_console(gl);
	console->m_position.x = b;
	console->m_position.y = m_size.h - h1;
	console->resize(w2 - b, h1 - b);

	GUI_ActionManager* AMTextBox;
	AMTextBox = new GUI_ActionManager(w2 + b, m_size.h - h1, w2 - b, h1 - b, am);

	GUI_button* button = new GUI_button(w3 + b, m_size.h - (h2 + b*2), m_size.w - w3 - b * 2, h2, "Õîä");
	button->mouse_click += std::bind(&Application::on_turn, &Application::instance());

	m_action_bar = new GUI_Action_bar(w3 + b, y + b, m_size.w - w3 - b * 2, m_size.h - (h2 + b * 4));
	m_action_bar->bind(p);

	MenuLayer->add(AMTextBox);
	//MenuLayer->add(ActionPanel);
	MenuLayer->add(console);
	MenuLayer->add(button);
	MenuLayer->add(m_action_bar);


	add(Application::instance().m_window_manager);
	add(MenuLayer);
	add(MapViewer);
	MapViewer->m_GUI = MenuLayer;
}


ApplicationGUI::~ApplicationGUI()
{
}
