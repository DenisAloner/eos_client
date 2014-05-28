#include "TAction.h"

TAction::TAction(void)
{
	kind = ActionKind_Move;
	Name="Нету";
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

void TAction::InteractionHandler()
{
	/*Game->PlaySound1();*/
	if (Application::Instance().MessageQueue.Busy)
	{
		if (Application::Instance().MessageQueue.Reader)
		{
			TParameter* p = new TParameter(ParameterKind_Cancel);
			Application::Instance().MessageQueue.Push(p);
		}
		while (Application::Instance().MessageQueue.Busy){}
	}
}

ActionClass_Move::ActionClass_Move()
{
	kind = ActionKind_Move;
	Name = "Идти";
}


ActionClass_Move::~ActionClass_Move()
{
}

void ActionClass_Move::InteractionHandler()
{
	TAction::InteractionHandler();
	Application::Instance().MessageQueue.Busy = true;
	Parameter_Position* p = new Parameter_Position();
	p->object = Application::Instance().GUI->MapViewer->Player;
	p->map = Application::Instance().GUI->MapViewer->Map;
	if (Application::Instance().Command_SelectLocation(p->object, p->place))
	{
		Application::Instance().GUI->DescriptionBox->AddItemControl(new GUI_Text("Выбрана клетка {" + std::to_string(p->place->x) + "," + std::to_string(p->place->y) + "}."));
	}
	else
	{
		Application::Instance().GUI->DescriptionBox->AddItemControl(new GUI_Text("Действие отменено."));
		Application::Instance().MessageQueue.Busy = false;
		return;
	}
	Application::Instance().ActionManager->Add(new GameTask(this, p));
	Application::Instance().MessageQueue.Busy = false;
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
	return Application::Instance().Command_CheckPosition(p->object,p->place,p->map);
}

void ActionClass_Move::perfom(TParameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	if (check(p))
	{
		p->map->MoveObject(p->object, p->place);
	}
}

char const* ActionClass_Move::GetDescription(TParameter* parameter)
{
	Parameter_Position* p = static_cast<Parameter_Position*>(parameter);
	std::string s("Идти в X:");
	s += std::to_string(p->place->x);
	s += ",Y:";
	s += std::to_string(p->place->y);
	char* cstr = new char[s.length() + 1];
	std::strcpy(cstr, s.c_str());
	return cstr;
}

ActionClass_Push::ActionClass_Push(void)
{
	kind = ActionKind_Push;
	Name = "Толкать";
}


ActionClass_Push::~ActionClass_Push(void)
{
}

char const* ActionClass_Push::GetDescription(TParameter* parameter)
{
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	std::string s("Переместить ");
	s += p->object->Name;
	s += " в X:"+std::to_string(p->place->x);
	s += ",Y:";
	s += std::to_string(p->place->y);
	char* cstr = new char[s.length() + 1];
	std::strcpy(cstr, s.c_str());
	return cstr;
}

bool ActionClass_Push::check(TParameter* parameter)
{
	Parameter_MoveObjectByUnit* p = static_cast<Parameter_MoveObjectByUnit*>(parameter);
	GameObjectProperty* result(nullptr);
	for (int i = 0; i<p->object->Area.y; i++)
	{
		for (int j = 0; j<p->object->Area.x; j++)
		{
			if (p->map->Items[p->place->y + i][p->place->x - j] == nullptr){ return false; }
			if (p->map->Items[p->place->y + i][p->place->x - j]->FindProperty(PropertyKind_Impassable, p->object) != nullptr)
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
		p->map->MoveObject(p->object, p->place);
	}
}

void ActionClass_Push::InteractionHandler()
{
	TAction::InteractionHandler();
	Application::Instance().MessageQueue.Busy = true;
	Parameter_MoveObjectByUnit* p = new Parameter_MoveObjectByUnit();
	p->unit = Application::Instance().GUI->MapViewer->Player;
	p->map = Application::Instance().GUI->MapViewer->Map;
	if (Application::Instance().Command_SelectObject(p->object))
	{
		std::string a = "Выбран "; 
		a.append(p->object->Name);
		a = a + ".";
		Application::Instance().GUI->DescriptionBox->AddItemControl(new GUI_Text(a));
	}
	else
	{
		Application::Instance().GUI->DescriptionBox->AddItemControl(new GUI_Text("Действие отменено."));
		Application::Instance().MessageQueue.Busy = false;
		return;
	}
	if (Application::Instance().Command_SelectLocation(p->object, p->place))
	{
		Application::Instance().GUI->DescriptionBox->AddItemControl(new GUI_Text("Выбрана клетка {" + std::to_string(p->place->x) + "," + std::to_string(p->place->y) + "}."));
	}
	else
	{
		Application::Instance().GUI->DescriptionBox->AddItemControl(new GUI_Text("Действие отменено."));
		Application::Instance().MessageQueue.Busy = false;
		return;
	}
	Application::Instance().ActionManager->Add(new GameTask(this, p));
	Application::Instance().MessageQueue.Busy = false;
}

ActionClass_Turn::ActionClass_Turn(void)
{
	kind = ActionKind_Turn;
	Name = "Повернуться";
}


ActionClass_Turn::~ActionClass_Turn(void)
{
}

char const* ActionClass_Turn::GetDescription(TParameter* parameter)
{
	Parameter_GameObject* p = static_cast<Parameter_GameObject*>(parameter);
	std::string s("Повернуться");
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
		p->object->Turn();
	}
}

void ActionClass_Turn::InteractionHandler()
{
	TAction::InteractionHandler();
	Application::Instance().MessageQueue.Busy = true;
	Application::Instance().GUI->DescriptionBox->AddItemControl(new GUI_Text("Вы разворачиваетесь."));
	Parameter_GameObject* p = new Parameter_GameObject();
	p->object = Application::Instance().GUI->MapViewer->Player;
	Application::Instance().ActionManager->Add(new GameTask(this, p));
	Application::Instance().MessageQueue.Busy = false;
}

Action_OpenInventory::Action_OpenInventory()
{
	kind = ActionKind_OpenInventory;
}


Action_OpenInventory::~Action_OpenInventory()
{
}

void Action_OpenInventory::InteractionHandler()
{
	TAction::InteractionHandler();
	Application::Instance().MessageQueue.Busy = true;
	Parameter_GameObject* p = new Parameter_GameObject();
	if (Application::Instance().Command_SelectObject(p->object))
	{
		std::string a = "Выбран ";
		a.append(p->object->Name);
		a = a + ".";
		Application::Instance().GUI->DescriptionBox->AddItemControl(new GUI_Text(a));
	}
	else
	{
		Application::Instance().GUI->DescriptionBox->AddItemControl(new GUI_Text("Действие отменено."));
		Application::Instance().MessageQueue.Busy = false;
		return;
	}
	//Game->ActionManager->Add(new GameTask(this, p));
	Application::Instance().Command_OpenInventory(p->object);
	Application::Instance().MessageQueue.Busy = false;
}


bool Action_OpenInventory::check(TParameter* parameter)
{
	return true;
}

void Action_OpenInventory::perfom(TParameter* parameter)
{

}

char const* Action_OpenInventory::GetDescription(TParameter* parameter)
{
	return "";
}