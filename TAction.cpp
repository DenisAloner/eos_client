#include "TAction.h"

TAction::TAction(void)
{
	m_kind = ActionKind_Move;
	m_name="���";
}


TAction::~TAction(void)
{
}

bool TAction::check(TParameter* parameter)
{
	return true;
}

void TAction::perfom(TParameter* parameter)
{
}

void TAction::interaction_handler()
{
	/*Game->PlaySound1();*/
	if (Application::instance().m_message_queue.m_busy)
	{
		if (Application::instance().m_message_queue.m_reader)
		{
			TParameter* p = new TParameter(ParameterKind_Cancel);
			Application::instance().m_message_queue.push(p);
		}
		while (Application::instance().m_message_queue.m_busy){}
	}
}

ActionClass_Move::ActionClass_Move()
{
	m_kind = ActionKind_Move;
	m_name = "����";
}


ActionClass_Move::~ActionClass_Move()
{
}

void ActionClass_Move::interaction_handler()
{
	TAction::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter_Position* p = new Parameter_Position();
	p->m_object = Application::instance().m_GUI->MapViewer->m_player;
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	if (Application::instance().command_select_location(p->m_object, p->m_place))
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("������� ������ {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("�������� ��������."));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}


bool ActionClass_Move::check(TParameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	//GameObjectProperty* result(nullptr);
	//for (int i = 0; i<p->object->Area.y; i++)
	//{
	//	for (int j = 0; j<p->object->Area.x; j++)
	//	{
	//		if (p->map->Items[p->place->y + i][p->place->x - j] == nullptr){ return false; }
	//		if (p->map->Items[p->place->y + i][p->place->x - j]->FindProperty(PropertyKind_Impassable, p->object) != nullptr)
	//		{
	//			return false;
	//		}
	//	}
	//}
	//return true;
	return Application::instance().command_check_position(p->m_object,p->m_place,p->m_map);
}

void ActionClass_Move::perfom(TParameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	if (check(p))
	{
		p->m_map->move_object(p->m_object, p->m_place);
	}
}

char const* ActionClass_Move::get_description(TParameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	std::string s("���� � X:");
	s += std::to_string(p->m_place->x);
	s += ",Y:";
	s += std::to_string(p->m_place->y);
	char* cstr = new char[s.length() + 1];
	std::strcpy(cstr, s.c_str());
	return cstr;
}

ActionClass_Push::ActionClass_Push(void)
{
	m_kind = ActionKind_Push;
	m_name = "�������";
}


ActionClass_Push::~ActionClass_Push(void)
{
}

char const* ActionClass_Push::get_description(TParameter* parameter)
{
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	std::string s("����������� ");
	s += p->m_object->m_name;
	s += " � X:"+std::to_string(p->m_place->x);
	s += ",Y:";
	s += std::to_string(p->m_place->y);
	char* cstr = new char[s.length() + 1];
	std::strcpy(cstr, s.c_str());
	return cstr;
}

bool ActionClass_Push::check(TParameter* parameter)
{
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	GameObjectProperty* result(nullptr);
	for (int i = 0; i<p->m_object->m_size.y; i++)
	{
		for (int j = 0; j<p->m_object->m_size.x; j++)
		{
			if (p->m_map->m_items[p->m_place->y + i][p->m_place->x - j] == nullptr){ return false; }
			if (p->m_map->m_items[p->m_place->y + i][p->m_place->x - j]->find_property(PropertyKind_Impassable, p->m_object) != nullptr)
			{
				return false;
			}
		}
	}
	return true;
}

void ActionClass_Push::perfom(TParameter* parameter)
{
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	if (check(p))
	{
		p->m_map->move_object(p->m_object, p->m_place);
	}
}

void ActionClass_Push::interaction_handler()
{
	TAction::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter_MoveObjectByUnit* p = new Parameter_MoveObjectByUnit();
	p->m_unit = Application::instance().m_GUI->MapViewer->m_player;
	p->m_map = Application::instance().m_GUI->MapViewer->m_map;
	if (Application::instance().command_select_object(p->m_object))
	{
		std::string a = "������ "; 
		a.append(p->m_object->m_name);
		a = a + ".";
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(a));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("�������� ��������."));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	if (Application::instance().command_select_location(p->m_object, p->m_place))
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("������� ������ {" + std::to_string(p->m_place->x) + "," + std::to_string(p->m_place->y) + "}."));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("�������� ��������."));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}

ActionClass_Turn::ActionClass_Turn(void)
{
	m_kind = ActionKind_Turn;
	m_name = "�����������";
}


ActionClass_Turn::~ActionClass_Turn(void)
{
}

char const* ActionClass_Turn::get_description(TParameter* parameter)
{
	Parameter_GameObject* p = static_cast<Parameter_GameObject*>(parameter);
	std::string s("�����������");
	char* cstr = new char[s.length() + 1];
	std::strcpy(cstr, s.c_str());
	return cstr;
}

bool ActionClass_Turn::check(TParameter* parameter)
{
	Parameter_GameObject* p = static_cast<Parameter_GameObject*>(parameter);
	return true;
}

void ActionClass_Turn::perfom(TParameter* parameter)
{
	Parameter_GameObject* p = static_cast<Parameter_GameObject*>(parameter);
	if (check(p))
	{
		p->m_object->Turn();
	}
}

void ActionClass_Turn::interaction_handler()
{
	TAction::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("�� ����������������."));
	Parameter_GameObject* p = new Parameter_GameObject();
	p->m_object = Application::instance().m_GUI->MapViewer->m_player;
	Application::instance().m_action_manager->add(new GameTask(this, p));
	Application::instance().m_message_queue.m_busy = false;
}

Action_OpenInventory::Action_OpenInventory()
{
	m_kind = ActionKind_OpenInventory;
}


Action_OpenInventory::~Action_OpenInventory()
{
}

void Action_OpenInventory::interaction_handler()
{
	TAction::interaction_handler();
	Application::instance().m_message_queue.m_busy = true;
	Parameter_GameObject* p = new Parameter_GameObject();
	if (Application::instance().command_select_object(p->m_object))
	{
		std::string a = "������ ";
		a.append(p->m_object->m_name);
		a = a + ".";
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text(a));
	}
	else
	{
		Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("�������� ��������."));
		Application::instance().m_message_queue.m_busy = false;
		return;
	}
	//Game->ActionManager->Add(new GameTask(this, p));
	Application::instance().command_open_inventory(p->m_object);
	Application::instance().m_message_queue.m_busy = false;
}


bool Action_OpenInventory::check(TParameter* parameter)
{
	return true;
}

void Action_OpenInventory::perfom(TParameter* parameter)
{

}

char const* Action_OpenInventory::get_description(TParameter* parameter)
{
	return "";
}