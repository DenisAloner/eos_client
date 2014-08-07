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
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	if (Application::instance().command_select_location(p->m_object, p->m_place))
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
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


bool ActionClass_Move::check(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	//GameObjectProperty* result(nullptr);
	//for (int i = 0; i<p->object->Area.y; i++)
	//{
	//	for (int j = 0; j<p->object->Area.x; j++)
	//	{
	//		if (p->map->Items[p->place->y + i][p->place->x - j] == nullptr){ return false; }
	//		if (p->map->Items[p->place->y + i][p->place->x - j]->FindProperty(property_action_move, p->object) != nullptr)
	//		{
	//			return false;
	//		}
	//	}
	//}
	//return true;
	return Application::instance().command_check_position(p->m_object,p->m_place,p->m_map);
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
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	return Application::instance().command_check_position(p->m_object, p->m_place, p->m_map);
}


void ActionClass_Push::perfom(Parameter* parameter)
{
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	if (check(p))
	{
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
	if (Application::instance().command_select_location(p->m_object, p->m_place))
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
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
	Parameter_GameObject* p = static_cast<Parameter_GameObject*>(parameter);
	std::string s("Повернуться");
	return s;
}


bool ActionClass_Turn::check(Parameter* parameter)
{
	Parameter_GameObject* p = static_cast<Parameter_GameObject*>(parameter);
	return true;
}


void ActionClass_Turn::perfom(Parameter* parameter)
{
	Parameter_GameObject* p = static_cast<Parameter_GameObject*>(parameter);
	if (check(p))
	{
		p->m_object->Turn();
	}
}


void ActionClass_Turn::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Вы разворачиваетесь."));
	Parameter_GameObject* p = new Parameter_GameObject();
	p->m_object = Application::instance().m_GUI->MapViewer->m_player;
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
	p->m_object = Application::instance().m_GUI->MapViewer->m_player;
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	if (Application::instance().command_select_location(p->m_object, p->m_place))
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(" Освещение {" + std::to_string(p->m_place->m_light.R) + "," + std::to_string(p->m_place->m_light.G) + "," + std::to_string(p->m_place->m_light.B) + "}."));
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

