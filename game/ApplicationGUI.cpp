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

	GUI_ActionPanel* ActionPanel;
	ActionPanel = new GUI_ActionPanel(2, m_size.h - 49, 898, 47);
	//m_action_panel = ActionPanel;
	ActionPanel->bind(p);

	GUI_ActionManager* AMTextBox;
	AMTextBox = new GUI_ActionManager(am);
	AMTextBox->m_position.x = 650;
	AMTextBox->m_position.y = ActionPanel->m_position.y - 265;
	AMTextBox->resize(372, 263);

	GUI_Layer* MenuLayer;
	MenuLayer = new GUI_Layer(0, 0, m_size.w, m_size.h);

	GUI_game_console* console = new GUI_game_console(gl);
	console->m_position.x = 2;
	console->m_position.y = ActionPanel->m_position.y - 265;
	console->resize(646, 263);

	//m_GUI->DescriptionBox = console;

	GUI_button* button = new GUI_button(902, m_size.h - 49, 120, 47, "Õîä");
	button->mouse_click += std::bind(&Application::on_turn, &Application::instance());

	MenuLayer->add(AMTextBox);
	MenuLayer->add(ActionPanel);
	MenuLayer->add(console);
	MenuLayer->add(button);

	add(Application::instance().m_window_manager);
	add(MenuLayer);
	add(MapViewer);
	MapViewer->m_GUI = MenuLayer;
}


ApplicationGUI::~ApplicationGUI()
{
}
