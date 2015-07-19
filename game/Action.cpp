#include "game/Action.h"
#include "game/Application.h"
#include "game/ApplicationGUI.h"
#include "game/ActionManager.h"
#include "game/GameObject.h"
#include "game/Parameter.h"
#include "game/graphics/GUI_MapViewer.h"
#include "game/graphics/GUI_TextBox.h"
#include "log.h"

Action::Action(void)
{
	m_interaction_message_type = interaction_message_type_e::action;
	m_kind = action_e::move;
	m_name = "Нет";
	m_error = "";
}

Action::~Action(void)
{
}

Object_interaction* Action::clone()
{
	return nullptr;
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
	m_icon = Application::instance().m_graph->m_actions[0];
	m_name = "Идти";
}


ActionClass_Move::~ActionClass_Move()
{
}


void ActionClass_Move::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	Parameter_Position* p = new Parameter_Position();
	p->m_object = Application::instance().m_GUI->MapViewer->m_player->m_object;
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_area(Application::instance().m_GUI->MapViewer, p->m_object, false));
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	result = Application::instance().command_select_location(p->m_object);
	if (result)
	{
		p->m_place = static_cast<MapCell*>(static_cast<P_object_owner*>(result)->m_cell);
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
	Application::instance().m_action_manager->add(p->m_object, new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}


bool ActionClass_Move::check(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	return Application::instance().command_check_position(p->m_object, p->m_place, p->m_map);
}

bool action_move_step::check(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	if (abs(p->m_object->cell()->x - p->m_place->x) < 2 && abs(p->m_object->cell()->y - p->m_place->y)< 2)
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
		int dx = p->m_place->x - p->m_object->cell()->x;
		int dy = p->m_place->y - p->m_object->cell()->y;
		switch (dy)
		{
		case 1:
		{
			switch (dx)
			{
			case -1: {p->m_object->set_direction(object_direction_e::topleft); break; }
			case 0: {p->m_object->set_direction(object_direction_e::top); break; }
			case 1: {p->m_object->set_direction(object_direction_e::topright); break; }
			}
			break;
		}
		case 0:
		{
			switch (dx)
			{
			case -1: {p->m_object->set_direction(object_direction_e::left); break; }
			case 1: {p->m_object->set_direction(object_direction_e::right); break; }
			}
			break;
		}
		case -1:
		{
			switch (dx)
			{
			case -1: {p->m_object->set_direction(object_direction_e::downleft); break; }
			case 0: {p->m_object->set_direction(object_direction_e::down); break; }
			case 1: {p->m_object->set_direction(object_direction_e::downright); break; }
			}
			break;
		}
		}
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
	m_icon = Application::instance().m_graph->m_actions[1];
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
	s += " в X:" + std::to_string(p->m_place->x);
	s += ",Y:";
	s += std::to_string(p->m_place->y);
	return s;
}


bool ActionClass_Push::check(Parameter* parameter)
{
	m_error = "";
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	MapCell* cell;
	auto* prop = p->m_unit->get_parameter(interaction_e::strength);
	if (prop != nullptr)
	{
		if (prop->m_value * 10 < p->m_object->m_weight)
		{
			m_error = "Вы слишком слабы: нужно " + std::to_string(p->m_object->m_weight / 10) + " силы.";
			return false;
		}
	}
	else { return false; }

	if (abs(p->m_unit->cell()->x - p->m_object->cell()->x)<2 || abs(p->m_unit->cell()->x - p->m_unit->m_active_state->m_size.x + 1 - p->m_object->cell()->x)<2 || abs(p->m_unit->cell()->x - p->m_unit->m_active_state->m_size.x + 1 - (p->m_object->cell()->x - p->m_object->m_active_state->m_size.x + 1))<2 || abs(p->m_unit->cell()->x - (p->m_object->cell()->x - p->m_object->m_active_state->m_size.x + 1))<2)
	{
		if (abs(p->m_unit->cell()->y - p->m_object->cell()->y) < 2 || abs(p->m_unit->cell()->y + p->m_unit->m_active_state->m_size.y - 1 - p->m_object->cell()->y) < 2 || abs(p->m_unit->cell()->y + p->m_unit->m_active_state->m_size.y - 1 - (p->m_object->cell()->y + p->m_object->m_active_state->m_size.y - 1)) < 2 || abs(p->m_unit->cell()->y - (p->m_object->cell()->y + p->m_object->m_active_state->m_size.y - 1)) < 2)
		{
			for (int i = 0; i < p->m_object->m_active_state->m_size.y; i++)
			{
				for (int j = 0; j < p->m_object->m_active_state->m_size.x; j++)
				{
					cell = p->m_map->m_items[p->m_place->y + i][p->m_place->x - j];
					if (cell == nullptr){ return false; }
					for (std::list<GameObject*>::iterator item = cell->m_items.begin(); item != cell->m_items.end(); item++)
					{
						if (p->m_object != (*item) && p->m_unit != (*item))
						{
							if (!(*item)->get_stat(object_tag_e::pass_able))
							{
								return false;
							}
						}
					}
				}
			}
			for (int i = 0; i < p->m_unit->m_active_state->m_size.y; i++)
			{
				for (int j = 0; j < p->m_unit->m_active_state->m_size.x; j++)
				{
					cell = p->m_map->m_items[p->m_unit->cell()->y + (p->m_place->y - p->m_object->cell()->y) + i][p->m_unit->cell()->x + (p->m_place->x - p->m_object->cell()->x) - j];
					if (cell == nullptr){ return false; }
					for (std::list<GameObject*>::iterator item = cell->m_items.begin(); item != cell->m_items.end(); item++)
					{
						if (p->m_object != (*item) && p->m_unit != (*item))
						{
							if (!(*item)->get_stat(object_tag_e::pass_able))
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
		p->m_map->move_object(p->m_unit, p->m_map->m_items[p->m_unit->cell()->y + (p->m_place->y - p->m_object->cell()->y)][p->m_unit->cell()->x + (p->m_place->x - p->m_object->cell()->x)]);
		p->m_map->move_object(p->m_object, p->m_place);
	}
}


void ActionClass_Push::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	Parameter_MoveObjectByUnit* p = new Parameter_MoveObjectByUnit();
	p->m_unit = Application::instance().m_GUI->MapViewer->m_player->m_object;
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_area(Application::instance().m_GUI->MapViewer, p->m_unit, true));
	result = Application::instance().command_select_object_on_map();
	if (result)
	{
		p->m_object = static_cast<P_object*>(result)->m_object;
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
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_area(Application::instance().m_GUI->MapViewer, p->m_object, false));
	result = Application::instance().command_select_location(p->m_object);
	if (result)
	{
		p->m_place = static_cast<MapCell*>(static_cast<P_object_owner*>(result)->m_cell);
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
		return;
	}
	Application::instance().m_action_manager->add(p->m_unit, new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
	Application::instance().m_GUI->MapViewer->m_hints.pop_front();
}


ActionClass_Turn::ActionClass_Turn(void)
{
	m_kind = action_e::turn;
	m_icon = Application::instance().m_graph->m_actions[2];
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
	Parameter* result;
	Parameter_Position* p = new Parameter_Position();
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	result = Application::instance().command_select_object_on_map();
	if (result)
	{
		p->m_object = static_cast<P_object*>(result)->m_object;
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
	Application::instance().m_action_manager->add(p->m_object, new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}


Action_OpenInventory::Action_OpenInventory()
{
	m_kind = action_e::open_inventory;
	m_icon = Application::instance().m_graph->m_actions[3];
}


Action_OpenInventory::~Action_OpenInventory()
{
}


void Action_OpenInventory::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	Parameter_GameObject* p = new Parameter_GameObject();
	result = Application::instance().command_select_object_on_map();
	if (result)
	{
		p->m_object = static_cast<P_object*>(result)->m_object;
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
	m_icon = Application::instance().m_graph->m_actions[4];
}

void Action_CellInfo::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	Parameter_Position* p = new Parameter_Position();
	p->m_object = nullptr;
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	result = Application::instance().command_select_location(p->m_object);
	if (result)
	{
		p->m_place = static_cast<MapCell*>(static_cast<P_object_owner*>(result)->m_cell);
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}:"));
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(" - освещение {" + std::to_string(p->m_place->m_light.R) + "," + std::to_string(p->m_place->m_light.G) + "," + std::to_string(p->m_place->m_light.B) + "}."));
		MapCell* c = Application::instance().m_GUI->MapViewer->m_player->m_object->cell();
		int f = Application::instance().m_GUI->MapViewer->m_player->m_fov->m_radius;
		int xf = p->m_place->x-c->x;
		int yf = p->m_place->y - c->y;
		if ((xf >= -f) && (xf <= f) && (yf >= -f) && (yf <=f))
		{
			Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(std::to_string(Application::instance().m_GUI->MapViewer->m_player->m_fov->m_map[Application::instance().m_GUI->MapViewer->m_player->m_fov->m_middle + yf][Application::instance().m_GUI->MapViewer->m_player->m_fov->m_middle + xf].visible)));
		}
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

action_set_motion_path::action_set_motion_path()
{
	m_kind = action_e::set_motion_path;
	m_icon = Application::instance().m_graph->m_actions[5];
}

void action_set_motion_path::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	Parameter_Position* p = new Parameter_Position();
	p->m_object = Application::instance().m_GUI->MapViewer->m_player->m_object;
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_line(Application::instance().m_GUI->MapViewer, p->m_object->cell()));
	result = Application::instance().command_select_location(p->m_object);
	if (result)
	{
		p->m_place = static_cast<MapCell*>(static_cast<P_object_owner*>(result)->m_cell);
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Не выбрана клетка карты."));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
		return;
	}
	p->m_map->bresenham_line(p->m_object->cell(), p->m_place, [p](MapCell* a) { Application::instance().m_action_manager->add(p->m_object, new GameTask(Application::instance().m_actions[0], new Parameter_Position(p->m_object, a, p->m_map))); });
	Application::instance().m_message_queue.m_busy = false;
	Application::instance().m_GUI->MapViewer->m_hints.pop_front();
}

void action_set_motion_path::perfom(Parameter* parameter)
{
}

std::string action_set_motion_path::get_description(Parameter* parameter)
{
	return "";
}

Action_pick::Action_pick()
{
	m_kind = action_e::push;
	m_icon = Application::instance().m_graph->m_actions[6];
}

void Action_pick::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	Parameter_destination* p = new Parameter_destination();
	p->m_unit = Application::instance().m_GUI->MapViewer->m_player->m_object;
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_area(Application::instance().m_GUI->MapViewer, p->m_unit, true));
	P_object* object = Application::instance().command_select_transfer_source(p);
	if (object)
	{
		p->m_object = object->m_object;
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
	Application::instance().m_clipboard.m_item = p->m_object;
	result = Application::instance().command_select_transfer(p);
	if (result)
	{
		switch (result->m_kind)
		{
		case ParameterKind::parameter_kind_cell:
		{
			p->m_owner = static_cast<P_object_owner*>(result)->m_cell;
			break;
		}
		case ParameterKind::parameter_kind_inventory_cell:
		{
			p->m_owner = static_cast<P_object_owner*>(result)->m_cell;
			break;
		}
		case ParameterKind::parameter_kind_body_part:
		{
			p->m_owner = static_cast<P_object_owner*>(result)->m_cell;
			break;
		}
		}
		//Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_clipboard.m_item = nullptr;
		return;
	}
	Application::instance().m_action_manager->add(p->m_unit, new GameTask(this, p));
	Application::instance().m_clipboard.m_item = nullptr;
	Application::instance().m_message_queue.m_busy = false;
}

void Action_pick::perfom(Parameter* parameter)
{
	Parameter_destination* p = static_cast<Parameter_destination*>(parameter);
	if (check(p))
	{
		switch (p->m_object->m_owner->m_kind)
		{
		case entity_e::cell:
		{
			//static_cast<MapCell*>(p->m_object->m_owner)->m_items.remove(p->m_object);
			static_cast<MapCell*>(p->m_object->m_owner)->m_map->remove_object(p->m_object);
			break;
		}
		case entity_e::inventory_cell:
		{
			static_cast<Inventory_cell*>(p->m_object->m_owner)->m_item = nullptr;
			break;
		}
		case entity_e::body_part:
		{
			Application::instance().command_unequip(p->m_unit, static_cast<Object_part*>(p->m_object->m_owner), p->m_object);
			break;
		}
		}
		switch (p->m_owner->m_kind)
		{
		case entity_e::cell:
		{
			//static_cast<MapCell*>(p->m_owner)->m_items.push_back(p->m_object);
			static_cast<MapCell*>(p->m_owner)->m_map->add_object(p->m_object, static_cast<MapCell*>(p->m_owner));
			p->m_object->m_owner = p->m_owner;
			break;
		}
		case entity_e::inventory_cell:
		{
			static_cast<Inventory_cell*>(p->m_owner)->m_item = p->m_object;
			p->m_object->m_owner = p->m_owner;
			break;
		}
		case entity_e::body_part:
		{
			Application::instance().command_equip(p->m_unit, static_cast<Object_part*>(p->m_owner), p->m_object);
			break;
		}
		}
	}
}

std::string Action_pick::get_description(Parameter* parameter)
{
	Parameter_destination* p = static_cast<Parameter_destination*>(parameter);
	std::string s("Поднять ");
	s += p->m_object->m_name + ".";
	return s;
}

void Action_pick::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<взять>");
}

Action_open::Action_open()
{
	m_kind = action_e::open;
	m_icon = Application::instance().m_graph->m_actions[7];
}

void Action_open::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	P_object* p = new P_object();
	result = Application::instance().command_select_object_on_map();
	if (result)
	{
		p->m_object = static_cast<P_object*>(result)->m_object;
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
	Application::instance().m_action_manager->add(Application::instance().m_GUI->MapViewer->m_player->m_object, new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}

void Action_open::perfom(Parameter* parameter)
{
	P_object* p = static_cast<P_object*>(parameter);
	if (check(p))
	{
		switch (p->m_object->m_active_state->m_state)
		{
		case object_state_e::on:
		{
			Interaction_prefix* ip = new Interaction_prefix();
			ip->m_subtype = effect_prefix_e::state_change;
			Slot_set_state* s = new Slot_set_state();
			s->m_value = object_state_e::off;
			ip->m_value = s;
			ip->apply_effect(p->m_object, nullptr);
			break;
		}
		case object_state_e::off:
		{
			Interaction_prefix* ip = new Interaction_prefix();
			ip->m_subtype = effect_prefix_e::state_change;
			Slot_set_state* s = new Slot_set_state();
			s->m_value = object_state_e::on;
			ip->m_value = s;
			ip->apply_effect(p->m_object, nullptr);
			break;
		}
		}
	}
}

std::string Action_open::get_description(Parameter* parameter)
{
	P_object* p = static_cast<P_object*>(parameter);
	std::string s("Открыть ");
	s += p->m_object->m_name + ".";
	return s;
}

Action_hit::Action_hit()
{
	m_kind = action_e::hit;
	m_icon = Application::instance().m_graph->m_actions[8];
}

void Action_hit::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	P_unit_interaction* p = new P_unit_interaction();
	p->m_unit = Application::instance().m_GUI->MapViewer->m_player->m_object;
	result = Application::instance().command_select_body_part();
	if (result)
	{
		p->m_unit_body_part = static_cast<Object_part*>(static_cast<P_object_owner*>(result)->m_cell);
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_clipboard.m_item = nullptr;
		return;
	}
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_area(Application::instance().m_GUI->MapViewer, p->m_unit, true));
	result = Application::instance().command_select_object_on_map();
	if (result)
	{
		p->m_object = static_cast<P_object*>(result)->m_object;
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
	Application::instance().m_action_manager->add(p->m_unit, new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}

bool Action_hit::check(Parameter* parameter)
{
	m_error = "";
	P_unit_interaction* p = static_cast<P_unit_interaction*>(parameter);
	MapCell* cell;
	if (!Game_algorithm::check_distance(p->m_unit->cell(), p->m_unit->m_active_state->m_size, p->m_object->cell(), p->m_object->m_active_state->m_size))
	{
		m_error = "Вы слишком далеко от " + p->m_object->m_name;
		return false;
	};
	return true;
}

void Action_hit::perfom(Parameter* parameter)
{
	P_unit_interaction* p = static_cast<P_unit_interaction*>(parameter);
	if (check(p))
	{
		p->m_unit->set_direction(Game_algorithm::turn_to_object(p->m_unit,p->m_object));
		auto reaction = p->m_unit->get_effect(interaction_e::total_damage);
		if (reaction)
		{
			Object_interaction* msg = reaction->clone();
			msg->apply_effect(p->m_object, nullptr);
		}
		if (p->m_unit_body_part)
		{
			reaction = p->m_unit_body_part->m_item->get_effect(interaction_e::damage);
			if (reaction)
			{
				Object_interaction* msg = reaction->clone();
				msg->apply_effect(p->m_object, nullptr);
			}
		}
		p->m_object->update_interaction();
		p->m_object->event_update(VoidEventArgs());
	}
}

action_hit_melee::action_hit_melee()
{
	m_kind = action_e::hit_melee;
	m_icon = Application::instance().m_graph->m_actions[8];
}

void action_hit_melee::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	P_interaction_cell* p = new P_interaction_cell();
	p->m_unit = Application::instance().m_GUI->MapViewer->m_player->m_object;
	result = Application::instance().command_select_body_part();
	if (result)
	{
		p->m_unit_body_part = static_cast<Object_part*>(static_cast<P_object_owner*>(result)->m_cell);
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_clipboard.m_item = nullptr;
		return;
	}
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_area(Application::instance().m_GUI->MapViewer, p->m_unit, true));
	result = Application::instance().command_select_object_on_map();
	if (result)
	{
		p->m_object = static_cast<P_object*>(result)->m_object;
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
	Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_object_area(Application::instance().m_GUI->MapViewer, p->m_object));
	result = Application::instance().command_select_location(p->m_object);
	if (result)
	{
		p->m_cell = static_cast<MapCell*>(static_cast<P_object_owner*>(result)->m_cell);
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_cell->x) + "," + std::to_string(p->m_cell->y) + "}:"));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
		return;
	}
	Application::instance().m_GUI->MapViewer->m_hints.pop_front();
	Application::instance().m_action_manager->add(p->m_unit, new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}

void action_hit_melee::perfom(Parameter* parameter)
{
	P_interaction_cell* p = static_cast<P_interaction_cell*>(parameter);
	if (check(p))
	{
		auto reaction = p->m_unit->get_effect(interaction_e::total_damage);
		if (reaction)
		{
			Object_interaction* msg = reaction->clone();
			msg->apply_effect(p->m_object, nullptr);
		}
		if (p->m_unit_body_part)
		{
			Parameter_list* str = p->m_unit->get_parameter(interaction_e::strength);
			Parameter_list* ms = p->m_unit->get_parameter(interaction_e::skill_sword);
			Parameter_list* dws = p->m_unit_body_part->m_item->get_parameter(interaction_e::demand_weapon_skill);
			Parameter_list* wd = p->m_unit_body_part->m_item->get_parameter(interaction_e::weapon_damage);
			Parameter_list* sb = p->m_unit_body_part->m_item->get_parameter(interaction_e::strength_bonus);
			srand(time(NULL));
			int accuracy = (ms->m_value - dws->m_value);
			int light = (p->m_cell->m_light.R > p->m_cell->m_light.G ? p->m_cell->m_light.R : p->m_cell->m_light.G);
			light = (light > p->m_cell->m_light.B ? light : p->m_cell->m_light.B);
			if (light > 100){ light = 100; };
			if (accuracy > 0)
			{
				accuracy = (ms->m_value + rand() % accuracy)*(light + rand() % (100 - light + 1)*0.5);
				Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(std::to_string(accuracy*0.0001)));
			}
			else
			{
				accuracy = (ms->m_value - rand() % accuracy)*(light + rand() % (100 - light + 1)*0.5);
			}
			if (accuracy > 0)
			{
				
				Effect* item = new Effect();
				item->m_interaction_message_type = interaction_message_type_e::single;
				item->m_subtype = effect_e::value;
				item->m_value = -accuracy*0.0001*sb->m_value*0.01*wd->m_value*str->m_value;
				Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(std::to_string(item->m_value)));
				Interaction_copyist* item1 = new Interaction_copyist();
				item1->m_interaction_message_type = interaction_message_type_e::single;
				item1->m_subtype = interaction_e::health;
				item1->m_value = item;
				item1->apply_effect(p->m_object, nullptr);
			}
			reaction = p->m_unit_body_part->m_item->get_effect(interaction_e::damage);
			if (reaction)
			{
				Object_interaction* msg = reaction->clone();
				msg->apply_effect(p->m_object, nullptr);
			}
		}
		p->m_object->update_interaction();
		p->m_object->event_update(VoidEventArgs());
	}
}

std::string Action_hit::get_description(Parameter* parameter)
{
	P_unit_interaction* p = static_cast<P_unit_interaction*>(parameter);
	std::string s("Атаковать ");
	s += p->m_object->m_name + ".";
	return s;
}

Action_equip::Action_equip()
{
	m_kind = action_e::equip;
	m_icon = Application::instance().m_graph->m_actions[9];
}

void Action_equip::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	Parameter_GameObject* p = new Parameter_GameObject();
	result = Application::instance().command_select_object_on_map();
	if (result)
	{
		p->m_object = static_cast<P_object*>(result)->m_object;
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
	Application::instance().command_open_body(p->m_object);
	Application::instance().m_message_queue.m_busy = false;
}

bool Action_equip::check(Parameter* parameter)
{
	m_error = "";
	return true;
}


void Action_equip::perfom(Parameter* parameter)
{
}

std::string Action_equip::get_description(Parameter* parameter)
{
	return "";
}

Action_show_parameters::Action_show_parameters()
{
	m_kind = action_e::show_parameters;
	m_icon = Application::instance().m_graph->m_actions[10];
}

void Action_show_parameters::interaction_handler()
{
	Action::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	P_object* p = new P_object();
	result = Application::instance().command_select_object_on_map();
	if (result)
	{
		p->m_object = static_cast<P_object*>(result)->m_object;
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
	Application::instance().command_gui_show_characterization(p->m_object);
	Application::instance().m_message_queue.m_busy = false;
}

std::string Action_show_parameters::get_description(Parameter* parameter)
{
	P_object* p = static_cast<P_object*>(parameter);
	std::string s("Показать меню характеристик для ");
	s += p->m_object->m_name + ".";
	return s;
}

