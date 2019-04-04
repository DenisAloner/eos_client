#include "game/Action.h"
#include "game/Application.h"
#include "game/ApplicationGUI.h"
#include "game/ActionManager.h"
#include "game/GameObject.h"
#include "game/Parameter.h"
#include "game/graphics/GUI_MapViewer.h"
#include  "impact/Effect.h"

Action::Action(void)
{
	m_interaction_message_type = interaction_message_type_e::action;
	m_kind = action_e::move;
	m_name = u"Нет";
	m_error = u"";
	m_decay = 10;
	m_animation = animation_e::idle;
}

Action::~Action(void)
{
}

Object_interaction* Action::clone()
{
	return Application::instance().m_actions[m_index];
}


bool Action::check(Parameter* parameter)
{
	return true;
}


char Action::perfom(Parameter* parameter)
{
	return 0;
}


void Action::interaction_handler(Parameter* arg)
{
	/*Game->PlaySound1();*/
	if (Application::instance().m_message_queue.m_busy)
	{
		if (Application::instance().m_message_queue.m_reader)
		{
			Parameter* p = new Parameter(parameter_type_e::cancel);
			Application::instance().m_message_queue.push(p);
		}
		while (Application::instance().m_message_queue.m_busy){}
	}
}

Packer_generic& Action::get_packer()
{
	return Packer<Action>::Instance();
}

std::u16string Packer<Action>::to_json(iSerializable* value,Parser_context& context)
{
	std::u16string result = value->to_json(context);
	if (result.empty())
	{
		std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\"}";
		return out;
	}
	else
	{
		std::u16string out = u"{\"$type\":\"" + value->get_packer().get_type_name() + u"\"," + result + u"}";
		return out;
	}
}

std::string Packer<Action>::to_binary(iSerializable* value, Parser_context& context)
{
	std::size_t id = value->get_packer().get_type_id() + 1;
	return std::string(reinterpret_cast<const char*>(&id), sizeof(std::size_t)) + value->to_binary(context);
}

iSerializable* Packer<Action>::from_json(scheme_map_t* value, Parser_context& context)
{
	return Application::instance().m_actions[Parser::m_json_action_e.get_enum(Parser::UTF16_to_CP1251(Parser::get_value((*value)[u"value"])))];
}

iSerializable* Packer<Action>::from_binary(const std::string& value, std::size_t& pos, Parser_context& context)
{
	action_e::type x;
	Parser::from_binary<action_e::type>(value, x, pos,context);
	return Application::instance().m_actions[x];
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
	if (m_action)
	{
		if (m_action->m_serialization_index > 0)
		{
			m_action->reset_serialization_index();
		}
	}
	if (m_parameter)
	{
		if (m_parameter->m_serialization_index > 0)
		{
			m_parameter->reset_serialization_index();
		}
	}
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
			//m_action->perfom(m_parameter);
			m_action = nullptr;
		}
	}
}

Packer_generic& Action_wrapper::get_packer()
{
	return Packer<Action_wrapper>::Instance();
}

ActionClass_Move::ActionClass_Move()
{
	m_kind = action_e::move;
	m_icon = Application::instance().m_graph->m_actions[0];
	m_name = u"Идти";
	m_decay = 10;
	m_parameter_kind = parameter_type_e::position;
}


ActionClass_Move::~ActionClass_Move()
{
}


void ActionClass_Move::interaction_handler(Parameter* parameter)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	MapCell* temp;
	Parameter* out_parameter;
	if (parameter)
	{
		out_parameter = parameter->clone();
	}
	else
	{
		out_parameter = new Parameter(m_parameter_kind);
	}
	Parameter& p(*out_parameter);
	if (!p[0])
	{
		p[0].set(Application::instance().m_world->m_player->m_object);
	}
	if (!p[1])
	{
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.push_front(new mapviewer_object_move( p[0].m_object));
		Application::instance().m_UI_mutex.unlock();
		temp = Application::instance().command_select_location(p[0].m_object);
		if (temp)
		{
			p[1].set(Game_algorithm::step_in_direction(p[0].m_object, Game_algorithm::turn_to_cell(p[0].m_object, temp)));
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, u"Выбрана клетка {" + Parser::CP1251_to_UTF16(std::to_string(p[1].m_cell->x)) + u"," + Parser::CP1251_to_UTF16(std::to_string(p[1].m_cell->y)) + u"}"));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_UI_mutex.lock();
			Application::instance().m_gui_controller.m_hints.pop_front();
			Application::instance().m_UI_mutex.unlock();
			return;
		}
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.pop_front();
		Application::instance().m_UI_mutex.unlock();
	}
	Application::instance().m_action_manager->add(new GameTask(this, out_parameter));
	Application::instance().m_message_queue.m_busy = false;
}

bool ActionClass_Move::check(Parameter* parameter)
{
	Parameter& p(*parameter);
	return Application::instance().command_check_position(p[0].m_object, p[1].m_cell, p[1].m_cell->m_map);
}

action_move_step::action_move_step()
{
	m_kind = action_e::move_step;
	m_icon = Application::instance().m_graph->m_actions[0];
	m_name = u"Идти на шаг";
	m_decay = 10;
	m_parameter_kind = parameter_type_e::position;
	m_animation = animation_e::move;
}

bool action_move_step::check(Parameter* parameter)
{
	Parameter& p(*parameter);
	if (abs(p[0].m_object->cell()->x - p[1].m_cell->x) < 2 && abs(p[0].m_object->cell()->y - p[1].m_cell->y)< 2)
	{
		return Application::instance().command_check_position(p[0].m_object, p[1].m_cell, p[1].m_cell->m_map);
	}
	else return false;
}

char ActionClass_Move::perfom(Parameter* parameter)
{
	Parameter& p(*parameter);
	if (check(parameter))
	{
		int dx = p[1].m_cell->x - p[0].m_object->cell()->x;
		int dy = p[1].m_cell->y - p[0].m_object->cell()->y;
		switch (dy)
		{
		case 1:
		{
			switch (dx)
			{
			case -1: {p[0].m_object->set_direction(object_direction_e::topleft); break; }
			case 0: {p[0].m_object->set_direction(object_direction_e::top); break; }
			case 1: {p[0].m_object->set_direction(object_direction_e::topright); break; }
			}
			break;
		}
		case 0:
		{
			switch (dx)
			{
			case -1: {p[0].m_object->set_direction(object_direction_e::left); break; }
			case 1: {p[0].m_object->set_direction(object_direction_e::right); break; }
			}
			break;
		}
		case -1:
		{
			switch (dx)
			{
			case -1: {p[0].m_object->set_direction(object_direction_e::downleft); break; }
			case 0: {p[0].m_object->set_direction(object_direction_e::down); break; }
			case 1: {p[0].m_object->set_direction(object_direction_e::downright); break; }
			}
			break;
		}
		}
		p[1].m_cell->m_map->move_object(p[0].m_object, *p[1].m_cell);
		//LOG(INFO) << p->m_object->m_name << "  " << "двигается:" << std::to_string(p->m_place->x) << "," << std::to_string(p->m_place->y);
	}
	else {
		//LOG(INFO) << p->m_object->m_name << " движение не вышло";
		return 1;
	}
		return 0;
	
}


std::u16string ActionClass_Move::get_description(Parameter* parameter)
{
	Parameter& p(*parameter);
	std::u16string s(u"Идти");
	if (p[1])
	{
		s += u" в X:";
		s += Parser::CP1251_to_UTF16(std::to_string(p[1].m_cell->x));
		s += u",Y:";
		s += Parser::CP1251_to_UTF16(std::to_string(p[1].m_cell->y));
	}
	return s;
}

ActionClass_Push::ActionClass_Push(void)
{
	m_kind = action_e::push;
	m_icon = Application::instance().m_graph->m_actions[1];
	m_name = u"Толкать";
}


ActionClass_Push::~ActionClass_Push(void)
{
}


std::u16string ActionClass_Push::get_description(Parameter* parameter)
{
	Parameter& p(*parameter);
	std::u16string s(u"Переместить ");
	//s += p->m_object->m_name;
	//s += " в X:" + std::to_string(p->m_place->x);
	//s += ",Y:";
	//s += std::to_string(p->m_place->y);
	return s;
}


bool ActionClass_Push::check(Parameter* parameter)
{
	//m_error = "";
	//Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	//MapCell* cell;
	//auto* prop = p->m_unit->get_parameter(interaction_e::strength);
	//if (prop != nullptr)
	//{
	//	if (prop->m_value * 10 < p->m_object->m_active_state->m_weight)
	//	{
	//		m_error = "Вы слишком слабы: нужно " + std::to_string(p->m_object->m_active_state->m_weight / 10) + " силы.";
	//		return false;
	//	}
	//}
	//else { return false; }

	//if (abs(p->m_unit->cell()->x - p->m_object->cell()->x)<2 || abs(p->m_unit->cell()->x - p->m_unit->m_active_state->m_size.x + 1 - p->m_object->cell()->x)<2 || abs(p->m_unit->cell()->x - p->m_unit->m_active_state->m_size.x + 1 - (p->m_object->cell()->x - p->m_object->m_active_state->m_size.x + 1))<2 || abs(p->m_unit->cell()->x - (p->m_object->cell()->x - p->m_object->m_active_state->m_size.x + 1))<2)
	//{
	//	if (abs(p->m_unit->cell()->y - p->m_object->cell()->y) < 2 || abs(p->m_unit->cell()->y + p->m_unit->m_active_state->m_size.y - 1 - p->m_object->cell()->y) < 2 || abs(p->m_unit->cell()->y + p->m_unit->m_active_state->m_size.y - 1 - (p->m_object->cell()->y + p->m_object->m_active_state->m_size.y - 1)) < 2 || abs(p->m_unit->cell()->y - (p->m_object->cell()->y + p->m_object->m_active_state->m_size.y - 1)) < 2)
	//	{
	//		for (int i = 0; i < p->m_object->m_active_state->m_size.y; i++)
	//		{
	//			for (int j = 0; j < p->m_object->m_active_state->m_size.x; j++)
	//			{
	//				cell = p->m_map->m_items[p->m_place->y + i][p->m_place->x - j];
	//				if (cell == nullptr){ return false; }
	//				for (std::list<GameObject*>::iterator item = cell->m_items.begin(); item != cell->m_items.end(); item++)
	//				{
	//					if (p->m_object != (*item) && p->m_unit != (*item))
	//					{
	//						if (!(*item)->get_stat(object_tag_e::pass_able))
	//						{
	//							return false;
	//						}
	//					}
	//				}
	//			}
	//		}
	//		for (int i = 0; i < p->m_unit->m_active_state->m_size.y; i++)
	//		{
	//			for (int j = 0; j < p->m_unit->m_active_state->m_size.x; j++)
	//			{
	//				cell = p->m_map->m_items[p->m_unit->cell()->y + (p->m_place->y - p->m_object->cell()->y) + i][p->m_unit->cell()->x + (p->m_place->x - p->m_object->cell()->x) - j];
	//				if (cell == nullptr){ return false; }
	//				for (std::list<GameObject*>::iterator item = cell->m_items.begin(); item != cell->m_items.end(); item++)
	//				{
	//					if (p->m_object != (*item) && p->m_unit != (*item))
	//					{
	//						if (!(*item)->get_stat(object_tag_e::pass_able))
	//						{
	//							return false;
	//						}
	//					}
	//				}
	//			}
	//		}
	//		return true;
	//	}
	//}
	return false;
}


char ActionClass_Push::perfom(Parameter* parameter)
{
	//Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	//if (check(p))
	//{
	//	p->m_map->move_object(p->m_unit, p->m_map->m_items[p->m_unit->cell()->y + (p->m_place->y - p->m_object->cell()->y)][p->m_unit->cell()->x + (p->m_place->x - p->m_object->cell()->x)]);
	//	p->m_map->move_object(p->m_object, p->m_place);
	//}
	//else return 1;
	return 0;
}


void ActionClass_Push::interaction_handler(Parameter* arg)
{
	//Action::interaction_handler(nullptr);
	//Application::instance().m_message_queue.m_busy = true;
	//Parameter* result;
	//Parameter_MoveObjectByUnit* p = new Parameter_MoveObjectByUnit();
	//p->m_unit = Application::instance().m_GUI->MapViewer->m_player->m_object;
	//p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	//Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_area(Application::instance().m_GUI->MapViewer, p->m_unit, true));
	//result = Application::instance().command_select_object_on_map();
	//if (result)
	//{
	//	p->m_object = static_cast<P_object*>(result)->m_object;
	//	std::string a = "Выбран ";
	//	a.append(p->m_object->m_name);
	//	a = a + ".";
	//	Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
	//}
	//else
	//{
	//	Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::string("Действие отменено")));
	//	Application::instance().m_message_queue.m_busy = false;
	//	Application::instance().m_GUI->MapViewer->m_hints.pop_front();
	//	return;
	//}
	//Application::instance().m_GUI->MapViewer->m_hints.pop_front();
	//Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_area(Application::instance().m_GUI->MapViewer, p->m_object, false));
	//result = Application::instance().command_select_location(p->m_object);
	//if (result)
	//{
	//	p->m_place = static_cast<MapCell*>(static_cast<P_object_owner*>(result)->m_cell);
	//	Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, "Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}"));
	//}
	//else
	//{
	//	Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::string("Действие отменено")));
	//	Application::instance().m_message_queue.m_busy = false;
	//	Application::instance().m_GUI->MapViewer->m_hints.pop_front();
	//	return;
	//}
	//Application::instance().m_action_manager->add(new GameTask(this, p));
	//Application::instance().m_message_queue.m_busy = false;
	//Application::instance().m_GUI->MapViewer->m_hints.pop_front();
}


ActionClass_Turn::ActionClass_Turn(void)
{
	m_kind = action_e::turn;
	m_icon = Application::instance().m_graph->m_actions[2];
	m_name = u"Повернуться";
	m_decay = 2;
	m_parameter_kind = parameter_type_e::direction;
}


ActionClass_Turn::~ActionClass_Turn(void)
{
}


std::u16string ActionClass_Turn::get_description(Parameter* parameter)
{
	Parameter& p(*parameter);
	std::u16string s(u"Повернуть");
	if (parameter)
	{
		Parameter& p(*parameter);
		if (p[0])
		{
			s += u" " + p[0].m_object->m_name;
		}
	}
	return s;
}


bool ActionClass_Turn::check(Parameter* parameter)
{
	/*Parameter_direction* p = static_cast<Parameter_direction*>(parameter);*/
	return true;
}

char ActionClass_Turn::perfom(Parameter* parameter)
{
	Parameter& p(*parameter);
	if (check(parameter))
	{
		p[0].m_object->set_direction(p[1].m_direction);
	}
	else return 1;
	return 0;
}

void ActionClass_Turn::interaction_handler(Parameter* parameter)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* out_parameter;
	if (parameter)
	{
		out_parameter = parameter->clone();
	}
	else
	{
		out_parameter = new Parameter(m_parameter_kind);
	}
	Parameter& p(*out_parameter);
	if (!p[0])
	{
		GameObject* result = Application::instance().command_select_object_on_map();
		if (result)
		{
			p[0].set(result);
			std::u16string a = u"Выбран ";
			a.append(p[0].m_object->m_name);
			a = a + u".";
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			return;
		}
	}
	if (!p[1])
	{
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.push_front(new mapviewer_object_rotate( p[0].m_object));
		Application::instance().m_UI_mutex.unlock();
		MapCell* result = Application::instance().command_select_location(p[0].m_object);
		if (result)
		{
			p[1].set(Game_algorithm::turn_to_cell(p[0].m_object, result));
			Application::instance().m_UI_mutex.lock();
			Application::instance().m_gui_controller.m_hints.pop_front();
			Application::instance().m_UI_mutex.unlock();
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Направление выбрано")));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_UI_mutex.lock();
			Application::instance().m_gui_controller.m_hints.pop_front();
			Application::instance().m_UI_mutex.unlock();
			return;
		}
	}
	rotate_direction_e rotate = Game_algorithm::get_rotation(p[0].m_object->m_direction, p[1].m_direction);
	for (auto i = p[0].m_object->m_direction; i!= p[1].m_direction;)
	{
		i = i + rotate;
		Application::instance().m_action_manager->add(new GameTask(this, new Parameter(parameter_type_e::direction,p[0].m_object,i)));
	}
	Application::instance().m_message_queue.m_busy = false;
}


Action_OpenInventory::Action_OpenInventory()
{
	m_kind = action_e::open_inventory;
	m_icon = Application::instance().m_graph->m_actions[3];
	m_parameter_kind = parameter_type_e::object;
}


Action_OpenInventory::~Action_OpenInventory()
{
}


void Action_OpenInventory::interaction_handler(Parameter* parameter)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* out_parameter;
	if (parameter)
	{
		out_parameter = parameter->clone();
	}
	else
	{
		out_parameter = new Parameter(m_parameter_kind);
	}
	Parameter& p(*out_parameter);
	GameObject* result = Application::instance().command_select_object_on_map();
	if (result)
	{
		p[0].m_object = result;
		std::u16string a = u"Выбран ";
		a.append(p[0].m_object->m_name);
		a = a + u".";
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
	}
	else
	{
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	Application::instance().m_message_queue.m_busy = false;
}


bool Action_OpenInventory::check(Parameter* parameter)
{
	return true;
}


char Action_OpenInventory::perfom(Parameter* parameter)
{
	return 0;
}


std::u16string Action_OpenInventory::get_description(Parameter* parameter)
{
	return u"Открыть инвернтарь";
}

Action_CellInfo::Action_CellInfo()
{
	m_kind = action_e::cell_info;
	m_icon = Application::instance().m_graph->m_actions[4];
	m_name = u"Информация о клетке";
}

void Action_CellInfo::interaction_handler(Parameter* arg)
{
	//Action::interaction_handler(nullptr);
	//Application::instance().m_message_queue.m_busy = true;
	//Parameter* result;
	//Parameter_Position* p = new Parameter_Position();
	//p->m_object = nullptr;
	//p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	//result = Application::instance().command_select_location(p->m_object);
	//if (result)
	//{
	//	p->m_place = static_cast<MapCell*>(static_cast<P_object_owner*>(result)->m_cell);
	//	Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, "Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}:"));
	//	Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, " - освещение {" + std::to_string(p->m_place->m_light.R) + "," + std::to_string(p->m_place->m_light.G) + "," + std::to_string(p->m_place->m_light.B) + "}"));
	//	MapCell* c = Application::instance().m_GUI->MapViewer->m_player->m_object->cell();
	//	/*int f = Application::instance().m_GUI->MapViewer->m_player->m_fov->m_radius;
	//	int xf = p->m_place->x-c->x;
	//	int yf = p->m_place->y - c->y;
	//	if ((xf >= -f) && (xf <= f) && (yf >= -f) && (yf <=f))
	//	{
	//		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(std::to_string(Application::instance().m_GUI->MapViewer->m_player->m_fov->m_map[Application::instance().m_GUI->MapViewer->m_player->m_fov->m_middle + yf][Application::instance().m_GUI->MapViewer->m_player->m_fov->m_middle + xf].visible)));
	//	}*/
	//}
	//else
	//{
	//	Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::string("Не выбрана клетка карты")));
	//	Application::instance().m_message_queue.m_busy = false;
	//	return;
	//}
	//Application::instance().m_message_queue.m_busy = false;
}

bool Action_CellInfo::check(Parameter* parameter)
{
	return true;
}

char Action_CellInfo::perfom(Parameter* parameter)
{
	return 0;
}

std::u16string Action_CellInfo::get_description(Parameter* parameter)
{
	return u"Информация о клетке";
}

action_set_motion_path::action_set_motion_path()
{
	m_kind = action_e::set_motion_path;
	m_icon = Application::instance().m_graph->m_actions[5];
	m_name = u"Идти в указанную позицию";
}

void action_set_motion_path::interaction_handler(Parameter* parameter)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	GameObject* object = Application::instance().m_world->m_player->m_object;
	Application::instance().m_UI_mutex.lock();
	Application::instance().m_gui_controller.m_hints.push_front(new mapviewer_hint_line( object->cell(), object));
	Application::instance().m_UI_mutex.unlock();
	MapCell* cell = Application::instance().command_select_location(object);
	if (cell)
	{
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, u"Выбрана клетка {" + Parser::CP1251_to_UTF16(std::to_string(cell->x) + "," + std::to_string(cell->y) + "}")));
	}
	else
	{
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Не выбрана клетка карты")));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.pop_front();
		Application::instance().m_UI_mutex.unlock();
		return;
	}
	//p->m_map->bresenham_line(p->m_object->cell(), p->m_place, [p](MapCell* a) { Application::instance().m_action_manager->add(new GameTask(Application::instance().m_actions[action_e::move_step], new Parameter_Position(p->m_object, a, p->m_map))); });
	Application::instance().m_UI_mutex.lock();
	Application::instance().m_gui_controller.m_hints.pop_front();
	Application::instance().m_UI_mutex.unlock();
	Path::instance().map_costing(cell->m_map, object, cell, 40);
	std::vector<MapCell*>* path;
	path = Path::instance().get_path_to_cell();
	if (path)
	{
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.push_front(new mapviewer_hint_path(path,object));
		Application::instance().m_UI_mutex.unlock();
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Следовать под данному пути? [Y/N]")));
		if(Application::instance().command_agreement())
		{
			for (int i=0; i < path->size(); ++i)
			{
				Parameter* p = new Parameter(parameter_type_e::position, object, (*path)[(path->size() - 1) - i]);
				Application::instance().m_action_manager->add(new GameTask(Application::instance().m_actions[action_e::move_step], p));
			}
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Игрок отменил действие")));
		}
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.pop_front();
		Application::instance().m_UI_mutex.unlock();
		Path::instance().m_heap.m_items.clear();
	}
	else
	{
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Нет пути в указанную клетку")));
	}
	Application::instance().m_message_queue.m_busy = false;
}

char action_set_motion_path::perfom(Parameter* parameter)
{
	return 0;
}

std::u16string action_set_motion_path::get_description(Parameter* parameter)
{
	return u"Идти в указанную позицию";
}

Action_pick::Action_pick()
{
	m_kind = action_e::pick;
	m_icon = Application::instance().m_graph->m_actions[6];
	m_decay = 1;
	m_name = u"Поднять";
	m_parameter_kind = parameter_type_e::destination;
}

void Action_pick::interaction_handler(Parameter* parameter)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* out_parameter;
	if (parameter)
	{
		out_parameter = parameter->clone();
	}
	else
	{
		out_parameter = new Parameter(m_parameter_kind);
	}
	Parameter& p(*out_parameter);
	if (!p[0])
	{
		p[0].set(Application::instance().m_world->m_player->m_object);
	}
	if (!p[2])
	{
		Game_object_owner* result = Application::instance().command_select_transfer(nullptr);
		if (result)
		{
			p[2].set(result);
			//Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Выбрана клетка {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_clipboard.m_item = nullptr;
			return;
		}
	}
	if (!p[1])
	{
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.push_front(new mapviewer_hint_area( p[0].m_object, true));
		Application::instance().m_UI_mutex.unlock();
		GameObject* result = Application::instance().command_select_object();
		if (result)
		{
			p[1].set(result);
			std::u16string a = u"Выбран ";
			a.append(p[1].m_object->m_name);
			a = a + u".";
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_UI_mutex.lock();
			Application::instance().m_gui_controller.m_hints.pop_front();
			Application::instance().m_UI_mutex.unlock();
			return;
		}
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.pop_front();
		Application::instance().m_UI_mutex.unlock();
		Application::instance().m_clipboard.m_item = p[1].m_object;
	}
	Application::instance().m_action_manager->add(new GameTask(this, out_parameter));
	Application::instance().m_clipboard.m_item = nullptr;
	Application::instance().m_message_queue.m_busy = false;
}

char Action_pick::perfom(Parameter* parameter)
{
	Parameter& p(*parameter);
	if (check(parameter))
	{
		Instruction_slot_parameter* np = new Instruction_slot_parameter();
		np->m_parameter = parameter;
		np->m_result = true;
		ObjectTag::Can_transfer_object* t=static_cast<ObjectTag::Can_transfer_object*>(p[3].m_part->m_attributes.get_tag(object_tag_e::can_transfer_object));
		t->apply_effect(nullptr, np);
		if (!np->m_result) { return 1; };
		np->m_result = false;
		Application::instance().command_change_owner(np);
	}
	else return 1;
	return 0;
}

std::u16string Action_pick::get_description(Parameter* parameter)
{
	
	std::u16string s(u"Поднять");
	if (parameter)
	{
		Parameter& p(*parameter);
		if (p[1])
		{
			s += u" " + p[1].m_object->m_name;
		}
		if (p[2])
		{
			switch (p[2].m_owner->m_kind)
			{
			case entity_e::cell:
			{
				////static_cast<MapCell*>(p->m_owner)->m_items.push_back(p->m_object);
				//static_cast<MapCell*>(p[2].m_owner)->m_map->add_object(p[1].m_object, static_cast<MapCell*>(p[2].m_owner));
				//p[1].m_object->m_owner = p[2].m_owner;
				break;
			}
			case entity_e::inventory_cell:
			{
				/*static_cast<Inventory_cell*>(p[2].m_owner)->m_items = p[1].m_object;
				p[1].m_object->m_owner = p[2].m_owner;*/
				break;
			}
			case entity_e::body_part:
			{
				s += u" в " + static_cast<Object_part*>(p[2].m_owner)->m_name;
				break;
			}
			}
		}
	}
	return s;
}

void Action_pick::apply_visitor(Visitor_generic& visitor)
{
	visitor.visit(*this);
}

Action_move_out::Action_move_out()
{
	m_kind = action_e::move_out;
	m_icon = Application::instance().m_graph->m_actions[6];
	m_decay = 1;
	m_name = u"Переместить";
	m_parameter_kind = parameter_type_e::destination;
}

std::u16string Action_move_out::get_description(Parameter* parameter)
{

	std::u16string s(u"Переместить");
	if (parameter)
	{
		Parameter& p(*parameter);
		if (p[1])
		{
			s += u" " + p[1].m_object->m_name;
			switch (p[1].m_object->m_owner->m_kind)
			{
			case entity_e::cell:
			{
				////static_cast<MapCell*>(p->m_owner)->m_items.push_back(p->m_object);
				//static_cast<MapCell*>(p[2].m_owner)->m_map->add_object(p[1].m_object, static_cast<MapCell*>(p[2].m_owner));
				//p[1].m_object->m_owner = p[2].m_owner;
				break;
			}
			case entity_e::inventory_cell:
			{
				/*static_cast<Inventory_cell*>(p[2].m_owner)->m_items = p[1].m_object;
				p[1].m_object->m_owner = p[2].m_owner;*/
				break;
			}
			case entity_e::body_part:
			{
				s += u" из " + static_cast<Object_part*>(p[1].m_object->m_owner)->m_name;
				break;
			}
			}
		}
		if (p[2])
		{
			switch (p[2].m_owner->m_kind)
			{
			case entity_e::cell:
			{
				s += u" на пол ";
				break;
			}
			case entity_e::inventory_cell:
			{
				/*static_cast<Inventory_cell*>(p[2].m_owner)->m_items = p[1].m_object;
				p[1].m_object->m_owner = p[2].m_owner;*/
				break;
			}
			case entity_e::body_part:
			{
				s += u" в " + static_cast<Object_part*>(p[2].m_owner)->m_name;
				break;
			}
			}
		}
	}
	return s;
}

Action_open::Action_open()
{
	m_kind = action_e::open;
	m_icon = Application::instance().m_graph->m_actions[7];
	m_name = u"Открыть ";
}

void Action_open::interaction_handler(Parameter* arg)
{
	/*Action::interaction_handler(nullptr);
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
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
	}
	else
	{
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::string("Действие отменено")));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;*/
}

char Action_open::perfom(Parameter* parameter)
{
	/*P_object* p = static_cast<P_object*>(parameter);
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
	else return 1;*/
	return 0;
}

std::u16string Action_open::get_description(Parameter* parameter)
{
	Parameter& p(*parameter);
	std::u16string s(u"Открыть ");
	/*s += p->m_object->m_name + ".";*/
	return s;
}

Action_hit::Action_hit()
{
	m_kind = action_e::hit;
	m_icon = Application::instance().m_graph->m_actions[8];
	m_decay = 10;
	m_name = u"Ударить без оружия";
	m_parameter_kind = parameter_type_e::unit_interaction;
}

void Action_hit::interaction_handler(Parameter* arg)
{
	/*Action::interaction_handler(nullptr);
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
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::string("Действие отменено")));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_clipboard.m_items = nullptr;
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
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
	}
	else
	{
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::string("Действие отменено")));
		Application::instance().m_message_queue.m_busy = false;
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
		return;
	}
	Application::instance().m_GUI->MapViewer->m_hints.pop_front();
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;*/
}

bool Action_hit::check(Parameter* parameter)
{
	/*m_error = "";
	P_unit_interaction* p = static_cast<P_unit_interaction*>(parameter);
	MapCell* cell;
	if (!Game_algorithm::check_distance(p->m_unit->cell(), p->m_unit->m_active_state->m_size, p->m_object->cell(), p->m_object->m_active_state->m_size))
	{
		m_error = "Вы слишком далеко от " + p->m_object->m_name;
		return false;
	};*/
	return true;
}

char Action_hit::perfom(Parameter* parameter)
{
	/*P_unit_interaction* p = static_cast<P_unit_interaction*>(parameter);
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
			reaction = p->m_unit_body_part->m_items->get_effect(interaction_e::damage);
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
	else return 1;*/
	return 0;
}

action_hit_melee::action_hit_melee()
{
	m_kind = action_e::hit_melee;
	m_icon = Application::instance().m_graph->m_actions[8];
	m_name = u"Ударить оружием";
	m_parameter_kind = parameter_type_e::interaction_cell;
}

void action_hit_melee::interaction_handler(Parameter* parameter)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* out_parameter;
	if (parameter)
	{
		out_parameter = parameter->clone();
	}
	else
	{
		out_parameter = new Parameter(m_parameter_kind);
	}
	Parameter& p(*out_parameter);
	if (!p[0])
	{ 
		p[0].set(Application::instance().m_world->m_player->m_object);
	}
	if (!p[2])
	{
		Object_part* result = Application::instance().command_select_body_part();
		if (result)
		{
			p[2].set(result);
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_clipboard.m_item = nullptr;
			return;
		}
	}
	if (!p[1])
	{
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.push_front(new mapviewer_hint_area( p[0].m_object, true));
		Application::instance().m_UI_mutex.unlock();
		GameObject* result = Application::instance().command_select_object_on_map();
		if (result)
		{
			p[1].set(result);
			std::u16string a = u"Выбран ";
			a.append(p[1].m_object->m_name);
			a = a + u".";
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_UI_mutex.lock();
			Application::instance().m_gui_controller.m_hints.pop_front();
			Application::instance().m_UI_mutex.unlock();
			return;
		}
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.pop_front();
		Application::instance().m_UI_mutex.unlock();
	}
	if(!p[3])
	{
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.push_front(new mapviewer_hint_object_area( p[1].m_object));
		Application::instance().m_UI_mutex.unlock();
		MapCell* result = Application::instance().command_select_location(p[1].m_object);
		if (result)
		{
			p[3].set(result);
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, u"Выбрана клетка {" + Parser::CP1251_to_UTF16(std::to_string(p[3].m_cell->x) + "," + std::to_string(p[3].m_cell->y) + "}:")));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_UI_mutex.lock();
			Application::instance().m_gui_controller.m_hints.pop_front();
			Application::instance().m_UI_mutex.unlock();
			return;
		}
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.pop_front();
		Application::instance().m_UI_mutex.unlock();
	}
	Application::instance().m_action_manager->add(new GameTask(this, out_parameter));
	Application::instance().m_message_queue.m_busy = false;
}

char action_hit_melee::perfom(Parameter* parameter)
{
	Parameter& p(*parameter);
	Parameter_list* sbj_health = p[1].m_object->get_parameter(interaction_e::health);
	int sbj_health_old_value = sbj_health->m_value;
	if (check(parameter))
	{
		std::u16string msg= p[0].m_object->m_name + u" атакует " + p[1].m_object->m_name + u". ";
		srand(time(NULL));
		Parameter_list* dexterity_subject = p[0].m_object->get_parameter(interaction_e::dexterity);
		Parameter_list* dexterity_object = p[1].m_object->get_parameter(interaction_e::dexterity);
		Parameter_list* evasion_skill_object = p[1].m_object->get_parameter(interaction_e::evasion_skill);
		int evasion;
		if (dexterity_subject->m_value != 0)
		{
			evasion = evasion_skill_object->m_value / 1000 * ((float)dexterity_object->m_value / dexterity_subject->m_value / 2);
		}
		else
		{
			evasion = 100;
		}
		if (evasion > 100) { evasion = 100; }
		//???Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(std::to_string(evasion)));
		if (rand() % 100 > evasion)
		{
			Parameter_list* str = p[0].m_object->get_parameter(interaction_e::strength);
			Parameter_list* ms;
			int dws;
			int wd;
			int sb;
			if (p[2].m_part->m_item)
			{
				ms = p[0].m_object->get_parameter(interaction_e::skill_sword);
				dws = p[2].m_part->m_item->get_parameter(interaction_e::demand_weapon_skill)->m_value;
				wd = p[2].m_part->m_item->get_parameter(interaction_e::weapon_damage)->m_value;
				sb = p[2].m_part->m_item->get_parameter(interaction_e::strength_bonus)->m_value;
			}
			else
			{
				ms = p[0].m_object->get_parameter(interaction_e::skill_unarmed_combat);
				dws = 0;
				wd = 1;
				sb = 100;
			}
			int accuracy = (ms->m_value - dws);
			int light = (p[3].m_cell->m_light.R > p[3].m_cell->m_light.G ? p[3].m_cell->m_light.R : p[3].m_cell->m_light.G);
			light = (light > p[3].m_cell->m_light.B ? light : p[3].m_cell->m_light.B);
			if (light > 100) { light = 100; };
			if (accuracy > 0)
			{
				accuracy = (ms->m_value + rand() % accuracy) * (light + rand() % (100 - light + 1) * 0.5);
				//???Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Точность: "+std::to_string(accuracy*0.0000001)));
			}
			else
			{
				accuracy = (ms->m_value - rand() % accuracy)*(light + rand() % (100 - light + 1)*0.5);
			}
			if (accuracy > 0)
			{
				auto* item = new Effect();
				item->m_interaction_message_type = interaction_message_type_e::single;
				item->m_subtype = effect_e::value;
				item->m_value = -accuracy*0.0000001*sb*0.01*wd*str->m_value;
				//???Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("Урон: " + std::to_string(item->m_value)));
				Interaction_copyist* item1 = new Interaction_copyist();
				item1->m_interaction_message_type = interaction_message_type_e::single;
				item1->m_subtype = interaction_e::health;
				item1->m_value = item;
				item1->apply_effect(p[1].m_object, nullptr);
				ms->m_basic_value += 1;
				ms->update();
				auto reaction = p[0].m_object->get_effect(interaction_e::total_damage);
				if (reaction)
				{
					Object_interaction* msg = reaction->clone();
					msg->apply_effect(p[1].m_object, nullptr);
				}
				if (p[2].m_part->m_item)
				{
					reaction = p[2].m_part->m_item->get_effect(interaction_e::damage);
					if (reaction)
					{
						Object_interaction* msg = reaction->clone();
						msg->apply_effect(p[1].m_object, nullptr);
					}
				}
			}
		}
		else
		{
			msg += p[1].m_object->m_name + u" уклонился.";
		}
		p[1].m_object->update_interaction();
		p[1].m_object->event_update(VoidEventArgs());
		if (sbj_health->m_value - sbj_health_old_value != 0)
		{
			msg += u"Здоровье " + p[1].m_object->m_name + u" изменилось на " + Parser::CP1251_to_UTF16(std::to_string(sbj_health->m_value - sbj_health_old_value)) + u".";
		}
		if(p[1].m_object->m_active_state->m_state==object_state_e::dead)
		{
			msg += p[1].m_object->m_name + u" повержен.";
		}
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_battle, msg));
	}
	else return 1;
	return 0;
}

std::u16string Action_hit::get_description(Parameter* parameter)
{
	std::u16string s(u"Ударить");
	if (parameter)
	{
		Parameter& p(*parameter);
		if (p[1])
		{
			s += u" " + p[1].m_object->m_name;
		}
		if (p[2])
		{
			s += u" " + p[2].m_part->m_name;
		}
	}
	return s;
}

Action_equip::Action_equip()
{
	m_kind = action_e::equip;
	m_icon = Application::instance().m_graph->m_actions[9];
	m_parameter_kind = parameter_type_e::object;
}

void Action_equip::interaction_handler(Parameter* parameter)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* out_parameter;
	if (parameter)
	{
		out_parameter = parameter->clone();
	}
	else
	{
		out_parameter = new Parameter(m_parameter_kind);
	}
	Parameter& p(*out_parameter);
	GameObject* result = Application::instance().command_select_object_on_map();
	if (result)
	{
		p[0].m_object = result;
		std::u16string a = u"Выбран ";
		a.append(p[0].m_object->m_name);
		a = a + u".";
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
	}
	else
	{
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	Application::instance().command_open_body(p[0].m_object);
	Application::instance().m_message_queue.m_busy = false;
}

bool Action_equip::check(Parameter* parameter)
{
	m_error = u"";
	return true;
}


char Action_equip::perfom(Parameter* parameter)
{
	return 0;
}

std::u16string Action_equip::get_description(Parameter* parameter)
{
	Parameter& p(*parameter);
	std::u16string s(u"Экипировка");
	if (parameter)
	{
		Parameter& p(*parameter);
		if (p[0])
		{
			s += u" " + p[0].m_object->m_name;
		}
	}
	return s;
}

Action_show_parameters::Action_show_parameters()
{
	m_kind = action_e::show_parameters;
	m_icon = Application::instance().m_graph->m_actions[10];
	m_name = u"Показать характеристики";
	m_parameter_kind = parameter_type_e::object;
}

void Action_show_parameters::interaction_handler(Parameter* parameter)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* out_parameter;
	if (parameter)
	{
		out_parameter = parameter->clone();
	}
	else
	{
		out_parameter = new Parameter(m_parameter_kind);
	}
	Parameter& p(*out_parameter);
	if (!p[0])
	{
		GameObject* result = Application::instance().command_select_object_on_map();
		if (result)
		{
			p[0].set(result);
			std::u16string a = u"Выбран ";
			a.append(p[0].m_object->m_name);
			a = a + u".";
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			return;
		}
	}
	Application::instance().command_gui_show_characterization(p[0].m_object);
	Application::instance().m_message_queue.m_busy = false;
}

std::u16string Action_show_parameters::get_description(Parameter* parameter)
{
	Parameter& p(*parameter);
	std::u16string s(u"Показать характеристики");
	if (parameter)
	{
		Parameter& p(*parameter);
		if (p[0])
		{
			s += u" " + p[0].m_object->m_name;
		}
	}
	return s;
}

Action_use::Action_use()
{
	m_kind = action_e::use;
	m_icon = Application::instance().m_graph->m_actions[11];
	m_name = u"Применить";
	m_decay = 1;
	m_parameter_kind = parameter_type_e::unit_interaction;
}

void Action_use::interaction_handler(Parameter* parameter)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* out_parameter;
	if (parameter)
	{
		out_parameter = parameter->clone();
	}
	else
	{
		out_parameter = new Parameter(m_parameter_kind);
	}
	Parameter& p(*out_parameter);
	if (!p[0])
	{
		GameObject* result = Application::instance().command_select_object();
		if (result)
		{
			p[0].set(result);
			std::u16string a = u"Выбран ";
			a.append(p[0].m_object->m_name);
			a = a + u".";
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			return;
		}
	}
	if (!p[1])
	{
		GameObject* result = Application::instance().command_select_object();
		if (result)
		{
			p[1].set(result);
			std::u16string a = u"Выбран ";
			a.append(p[1].m_object->m_name);
			a = a + u".";
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			return;
		}
	}
	Application::instance().m_action_manager->add(new GameTask(this, out_parameter));
	Application::instance().m_message_queue.m_busy = false;
}


bool Action_use::check(Parameter* parameter)
{
	return true;
}

char Action_use::perfom(Parameter* parameter)
{
	//Parameter& p(*parameter);
	//if (check(parameter))
	//{
	//	//p->m_unit->set_direction(Game_algorithm::turn_to_object(p->m_unit, p->m_object));
	//	auto reaction = p[1].m_object->get_effect(interaction_e::use);
	//	if (reaction)
	//	{
	//		Object_interaction* msg = reaction->clone();
	//		msg->apply_effect(p[0].m_object, nullptr);
	//	}
	//	switch (p[1].m_object->m_owner->m_kind)
	//	{
	//	case entity_e::cell:
	//	{
	//		//static_cast<MapCell*>(p->m_object->m_owner)->m_items.remove(p->m_object);
	//		static_cast<MapCell*>(p[1].m_object->m_owner)->m_map->remove_object(p[1].m_object);
	//		break;
	//	}
	//	case entity_e::inventory_cell:
	//	{
	//		static_cast<Inventory_cell*>(p[1].m_object->m_owner)->m_items = nullptr;
	//		break;
	//	}
	//	case entity_e::body_part:
	//	{
	//		Application::instance().command_unequip(p[0].m_object, static_cast<Object_part*>(p[1].m_object->m_owner), p[1].m_object);
	//		break;
	//	}
	//	}
	//	Application::instance().m_world->m_object_manager.m_items.remove(p[1].m_object);
	//	p[0].m_object->update_interaction();
	//	p[0].m_object->event_update(VoidEventArgs());
	//}
	//else return 1;
	return 0;
}

std::u16string Action_use::get_description(Parameter* parameter)
{
	Parameter& p(*parameter);
	std::u16string s(u"Применить");
	return s;
}

Action_save::Action_save()
{
	m_kind = action_e::save;
	m_icon = Application::instance().m_graph->m_actions[12];
	m_name = u"Сохранить игру";
}

void Action_save::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;

	auto* world = Application::instance().m_world;
	Parser_context pc(*world);
	pc.reset();
	world->reset_serialization_index();
	std::string binary = world->bin_serialize(pc);
	std::ofstream file(FileSystem::instance().m_resource_path + "Saves\\save.bin", std::ios::binary);
	file.write(&binary[0], binary.size());
	file.close();
	Application::instance().m_message_queue.m_busy = false;
}

std::u16string Action_save::get_description(Parameter* parameter)
{
	std::u16string s(u"Сохранить игру");
	return s;
}

Action_load::Action_load()
{
	m_kind = action_e::load;
	m_icon = Application::instance().m_graph->m_actions[15];
	m_name = u"Загрузить игру";
}

void Action_load::interaction_handler(Parameter* arg)
{
	std::mutex update_lock;
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	update_lock.lock();
	Application::instance().stop();
	//Application::instance().m_GUI->remove(Application::instance().m_GUI->MapViewer);
	Application::instance().load_game();
	update_lock.unlock();
	Application::instance().m_message_queue.m_busy = false;
}

std::u16string Action_load::get_description(Parameter* parameter)
{
	std::u16string s(u"Загрузить игру");
	return s;
}

Action_rotate_view::Action_rotate_view()
{
	m_kind = action_e::rotate_view;
	m_icon = Application::instance().m_graph->m_actions[16];
	m_name = u"Повернуть вид";
}

std::u16string Action_rotate_view::get_description(Parameter* parameter)
{
	std::u16string s(u"Повернуть вид");
	return s;
}

void Action_rotate_view::interaction_handler(Parameter* arg)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Application::instance().m_gui_controller.m_GUI->MapViewer->m_rotate += 1;
	if (Application::instance().m_gui_controller.m_GUI->MapViewer->m_rotate > 3) { Application::instance().m_gui_controller.m_GUI->MapViewer->m_rotate = 0; };
	Application::instance().m_message_queue.m_busy = false;
}

Action_autoexplore::Action_autoexplore()
{
	m_kind = action_e::autoexplore;
	m_icon = Application::instance().m_graph->m_actions[13];
	m_name = u"Автоисследование";
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

std::u16string Action_autoexplore::get_description(Parameter* parameter)
{
	return u"Автоисследование";
}

bool Action_autoexplore::get_child(GameTask*& task)
{
	GameObject* object = Application::instance().m_world->m_player->m_object;
	GameMap* map = static_cast<MapCell*>(object->m_owner)->m_map;
	map->m_dijkstra_map->calculate_cost_autoexplore(map, object);
	map->m_dijkstra_map->trace();
	MapCell* c = map->m_dijkstra_map->next(object);
	task = nullptr;
	if (c)
	{ 
		Parameter* p = new Parameter(parameter_type_e::position);
		(*p)[0].set(object);
		(*p)[1].set(c);
		if (Application::instance().m_actions[action_e::move_step]->check(p))
		{
			task = new GameTask(Application::instance().m_actions[action_e::move_step], p);
		}
	}
	return true;
}

Action_shoot::Action_shoot()
{
	m_kind = action_e::shoot;
	m_icon = Application::instance().m_graph->m_actions[14];
	m_decay = 10;
	m_name = u"Выстрелить из оружия";
	m_parameter_kind = parameter_type_e::bow_shoot;
}

//TODO Адаптировать для 3D
bool Action_shoot::check_cell(Parameter* parameter)
{
	/*Parameter& p(*parameter);
	if (!p[0].m_object->is_own(p[4].m_cell))
	{
		MapCell* cell = p[0].m_object->get_center(p[4].m_cell);
		int distance = (std::max)(abs(p[4].m_cell->x - cell->x) + p[0].m_object->m_active_state->m_size.x / 2 - 1, abs(p[4].m_cell->y - cell->y) + p[0].m_object->m_active_state->m_size.y / 2 - 1);
		Parameter_list* range = p[2].m_part->m_item->get_parameter(interaction_e::weapon_range);
		return distance <= range->m_value;
	}*/
	return true;
}

void Action_shoot::interaction_handler(Parameter* parameter)
{
	Action::interaction_handler(nullptr);
	Application::instance().m_message_queue.m_busy = true;
	Parameter* out_parameter;
	if (parameter)
	{
		out_parameter = parameter->clone();
	}
	else
	{
		out_parameter = new Parameter(m_parameter_kind);
	}
	Parameter& p(*out_parameter);
	if (!p[0])
	{
		p[0].set(Application::instance().m_world->m_player->m_object);
	}

	if (!p[2])
	{
		Object_part* result = Application::instance().command_select_body_part();
		if (result)
		{
			p[2].set(result);
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_clipboard.m_item = nullptr;
			return;
		}
	}

	if (!p[3])
	{
		Object_part* result = Application::instance().command_select_body_part();
		if (result)
		{
			p[3].set(result);
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_clipboard.m_item = nullptr;
			return;
		}
	}

	/*do
	{
		result = Application::instance().command_select_body_part();
		if (result)
		{
			p->m_ammo_owner.push_back(static_cast<Object_part*>(static_cast<P_object_owner*>(result)->m_cell));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::string("Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_clipboard.m_items = nullptr;
			return;
		}
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::string("Выбрать еще заряд? [Y/N]")));
	} while (Application::instance().command_agreement());*/
	
	/*if (!p[1])
	{
		Application::instance().m_GUI->MapViewer->m_hints.push_front(new mapviewer_hint_area(Application::instance().m_GUI->MapViewer, p[0].m_object, true));
		GameObject* result = Application::instance().command_select_object_on_map();
		if (result)
		{
			p[1].set(result);
			std::string a = "Выбран ";
			a.append(p[1].m_object->m_name);
			a = a + ".";
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, a));
		}
		else
		{
			Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::string("Действие отменено")));
			Application::instance().m_message_queue.m_busy = false;
			Application::instance().m_GUI->MapViewer->m_hints.pop_front();
			return;
		}
		Application::instance().m_GUI->MapViewer->m_hints.pop_front();
	}*/

	if (!p[4])
	{
		Parameter_list* wr = p[2].m_part->m_item->get_parameter(interaction_e::weapon_range);
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.push_front(new mapviewer_hint_shoot(p[0].m_object, wr->m_value));
		Application::instance().m_gui_controller.m_hints.push_front(new mapviewer_hint_weapon_range( p[0].m_object, wr->m_value));
		Application::instance().m_UI_mutex.unlock();
		bool valid = false;
		while (!valid)
		{
			MapCell* result = Application::instance().command_select_location(nullptr);
			if (result)
			{
				p[4].set(result);
				if (check_cell(out_parameter))
				{
					Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, u"Выбрана клетка {" + Parser::CP1251_to_UTF16(std::to_string(p[4].m_cell->x) + "," + std::to_string(p[4].m_cell->y) + "}:")));
					valid = true;
				}
				else
				{
					Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Слишком большая дистанция")));
				}
			}
			else
			{
				Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, std::u16string(u"Действие отменено")));
				Application::instance().m_message_queue.m_busy = false;
				Application::instance().m_UI_mutex.lock();
				Application::instance().m_gui_controller.m_hints.pop_front();
				Application::instance().m_gui_controller.m_hints.pop_front();
				Application::instance().m_UI_mutex.unlock();
				return;
			}
		}
		Application::instance().m_UI_mutex.lock();
		Application::instance().m_gui_controller.m_hints.pop_front();
		Application::instance().m_gui_controller.m_hints.pop_front();
		Application::instance().m_UI_mutex.unlock();
	}
	Application::instance().m_action_manager->add(new GameTask(this, out_parameter));
	Application::instance().m_message_queue.m_busy = false;
}

bool Action_shoot::check(Parameter* parameter)
{
	//m_error = "";
	//P_bow_shoot* p = static_cast<P_bow_shoot*>(parameter);
	//if (!p->m_ammo_owner.front()->m_items)
	//{
	//	m_error = "Нет снарядов.";
	//	return false;
	//}
	///*P_unit_interaction* p = static_cast<P_unit_interaction*>(parameter);
	//MapCell* cell;
	//if (!Game_algorithm::check_distance(p->m_unit->cell(), p->m_unit->m_active_state->m_size, p->m_object->cell(), p->m_object->m_active_state->m_size))
	//{
	//	m_error = "Вы слишком далеко от " + p->m_object->m_name;
	//	return false;
	//};*/
	return true;
}

bool Action_shoot::process_cell(Parameter* parameter, MapCell* cell)
{
	Parameter& p(*parameter);
	for (auto c_object = cell->m_items.begin(); c_object != cell->m_items.end(); ++c_object)
	{
		if (!p[0].m_object->is_own(cell))
		{
			if (!(*c_object)->get_tag(object_tag_e::pass_able))
			{
				return true;
			}
		}
	}
	return false;
}

char Action_shoot::perfom(Parameter* parameter)
{
	Parameter& p(*parameter);
	if (check(parameter))
	{
	/*	MapCell* center= p[0].m_object->get_center(p[4].m_cell);
		MapCell* c= p[4].m_cell->m_map->bresenham_line2(center, p[4].m_cell,parameter, std::bind(&Action_shoot::process_cell, this, std::placeholders::_1, std::placeholders::_2));
		GameObject* temp = p[3].m_part->m_items;
		Application::instance().command_unequip(p[0].m_object, p[3].m_part, temp);
		c->m_map->add_object(temp, c);
		temp->m_owner = c;*/

		//switch (p->m_owner->m_kind)
		//{
		//case entity_e::cell:
		//{
		//	//static_cast<MapCell*>(p->m_owner)->m_items.push_back(p->m_object);
		//	static_cast<MapCell*>(p->m_owner)->m_map->add_object(p->m_object, static_cast<MapCell*>(p->m_owner));
		//	p->m_object->m_owner = p->m_owner;
		//	break;
		//}
		//case entity_e::inventory_cell:
		//{
		//	static_cast<Inventory_cell*>(p->m_owner)->m_items = p->m_object;
		//	p->m_object->m_owner = p->m_owner;
		//	break;
		//}
		//case entity_e::body_part:
		//{
		//	Application::instance().command_equip(p->m_unit, static_cast<Object_part*>(p->m_owner), p->m_object);
		//	break;
		//}
		//}


		//GameObject* obj = Application::instance().m_game_object_manager->new_object("arrow");
		//c->m_map->add_to_map(obj, c);
		///*p->m_object->update_interaction();
		//p->m_object->event_update(VoidEventArgs());*/
	}
	else
	{
		Application::instance().m_game_log.add(game_log_message_t(game_log_message_type_e::message_action_interaction, m_error));
		return 1;
	}
	return 0;
}

std::u16string Action_shoot::get_description(Parameter* parameter)
{
	/*Parameter& p(*parameter);
	std::string s(" [");
	s += std::to_string(p[4].m_cell->x) + "," + std::to_string(p[4].m_cell->y) + "]";
	return s;*/

	Parameter& p(*parameter);
	std::u16string s(u"Выстрелить");
	if (parameter)
	{
		Parameter& p(*parameter);
		if (p[2])
		{
			s += u" из " + p[2].m_part->m_item->m_name;
		}
		if (p[3])
		{
			s += u"(" + p[3].m_part->m_item->m_name+u")";
		}
		if (p[4])
		{
			s +=u" в "+ Parser::CP1251_to_UTF16(std::to_string(p[4].m_cell->x)) + u"," + Parser::CP1251_to_UTF16(std::to_string(p[4].m_cell->y));
		}
	}
	return s;
}