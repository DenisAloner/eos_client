#include "game/Action.h"
#include "game/Application.h"
#include "game/ApplicationGUI.h"
#include "game/ActionManager.h"
#include "game/GameObject.h"
#include "game/Parameter.h"
#include "game/graphics/GUI_MapViewer.h"
#include "game/graphics/GUI_TextBox.h"
#include "log.h"
#include  "impact\Effect.h"

Action::Action(void)
{
	m_interaction_message_type = interaction_message_type_e::action;
	m_kind = action_e::move;
	m_name = "Нет";
	m_error = "";
	m_decay = 1;
}

Action::~Action(void)
{
}

Object_interaction* Action::clone()
{
	LOG(INFO) << std::to_string(m_index);
	return Application::instance().m_actions[m_index];
}


bool Action::check(Parameter* parameter)
{
	return true;
}


void Action::perfom(Parameter* parameter)
{
}


void Action::interaction_handler(Parameter* arg)
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

void Action::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::action;
	fwrite(&t, sizeof(type_e), 1, file);
	LOG(INFO)<< m_name <<" " <<std::to_string(m_index);
	fwrite(&m_index, sizeof(size_t), 1, file);
}

void Action::load()
{
}


Action_wrapper::Action_wrapper()
{
	m_action = nullptr;
	m_parameter = nullptr;
	m_decay = 0;
	m_prefix = new Interaction_prefix();
	m_prefix->m_interaction_message_type = interaction_message_type_e::single;
	m_prefix->m_subtype = effect_prefix_e::action;
	m_prefix->m_value = this;
}

Action_wrapper* Action_wrapper::clone()
{
	Action_wrapper* result = new Action_wrapper();
	result->m_action = m_action;
	result->m_parameter = m_parameter;
	result->m_decay = m_decay;
	return result;
}

void Action_wrapper::reset_serialization_index()
{
	m_serialization_index = 0;
	if (m_parameter)
	{
		if (m_parameter->m_serialization_index > 1)
		{
			m_parameter->reset_serialization_index();
		}
	}
}

void Action_wrapper::save()
{
	LOG(INFO) << "Выполняемое действие";
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::action_wrapper;
	fwrite(&t, sizeof(type_e), 1, file);
	fwrite(&m_decay, sizeof(int), 1, file);
	Serialization_manager::instance().serialize(m_action);
	Serialization_manager::instance().serialize(m_parameter);
	LOG(INFO) << "Конец выполняемого действия";
}

void Action_wrapper::load()
{
	LOG(INFO) << "Выполняемое действие";
	FILE* file = Serialization_manager::instance().m_file;
	fread(&m_decay, sizeof(int), 1, file);
	m_action = dynamic_cast<Action*>(Serialization_manager::instance().deserialize());
	m_parameter = dynamic_cast<Parameter*>(Serialization_manager::instance().deserialize());
	LOG(INFO) << "Конец выполняемого действия";
}

void Action_wrapper::set(GameObject* unit, Action* action, Parameter* parameter)
{
	m_action = action;
	m_parameter = parameter;
	m_decay = m_action->m_decay;
	m_prefix->apply_effect(unit, this);
	done = true;
}

void Action_wrapper::update()
{
	if (m_action)
	{
		if (done)
		{
			m_action->perfom(m_parameter);
			done = false;
		}
		m_decay -= 1;
		if (m_decay <= 0)
		{
			/*if (m_action->m_error != "")
			{
			m_GUI->DescriptionBox->add_item_control(new GUI_Text(A->m_action->m_error));
			}*/
			m_action = nullptr;
		}
	}
}

ActionClass_Move::ActionClass_Move()
{
	m_kind = action_e::move;
	m_icon = Application::instance().m_graph->m_actions[0];
	m_name = "Идти";
	m_decay = 2;
}


ActionClass_Move::~ActionClass_Move()
{
}


void ActionClass_Move::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
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
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}

bool ActionClass_Move::check(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	return Application::instance().command_check_position(p->m_object, p->m_place, p->m_map);
}

action_move_step::action_move_step()
{
	m_kind = action_e::move_step;
	m_icon = Application::instance().m_graph->m_actions[0];
	m_name = "Идти на шаг";
	m_decay = 2;
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
		//LOG(INFO) << p->m_object->m_name << "  " << "двигается:" << std::to_string(p->m_place->x) << "," << std::to_string(p->m_place->y);
	}
	else {
		LOG(INFO) << "errror";
		//LOG(INFO) << p->m_object->m_name << " движение не вышло";
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
		if (prop->m_value * 10 < p->m_object->m_active_state->m_weight)
		{
			m_error = "Вы слишком слабы: нужно " + std::to_string(p->m_object->m_active_state->m_weight / 10) + " силы.";
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


void ActionClass_Push::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
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
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
	Application::instance().m_GUI->MapViewer->m_hints.pop_front();
}


ActionClass_Turn::ActionClass_Turn(void)
{
	m_kind = action_e::turn;
	m_icon = Application::instance().m_graph->m_actions[2];
	m_name = "Повернуться";
	m_decay = 4;
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
		//p->m_map->turn_object(p->m_object);
	}
}


void ActionClass_Turn::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
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
	p->m_object = Application::instance().m_GUI->MapViewer->m_player->m_object;
	Application::instance().m_action_manager->add(new GameTask(this, p));
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


void Action_OpenInventory::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
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

void Action_CellInfo::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
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

void action_set_motion_path::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	Parameter_Position* p = new Parameter_Position();
	p->m_object = Application::instance().m_GUI->MapViewer->m_player->m_object;
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	//Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_line(Application::instance().m_GUI->MapViewer, p->m_object->cell()));
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
		//Application::instance().m_GUI->MapViewer->m_hints.pop_front();
		return;
	}
	//p->m_map->bresenham_line(p->m_object->cell(), p->m_place, [p](MapCell* a) { Application::instance().m_action_manager->add(new GameTask(Application::instance().m_actions[action_e::move_step], new Parameter_Position(p->m_object, a, p->m_map))); });
	
	Path::instance().map_costing(p->m_map, p->m_object, p->m_place, 40);
	std::vector<MapCell*>* path;
	path = Path::instance().get_path_to_cell();
	if (path)
	{
		Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_path(Application::instance().m_GUI->MapViewer, path));
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Следовать под данному пути? [Y/N]"));
		if(Application::instance().command_agreement())
		{
			for (int i = 0; i < path->size(); i++)
			{
				Parameter_Position* P;
				P = new Parameter_Position();
				P->m_object = p->m_object;
				P->m_place = (*path)[(path->size() - 1) - i];
				P->m_map = p->m_map;
				Application::instance().m_action_manager->add(new GameTask(Application::instance().m_actions[action_e::move_step], P));
			}
		}
		else
		{
			Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Игрок отменил действие."));
		}
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
		Path::instance().m_heap.m_items.clear();
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Нет пути в указанную клетку."));
	}
	Application::instance().m_message_queue.m_busy = false;
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
	m_name = "Поднять";
}

void Action_pick::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
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
		case ParameterKind::parameter_kind_owner:
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
	Application::instance().m_action_manager->add(new GameTask(this, p));
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
	m_name = "Открыть ";
}

void Action_open::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
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
	Application::instance().m_action_manager->add(new GameTask(this, p));
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
	m_decay = 1;
	m_name = "Ударить без оружия";
}

void Action_hit::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
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
	Application::instance().m_action_manager->add(new GameTask(this, p));
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
		LOG(INFO)  << p->m_unit->m_name << "  " << "атакует"<<"   "<<std::to_string(m_decay);
	}
}

action_hit_melee::action_hit_melee()
{
	m_kind = action_e::hit_melee;
	m_icon = Application::instance().m_graph->m_actions[8];
	m_name = "Ударить оружием";
}

void action_hit_melee::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	P_interaction_cell* arg_p = static_cast<P_interaction_cell*>(arg);
	P_interaction_cell* p = new P_interaction_cell();
	if(arg_p)
	{
		p->m_unit = arg_p->m_unit;
		p->m_unit_body_part = arg_p->m_unit_body_part;
		p->m_object = arg_p->m_object;
		p->m_cell = arg_p->m_cell;
	}
	if (!p->m_unit)
	{ 
		p->m_unit = Application::instance().m_GUI->MapViewer->m_player->m_object; 
	}
	if (!p->m_unit_body_part)
	{
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
	}
	if (!p->m_object)
	{
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
	}
	if(!p->m_cell)
	{
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
	}
	Application::instance().m_action_manager->add(new GameTask(this, p));
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
			srand(time(NULL));
			Parameter_list* dexterity_subject = p->m_unit->get_parameter(interaction_e::dexterity);
			Parameter_list* dexterity_object = p->m_object->get_parameter(interaction_e::dexterity);
			Parameter_list* evasion_skill_object = p->m_object->get_parameter(interaction_e::evasion_skill);
			int evasion;
			if (dexterity_subject->m_value!=0)
			{
				evasion = evasion_skill_object->m_value / 1000 * ((float)dexterity_object->m_value / dexterity_subject->m_value / 2);
			}
			else
			{
				evasion = 100;
			}
			if (evasion > 100) { evasion = 100; }
			Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(std::to_string(evasion)));
			if (rand() % 100>evasion)
			{
				Parameter_list* str = p->m_unit->get_parameter(interaction_e::strength);
				Parameter_list* ms = p->m_unit->get_parameter(interaction_e::skill_sword);
				Parameter_list* dws = p->m_unit_body_part->m_item->get_parameter(interaction_e::demand_weapon_skill);
				Parameter_list* wd = p->m_unit_body_part->m_item->get_parameter(interaction_e::weapon_damage);
				Parameter_list* sb = p->m_unit_body_part->m_item->get_parameter(interaction_e::strength_bonus);
				int accuracy = (ms->m_value - dws->m_value);
				int light = (p->m_cell->m_light.R > p->m_cell->m_light.G ? p->m_cell->m_light.R : p->m_cell->m_light.G);
				light = (light > p->m_cell->m_light.B ? light : p->m_cell->m_light.B);
				if (light > 100) { light = 100; };
				if (accuracy > 0)
				{
					accuracy = (ms->m_value + rand() % accuracy)*(light + rand() % (100 - light + 1)*0.5);
					Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(std::to_string(accuracy*0.0000001)));
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
					item->m_value = -accuracy*0.0000001*sb->m_value*0.01*wd->m_value*str->m_value*0.001;
					Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(std::to_string(item->m_value)));
					Interaction_copyist* item1 = new Interaction_copyist();
					item1->m_interaction_message_type = interaction_message_type_e::single;
					item1->m_subtype = interaction_e::health;
					item1->m_value = item;
					item1->apply_effect(p->m_object, nullptr);
					ms->m_basic_value += 1;
					ms->update();
				}
			} else { Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Объект уклонился!")); }
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

void Action_equip::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
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

void Action_show_parameters::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
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

Action_use::Action_use()
{
	m_kind = action_e::use;
	m_icon = Application::instance().m_graph->m_actions[11];
	m_name = "Применить";
	m_decay = 1;
}

void Action_use::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	P_unit_interaction* p = new P_unit_interaction();
	result = Application::instance().command_select_object();
	if (result)
	{
		p->m_object = static_cast<P_object*>(result)->m_object;
		std::string a = "Выбран ";
		a.append(p->m_unit->m_name);
		a = a + ".";
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(a));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	result = Application::instance().command_select_object();
	if (result)
	{
		p->m_unit = static_cast<P_object*>(result)->m_object;
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


bool Action_use::check(Parameter* parameter)
{
	return true;
}

void Action_use::perfom(Parameter* parameter)
{
	P_unit_interaction* p = static_cast<P_unit_interaction*>(parameter);
	if (check(p))
	{
		p->m_unit->set_direction(Game_algorithm::turn_to_object(p->m_unit, p->m_object));
		auto reaction = p->m_object->get_effect(interaction_e::use);
		if (reaction)
		{
			Object_interaction* msg = reaction->clone();
			msg->apply_effect(p->m_unit, nullptr);
		}
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
		Application::instance().m_GUI->MapViewer->m_map->m_object_manager.m_items.remove(p->m_object);
		p->m_unit->update_interaction();
		p->m_unit->event_update(VoidEventArgs());
	}
}

std::string Action_use::get_description(Parameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	std::string s("Применять");
	return s;
}

Action_save::Action_save()
{
	m_kind = action_e::save;
	m_icon = Application::instance().m_graph->m_actions[12];
}

void Action_save::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Serialization_manager::instance().save("save", Application::instance().m_GUI->MapViewer->m_map);
	Application::instance().m_message_queue.m_busy = false;
}

std::string Action_save::get_description(Parameter* parameter)
{
	std::string s("Игра сохранена");
	return s;
}

Action_autoexplore::Action_autoexplore()
{
	m_kind = action_e::autoexplore;
	m_icon = Application::instance().m_graph->m_actions[13];
}

void Action_autoexplore::interaction_handler(Parameter* arg)
{
	//Action::interaction_handler();
	//Application::instance().m_message_queue.m_busy = true;
	//Parameter* result;
	//P_object* p = new P_object();
	//result = Application::instance().command_select_object_on_map();
	//if (result)
	//{
	//	p->m_object = static_cast<P_object*>(result)->m_object;
	//	std::string a = "Выбран ";
	//	a.append(p->m_object->m_name);
	//	a = a + ".";
	//	Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(a));
	//}
	//else
	//{
	//	Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
	//	Application::instance().m_message_queue.m_busy = false;
	//	return;
	//}
	//Application::instance().command_gui_show_characterization(p->m_object);
	/*Application::instance().m_message_queue.m_busy = false;*/

	//P_object* p;
	//p->m_unit = Application::instance().m_GUI->MapViewer->m_player->m_object;
	Application::instance().m_action_manager->add(new GameTask(this, nullptr));
}

std::string Action_autoexplore::get_description(Parameter* parameter)
{
	return "Автоисследование";
}

bool Action_autoexplore::get_child(GameTask*& task)
{
	GameMap* map = Application::instance().m_GUI->MapViewer->m_map;
	map->m_dijkstra_map->calculate_cost_autoexplore(map, Application::instance().m_GUI->MapViewer->m_player->m_object);
	map->m_dijkstra_map->trace();
	MapCell* c = map->m_dijkstra_map->next(Application::instance().m_GUI->MapViewer->m_player->m_object);
	if (c)
	{
		Parameter_Position* P;
		P = new Parameter_Position();
		P->m_object = Application::instance().m_GUI->MapViewer->m_player->m_object;
		P->m_place = c;
		P->m_map = map;
		task = new GameTask(Application::instance().m_actions[action_e::move_step], P);
	}
	else task = nullptr;
	return true;
}

Action_shoot::Action_shoot()
{
	m_kind = action_e::shoot;
	m_icon = Application::instance().m_graph->m_actions[14];
	m_decay = 1;
	m_name = "Выстрелить из оружия";
}

bool Action_shoot::check_cell(MapCell* a)
{
	if (!m_arg->m_unit->is_own(a))
	{
		MapCell* cell = m_arg->m_unit->get_center(a);
		int distance = (std::max)(abs(a->x-cell->x)+ m_arg->m_unit->m_active_state->m_size.x/2, abs(a->y - cell->y) + m_arg->m_unit->m_active_state->m_size.y / 2);
		Parameter_list* range = m_arg->m_unit_body_part->m_item->get_parameter(interaction_e::weapon_range);
		return distance <= range->m_value;
	}
	return true;
}

void Action_shoot::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* result;
	P_interaction_cell* arg_p = static_cast<P_interaction_cell*>(arg);
	P_interaction_cell* p = new P_interaction_cell();
	bool valid;
	if (arg_p)
	{
		p->m_unit = arg_p->m_unit;
		p->m_unit_body_part = arg_p->m_unit_body_part;
		p->m_object = arg_p->m_object;
		p->m_cell = arg_p->m_cell;
	}
	m_arg = p;
	if (!p->m_unit)
	{
		p->m_unit = Application::instance().m_GUI->MapViewer->m_player->m_object;
	}
	if (!p->m_unit_body_part)
	{
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
	}
	/*if (!p->m_object)
	{
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
	}*/
	if (!p->m_cell)
	{
		Parameter_list* wr = p->m_unit_body_part->m_item->get_parameter(interaction_e::weapon_range);
		Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_shoot(Application::instance().m_GUI->MapViewer, p->m_unit, wr->m_value));
		Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_weapon_range(Application::instance().m_GUI->MapViewer, p->m_unit, wr->m_value));
		valid = false;
		while (!valid)
		{
			result = Application::instance().command_select_location(p->m_object);
			if (result)
			{
				p->m_cell = static_cast<MapCell*>(static_cast<P_object_owner*>(result)->m_cell);
				if (check_cell(p->m_cell))
				{
					Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_cell->x) + "," + std::to_string(p->m_cell->y) + "}:"));
					valid = true;
				}
				else
				{
					Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Слишком большая дистанция!"));
				}
			}
			else
			{
				Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Действие отменено."));
				Application::instance().m_message_queue.m_busy = false;
				Application::instance().m_GUI->MapViewer->m_hints.pop_front();
				Application::instance().m_GUI->MapViewer->m_hints.pop_front();
				return;
			}
		}
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
	}
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}

bool Action_shoot::check(Parameter* parameter)
{
	m_error = "";
	/*P_unit_interaction* p = static_cast<P_unit_interaction*>(parameter);
	MapCell* cell;
	if (!Game_algorithm::check_distance(p->m_unit->cell(), p->m_unit->m_active_state->m_size, p->m_object->cell(), p->m_object->m_active_state->m_size))
	{
		m_error = "Вы слишком далеко от " + p->m_object->m_name;
		return false;
	};*/
	return true;
}

bool Action_shoot::process_cell(MapCell* a)
{
	for (auto c_object =a->m_items.begin(); c_object != a->m_items.end(); c_object++)
	{
		if (!m_arg->m_unit->is_own(a))
		{
			if (!(*c_object)->get_tag(object_tag_e::pass_able))
			{
				return true;
			}
		}
	}
	return false;
}

void Action_shoot::perfom(Parameter* parameter)
{
	P_interaction_cell* p = static_cast<P_interaction_cell*>(parameter);
	if (check(p))
	{
		m_arg = p;
		MapCell* c=p->m_cell->m_map->bresenham_line2(p->m_unit->cell(), p->m_cell, std::bind(&Action_shoot::process_cell, this, std::placeholders::_1));
		GameObject* obj = Application::instance().m_game_object_manager->new_object("arrow");
		c->m_map->add_to_map(obj, c);
		/*p->m_object->update_interaction();
		p->m_object->event_update(VoidEventArgs());*/
	}
}

std::string Action_shoot::get_description(Parameter* parameter)
{
	P_unit_interaction* p = static_cast<P_unit_interaction*>(parameter);
	std::string s("Атаковать ");
	//s += p->m_object->m_name + ".";
	return s;
}