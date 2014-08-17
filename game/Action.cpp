#include "game/Action.h"
#include "game/Application.h"
#include "game/ApplicationGUI.h"
#include "game/ActionManager.h"
#include "game/GameObject.h"
#include "game/Parameter.h"
#include "game/graphics/GUI_MapViewer.h"
#include "game/graphics/GUI_TextBox.h"


Action::Action(void)
{
	m_kind = action_e::move;
	m_name = "Нет";
	m_error = "";
}


Action::~Action(void)
{
}


bool Action::check(Parameter* parameter)
{
	return true;
}


void Action::perfom(Parameter* parameter)
{
}


void Action::interaction_handler()
{
	/*Game->PlaySound1();*/
	if (Application::instance().m_message_queue.m_busy)
	{
		if (Application::instance().m_message_queue.m_reader)
		{
			Parameter* p = new Parameter(ParameterKind_Cancel);
			Application::instance().m_message_queue.push(p);
		}
		while (Application::instance().m_message_queue.m_busy){}
	}
}


ActionClass_Move::ActionClass_Move()
{
	m_kind = action_e::move;
	m_name = "Идти";
}


ActionClass_Move::~ActionClass_Move()
{
}


void ActionClass_Move::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter_Position* p = new Parameter_Position();
	p->m_object = Application::instance().m_GUI->MapViewer->m_player;
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_preselect_area(Application::instance().m_GUI->MapViewer,p->m_object->m_cell));
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	if (Application::instance().command_select_location(p->m_object, p->m_place))
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
		return;
	}
	Application::instance().m_GUI->MapViewer->m_hints.pop_front();
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}


bool ActionClass_Move::check(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	return Application::instance().command_check_position(p->m_object,p->m_place,p->m_map);
}

bool action_move_step::check(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	if (abs(p->m_object->m_cell->x - p->m_place->x) < 2 && abs(p->m_object->m_cell->y - p->m_place->y)< 2)
	{
		return Application::instance().command_check_position(p->m_object, p->m_place, p->m_map);
	}
	else return false;
}

void ActionClass_Move::perfom(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	if (check(p))
	{
		p->m_map->move_object(p->m_object, p->m_place);
	}
}


std::string ActionClass_Move::get_description(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	std::string s("Идти в X:");
	s += std::to_string(p->m_place->x);
	s += ",Y:";
	s += std::to_string(p->m_place->y);
	return s;
}


ActionClass_Push::ActionClass_Push(void)
{
	m_kind = action_e::push;
	m_name = "Толкать";
}


ActionClass_Push::~ActionClass_Push(void)
{
}


std::string ActionClass_Push::get_description(Parameter* parameter)
{
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	std::string s("Переместить ");
	s += p->m_object->m_name;
	s += " в X:"+std::to_string(p->m_place->x);
	s += ",Y:";
	s += std::to_string(p->m_place->y);
	return s;
}


bool ActionClass_Push::check(Parameter* parameter)
{
	m_error = "";
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	MapCell* cell;
	GameObjectParameter* prop = static_cast<GameObjectParameter*>(p->m_unit->find_property(property_e::strength));
	if (prop != nullptr)
	{
		if (prop->m_value * 10 < p->m_object->m_weight)
		{
			m_error = "Вы слишком слабы: нужно " + std::to_string(p->m_object->m_weight / 10) + " силы.";
			return false;
		}
	}
	else { return false; }
	
	if (abs(p->m_unit->m_cell->x - p->m_object->m_cell->x)<2 || abs(p->m_unit->m_cell->x - p->m_unit->m_size.x + 1 - p->m_object->m_cell->x)<2 || abs(p->m_unit->m_cell->x - p->m_unit->m_size.x + 1 - (p->m_object->m_cell->x - p->m_object->m_size.x + 1))<2 || abs(p->m_unit->m_cell->x- (p->m_object->m_cell->x - p->m_object->m_size.x + 1))<2)
	{
		if (abs(p->m_unit->m_cell->y - p->m_object->m_cell->y) < 2 || abs(p->m_unit->m_cell->y + p->m_unit->m_size.y - 1 - p->m_object->m_cell->y) < 2 || abs(p->m_unit->m_cell->y+ p->m_unit->m_size.y - 1 - (p->m_object->m_cell->y + p->m_object->m_size.y - 1)) < 2 || abs(p->m_unit->m_cell->y - (p->m_object->m_cell->y + p->m_object->m_size.y - 1)) < 2)
		{
			for (int i = 0; i < p->m_object->m_size.y; i++)
			{
				for (int j = 0; j < p->m_object->m_size.x; j++)
				{
					cell = p->m_map->m_items[p->m_place->y + i][p->m_place->x - j];
					if (cell == nullptr){ return false; }
					for (std::list<GameObject*>::iterator item = cell->m_items.begin(); item != cell->m_items.end(); item++)
					{
						if (p->m_object != (*item) && p->m_unit != (*item))
						{
							if ((*item)->find_property(property_e::permit_move) == nullptr)
							{
								return false;
							}
						}
					}
				}
			}
			for (int i = 0; i < p->m_unit->m_size.y; i++)
			{
				for (int j = 0; j < p->m_unit->m_size.x; j++)
				{
					cell == p->m_map->m_items[p->m_unit->m_cell->y + (p->m_place->y - p->m_object->m_cell->y) + i][p->m_unit->m_cell->x + (p->m_place->x - p->m_object->m_cell->x) - j];
					if (cell == nullptr){ return false; }
					for (std::list<GameObject*>::iterator item = cell->m_items.begin(); item != cell->m_items.end(); item++)
					{
						if (p->m_object != (*item) && p->m_unit != (*item))
						{
							if ((*item)->find_property(property_e::permit_move) == nullptr)
							{
								return false;
							}
						}
					}
				}
			}
			return true;
		}
	}
	return false;
}


void ActionClass_Push::perfom(Parameter* parameter)
{
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	if (check(p))
	{
		p->m_map->move_object(p->m_unit, p->m_map->m_items[p->m_unit->m_cell->y + (p->m_place->y - p->m_object->m_cell->y)][p->m_unit->m_cell->x + (p->m_place->x - p->m_object->m_cell->x)]);
		p->m_map->move_object(p->m_object, p->m_place);
	}
}


void ActionClass_Push::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter_MoveObjectByUnit* p = new Parameter_MoveObjectByUnit();
	p->m_unit = Application::instance().m_GUI->MapViewer->m_player;
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_preselect_area(Application::instance().m_GUI->MapViewer, p->m_unit->m_cell));
	if (Application::instance().command_select_object(p->m_object))
	{
		std::string a = "Выбран "; 
		a.append(p->m_object->m_name);
		a = a + ".";
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(a));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
		return;
	}
	Application::instance().m_GUI->MapViewer->m_hints.pop_front();
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_preselect_area(Application::instance().m_GUI->MapViewer, p->m_object->m_cell));
	if (Application::instance().command_select_location(p->m_object, p->m_place))
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
		return;
	}
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
	Application::instance().m_GUI->MapViewer->m_hints.pop_front();
}


ActionClass_Turn::ActionClass_Turn(void)
{
	m_kind = action_e::turn;
	m_name = "Повернуться";
}


ActionClass_Turn::~ActionClass_Turn(void)
{
}


std::string ActionClass_Turn::get_description(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	std::string s("Повернуть ");
	s += p->m_object->m_name + ".";
	return s;
}


bool ActionClass_Turn::check(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	return true;
}


void ActionClass_Turn::perfom(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	if (check(p))
	{
		p->m_map->turn_object(p->m_object);
	}
}


void ActionClass_Turn::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter_Position* p = new Parameter_Position();
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	if (Application::instance().command_select_object(p->m_object))
	{
		std::string a = "Выбран ";
		a.append(p->m_object->m_name);
		a = a + ".";
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(a));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}


Action_OpenInventory::Action_OpenInventory()
{
	m_kind = action_e::open_inventory;
}


Action_OpenInventory::~Action_OpenInventory()
{
}


void Action_OpenInventory::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter_GameObject* p = new Parameter_GameObject();
	if (Application::instance().command_select_object(p->m_object))
	{
		std::string a = "Выбран ";
		a.append(p->m_object->m_name);
		a = a + ".";
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(a));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	Application::instance().command_open_inventory(p->m_object);
	Application::instance().m_message_queue.m_busy = false;
}


bool Action_OpenInventory::check(Parameter* parameter)
{
	return true;
}


void Action_OpenInventory::perfom(Parameter* parameter)
{
}


std::string Action_OpenInventory::get_description(Parameter* parameter)
{
	return "";
}

Action_CellInfo::Action_CellInfo()
{
	m_kind = action_e::cell_info;
}


Action_CellInfo::~Action_CellInfo()
{
}

void Action_CellInfo::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter_Position* p = new Parameter_Position();
	p->m_object = nullptr;
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	if (Application::instance().command_select_location(p->m_object, p->m_place))
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}:"));
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(" - освещение {" + std::to_string(p->m_place->m_light.R) + "," + std::to_string(p->m_place->m_light.G) + "," + std::to_string(p->m_place->m_light.B) + "}."));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Не выбрана клетка карты."));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	Application::instance().m_message_queue.m_busy = false;
}


bool Action_CellInfo::check(Parameter* parameter)
{
	return true;
}

void Action_CellInfo::perfom(Parameter* parameter)
{
}

std::string Action_CellInfo::get_description(Parameter* parameter)
{
	return "";
}

