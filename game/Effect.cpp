#include "Effect.h"
#include "Application.h"
#include "game\graphics\GUI_TextBox.h"

//class Application;

Object_interaction::Object_interaction()
{
}

Interaction_slot::Interaction_slot()
{
}

void Object_interaction::do_predicat(predicat func){ func(this); };

void Object_interaction::apply_effect(GameObject* unit, Object_interaction* object)
{
}

bool Interaction_slot::on_turn()
{
	return m_value->on_turn();
}

void Interaction_slot::do_predicat(predicat func)
{
	func(this);
	m_value->do_predicat(func);
}

std::string Interaction_copyist::get_description()
{
	return "slot";
}

Object_interaction* Interaction_copyist::clone()
{
	Interaction_copyist* effect = new Interaction_copyist();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

Effect::Effect()
{
}

bool Effect::on_turn()
{
	return false;
}

Object_interaction* Effect::clone()
{
	Effect* effect = new Effect();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value;
	return effect;
}

std::string Effect::get_description()
{
	return Application::instance().m_game_object_manager->get_effect_subtype_string(m_subtype) + ":" + std::to_string(m_value);
}

void Effect::apply_effect(GameObject* unit, Object_interaction* object)
{
	switch (m_subtype)
	{
	case effect_e::value:
	{
		Parameter_list* item = static_cast<Parameter_list*>(object);
		item->m_basic_value = item->m_basic_value + m_value;
		if (item->m_basic_value <=1)
		{ 
			item->m_basic_value = 0;
			Interaction_prefix_ex* prefix = new Interaction_prefix_ex();
			prefix->m_value = object;
			prefix->m_subtype = effect_prefix_e::parameter_change;
			prefix->m_effect = clone();
			prefix->apply_effect(unit, object);
		}
		break;
	}
	case effect_e::limit:
	{
		Parameter_list* item = static_cast<Parameter_list*>(object);
		item->m_basic_limit = item->m_basic_limit + m_value;
		break;
	}
	}
}

Interaction_list::Interaction_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
}

bool Interaction_list::on_turn()
{
	bool result = false;
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		if ((*item)->on_turn())
		{
			result = true;
		}
	}
	return result;
}

std::string Interaction_list::get_description()
{
	std::string result = "";
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		result += (*current)->get_description() + ",";
	}
	return result;
}

void Interaction_list::update()
{
}

Interaction_list* Interaction_list::clone()
{
	Interaction_list* result = new Interaction_list();
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->m_effect.push_back((*item)->clone());
	}
	return result;
}

Tag_list::Tag_list()
{
	m_interaction_message_type = interaction_message_type_e::list;
}

Tag_list* Tag_list::clone()
{
	Tag_list* result = new Tag_list();
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->m_effect.push_back((*item)->clone());
	}
	return result;
}

Parameter_list::Parameter_list(interaction_e subtype) :m_subtype(subtype){};

void Parameter_list::update_list(Interaction_list* list)
{
	Effect* item;
	Interaction_list* list_item;
	for (auto current = list->m_effect.begin(); current != list->m_effect.end(); current++)
	{
		switch ((*current)->m_interaction_message_type)
		{
		case interaction_message_type_e::list:
		{
			update_list(static_cast<Interaction_list*>(*current));
			break;
		}
		default:
		{
			item = static_cast<Effect*>(*current);
			switch (item->m_subtype)
			{
			case effect_e::value:
			{
				m_value = m_value + item->m_value;
				break;
			}
			case effect_e::limit:
			{
				m_limit = m_limit + item->m_value;
				break;
			}
			}
		}
		}
	}
}

void Parameter_list::update()
{
	m_value = m_basic_value;
	m_limit = m_basic_limit;
	update_list(this);
}

void Interaction_list::apply_effect(GameObject* unit, Object_interaction* object)
{
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->apply_effect(unit,object);
	}
}

void Interaction_list::do_predicat(predicat func)
{
	func(this);
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		(*item)->do_predicat(func);
	}
}

std::string Parameter_list::get_description()
{
	std::string result = std::to_string(m_value) + "/" + std::to_string(m_limit) + ",";
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		result += (*current)->get_description() + ",";
	}
	return result;
}

void Parameter_list::add(Object_interaction* item)
{
}

Parameter_list* Parameter_list::clone()
{
	Parameter_list* result = new Parameter_list(m_subtype);
	result->m_basic_limit = m_basic_limit;
	result->m_basic_value = m_basic_value;
	result->m_value = m_value;
	result->m_limit = m_limit;
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->add((*item)->clone());
	}
	return result;
}

//void Interaction_list::add(Object_interaction* item)
//{
//	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
//	{
//		(*item)->apply(object);
//	}
//}


void Effect::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level,'.')+Application::instance().m_game_object_manager->get_effect_subtype_string(m_subtype) + ":" + std::to_string(m_value));
}

void Interaction_copyist::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<тип параметра:" + Application::instance().m_game_object_manager->get_effect_string(m_subtype) + ">:");
	m_value->description(info,level+1);
}

void Interaction_list::description(std::list<std::string>* info, int level)
{
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		(*current)->description(info,level);
	}
}

void Parameter_list::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + std::to_string(m_value) + "(" + std::to_string(m_basic_value) + ")/" + std::to_string(m_limit) + "(" + std::to_string(m_basic_limit) + "):");
	for (auto current = m_effect.begin(); current != m_effect.end(); current++)
	{
		(*current)->description(info,level);
	}
}

Interaction_timer::Interaction_timer()
{

}

bool Interaction_timer::on_turn()
{
	m_turn += 1;
	if (m_turn > m_period)
	{
		m_turn = 0;
	}
	return m_value->on_turn();
}

std::string Interaction_timer::get_description()
{
	return "slot";
}

Object_interaction* Interaction_timer::clone()
{
	Interaction_timer* effect = new Interaction_timer();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_turn = m_turn;
	effect->m_period = m_period;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_timer::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<цикличное наложение эффекта:" + std::to_string(m_turn) + "(" + std::to_string(m_period) + ")>:");
	m_value->description(info, level + 1);
}

void Interaction_copyist::apply_effect(GameObject* unit, Object_interaction* object)
{
	auto i = unit->get_effect(m_subtype);
	if (i)
	{
		m_value->apply_effect(unit, i);
	}
}

void Interaction_timer::apply_effect(GameObject* unit, Object_interaction* object)
{
	if (m_turn == m_period)
	{
		m_value->apply_effect(unit,object);
	}
}

Interaction_copyist::Interaction_copyist()
{
}

Interaction_time::Interaction_time()
{

}

bool Interaction_time::on_turn()
{
	m_turn -= 1;
	return m_turn == 0;
}

std::string Interaction_time::get_description()
{
	return "slot";
}

Object_interaction* Interaction_time::clone()
{
	Interaction_time* effect = new Interaction_time();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_turn = m_turn;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_time::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<длительное наложение эффекта:" + std::to_string(m_turn) + ">:");
	m_value->description(info, level + 1);
}

void Interaction_time::apply_effect(GameObject* unit, Object_interaction* object)
{
	m_value->apply_effect(unit,object);
}

std::string Interaction_prefix::get_description()
{
	return "none";
}

Interaction_prefix::Interaction_prefix(){};

void Interaction_prefix::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<тип эффекта:" + Application::instance().m_game_object_manager->get_effect_prefix_string(m_subtype) + ">:");
	m_value->description(info, level + 1);
}

Interaction_prefix* Interaction_prefix::clone()
{
	Interaction_prefix* effect = new Interaction_prefix();
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

Interaction_prefix_ex* Interaction_prefix_ex::clone()
{
	Interaction_prefix_ex* effect = new Interaction_prefix_ex();
	effect->m_subtype = m_subtype;
	effect->m_value = m_value;
	effect->m_effect = m_effect;
	return effect;
}

void Interaction_prefix::apply_effect(GameObject* unit, Object_interaction* object)
{
	Tag_list* i = static_cast<Tag_list*>(unit->get_effect(interaction_e::tag));
	if (i)
	{
		for (auto item = i->m_effect.begin(); item != i->m_effect.end(); item++)
		{
			(*item)->apply_effect(unit, this);
		}
	}
	if (m_value){ m_value->apply_effect(unit, object); }
}

Interaction_addon::Interaction_addon(){};

std::string Interaction_addon::get_description()
{
	return "slot";
}

Object_interaction* Interaction_addon::clone()
{
	Interaction_addon* effect = new Interaction_addon();
	effect->m_interaction_message_type = m_interaction_message_type;
	effect->m_subtype = m_subtype;
	effect->m_value = m_value->clone();
	return effect;
}

void Interaction_addon::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<наложение эффекта:" + Application::instance().m_game_object_manager->get_effect_string(m_subtype) + ">:");
	m_value->description(info, level + 1);
}

void Interaction_addon::apply_effect(GameObject* unit, Object_interaction* object)
{
unit->add_effect(m_subtype, m_value->clone());
}

Object_tag::Object_tag(object_tag_e key) :m_type(key){};

void Object_tag::description(std::list<std::string>* info, int level)
{
	info->push_back(std::string(level, '.') + "<" + Application::instance().m_game_object_manager->get_object_tag_string(m_type) + ">");
}

std::string Object_tag::get_description()
{
	return "none";
}

bool Object_tag::on_turn()
{
	return false;
}

ObjectTag::Poison_resist::Poison_resist() :Object_tag(object_tag_e::poison_resist){};

ObjectTag::Poison_resist* ObjectTag::Poison_resist::clone()
{
	ObjectTag::Poison_resist* effect = new ObjectTag::Poison_resist();
	return effect;
}

void ObjectTag::Poison_resist::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::poison_damage:
	{
		static_cast<Effect*>(prefix->m_value)->m_value /= 2;
		break;
	}
	}
}

ObjectTag::Mortal::Mortal() :Object_tag(object_tag_e::mortal){};

ObjectTag::Mortal* ObjectTag::Mortal::clone()
{
	ObjectTag::Mortal* effect = new ObjectTag::Mortal();
	return effect;
}

void ObjectTag::Mortal::apply_effect(GameObject* unit, Object_interaction* object)
{
	/*Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::parameter_change:
	{
		Interaction_prefix_ex* prefix_ex = static_cast<Interaction_prefix_ex*>(object);
		if (static_cast<Parameter_list*>(prefix_ex->m_value)->m_subtype == interaction_e::health)
		{
			if (static_cast<Parameter_list*>(prefix_ex->m_value)->m_basic_value == 0)
			{
				unit->set_state(object_state_e::dead);
			}
		}
		break;
	}
	}*/
}

ObjectTag::Purification_from_poison::Purification_from_poison() :Object_tag(object_tag_e::purification_from_poison){};

ObjectTag::Purification_from_poison* ObjectTag::Purification_from_poison::clone()
{
	ObjectTag::Purification_from_poison* effect = new ObjectTag::Purification_from_poison();
	return effect;
}

void ObjectTag::Purification_from_poison::apply_effect(GameObject* unit, Object_interaction* object)
{
	Interaction_prefix* prefix = static_cast<Interaction_prefix*>(object);
	switch (prefix->m_subtype)
	{
	case effect_prefix_e::poison_damage:
	{
		Parameter_list* p = unit->get_parameter(interaction_e::poison);
			if (p)
			{
				Effect* e = static_cast<Effect*>(prefix->m_value);
				p->m_basic_value -= e->m_value;
				if (p->m_basic_value > p->m_basic_limit)
				{
					e->m_value = p->m_basic_limit - p->m_basic_value;
					p->m_basic_value = p->m_basic_limit;
				}
				else
				{
					e->m_value = 0;
				}
			}
		break;
	}
	}
}

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
							if (!(*item)->get_stat(object_attribute_e::pass_able))
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
							if (!(*item)->get_stat(object_attribute_e::pass_able))
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
			static_cast<MapCell*>(p->m_object->m_owner)->m_items.remove(p->m_object);
			break;
		}
		case entity_e::inventory_cell:
		{
			static_cast<Inventory_cell*>(p->m_object->m_owner)->m_item = nullptr;
			break;
		}
		case entity_e::body_part:
		{
			Application::instance().command_unequip(p->m_unit, static_cast<Body_part*>(p->m_object->m_owner), p->m_object);
			break;
		}
		}
		switch (p->m_owner->m_kind)
		{
		case entity_e::cell:
		{
			static_cast<MapCell*>(p->m_owner)->m_items.push_back(p->m_object);
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
			Application::instance().command_equip(p->m_unit, static_cast<Body_part*>(p->m_owner), p->m_object);
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
		p->m_object->set_state(object_state_e::off);
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
		p->m_unit_body_part = static_cast<Body_part*>(static_cast<P_object_owner*>(result)->m_cell);
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
	if (check(p) && p->m_unit_body_part)
	{
		auto reaction = p->m_unit->get_effect(interaction_e::total_damage);
		if (reaction)
		{
			Object_interaction* msg = reaction->clone();
			msg->apply_effect(p->m_object, nullptr);
		}
		reaction = p->m_unit_body_part->m_item->get_effect(interaction_e::damage);
		if (reaction)
		{
			Object_interaction* msg = reaction->clone();
			msg->apply_effect(p->m_object, nullptr);
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

Action_list::Action_list(){};

Interaction_list* Action_list::clone()
{
	Action_list* result = new Action_list();
	for (auto item = m_effect.begin(); item != m_effect.end(); item++)
	{
		result->m_effect.push_back((*item)->clone());
	}
	return result;
}