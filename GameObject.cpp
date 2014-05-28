#include "GameObject.h"

GameObject::GameObject(Application* app = nullptr)
{
	direction = ObjectDirection_Left;
}

//bool GameObject::ContainAction(TAction* Action)
//{
//	return std::find(Actions.begin(), Actions.end(), 1)==Actions.end();
//}

GameObjectProperty* GameObject::FindProperty(PropertyKind kind)
{
	for(std::list<GameObjectProperty*>::iterator Current=Properties.begin();Current!=Properties.end();Current++)
	{
		if((*Current)->kind==kind)
		{
			return (*Current);
		}
	}
	return nullptr;
}

TAction* GameObject::FindAction(ActionKind kind)
{
	for(std::list<TAction*>::iterator Current=Actions.begin();Current!=Actions.end();Current++)
	{
		if((*Current)->kind==kind)
		{
			return (*Current);
		}
	}
	return nullptr;
}

void GameObject::Turn()
{
	if(direction == ObjectDirection_Left)
	{
		direction = ObjectDirection_Right;
	} else {
		direction = ObjectDirection_Left;
	}
}

TPlayer::TPlayer(Application* app)
{
	Cell=nullptr;
	Sprites[ObjectDirection_Left][0] = 3;
	Sprites[ObjectDirection_Left][1] = 4;
	Sprites[ObjectDirection_Left][2] = 5;
	Sprites[ObjectDirection_Left][3] = 6;
	Sprites[ObjectDirection_Right][0] = 10;
	Sprites[ObjectDirection_Right][1] = 11;
	Sprites[ObjectDirection_Right][2] = 12;
	Sprites[ObjectDirection_Right][3] = 13;
	Width=2;
	Height=6;
	Area.x=2;
	Area.y=1;
	Light=nullptr;
	Actions.push_back(app->Actions[ActionKind_Move]);
	Properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	Properties.push_back(new Property_Container(2,4,"Мешок"));
	Name = "Игрок";
	selected = false;
}

TOrc::TOrc(Application* app)
{
	Cell=nullptr;
	Sprites[ObjectDirection_Left][0] = 7;
	Sprites[ObjectDirection_Left][1] = 7;
	Sprites[ObjectDirection_Left][2] = 7;
	Sprites[ObjectDirection_Left][3] = 7;
	Width=3;
	Height=6;
	Area.x=3;
	Area.y=2;
	Light=nullptr;
	Properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	Name = "Враг";
	selected = false;
}

Elf::Elf(Application* app)
{
	Cell = nullptr;
	Sprites[ObjectDirection_Left][0] =14;
	Sprites[ObjectDirection_Left][1] = 14;
	Sprites[ObjectDirection_Left][2] = 14;
	Sprites[ObjectDirection_Left][3] = 14;
	Width = 2;
	Height =6;
	Area.x = 2;
	Area.y = 2;
	Light = nullptr;
	Properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	Name = "Эльф";
	selected = false;
}

TFloor::TFloor(Application* app)
{
	Cell=nullptr;
	Sprites[ObjectDirection_Left][0] = 0;
	Sprites[ObjectDirection_Left][1] = 0;
	Sprites[ObjectDirection_Left][2] = 0;
	Sprites[ObjectDirection_Left][3] = 0;
	Width=1;
	Height=1;
	Light=nullptr;
	Area.x=1;
	Area.y=1;
	Actions.push_back(app->Actions[ActionKind_Move]);
	Name = "Пол";
	selected = false;
}

TBox::TBox(Application* app)
{
	Cell=nullptr;
	Sprites[ObjectDirection_Left][0] = 9;
	Sprites[ObjectDirection_Left][1] = 9;
	Sprites[ObjectDirection_Left][2] = 9;
	Sprites[ObjectDirection_Left][3] = 9;
	Width=2;
	Height=3;
	Light=nullptr;
	Area.x=2;
	Area.y=2;
	Actions.push_back(app->Actions[ActionKind_Push]);
	Properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	Properties.push_back(new Property_Container(5, 5, "Внутри"));
	Name = "Сундук";
	selected = false;
}

TWall::TWall(Application* app)
{
	Cell=nullptr;
	Sprites[ObjectDirection_Left][0] = 1;
	Sprites[ObjectDirection_Left][1] = 1;
	Sprites[ObjectDirection_Left][2] = 1;
	Sprites[ObjectDirection_Left][3] = 1;
	Width=1;
	Height=5;
	Light=nullptr;
	Area.x=1;
	Area.y=1;
	Properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	Name = "Стена";
	selected = false;
}

TTorch::TTorch(Application* app)
{
	Cell=nullptr;
	Sprites[ObjectDirection_Left][0] = 2;
	Sprites[ObjectDirection_Left][1] = 2;
	Sprites[ObjectDirection_Left][2] = 2;
	Sprites[ObjectDirection_Left][3] = 2;
	Width=1;
	Height=4;
	Area.x=1;
	Area.y=1;
	Light=new TLight;
	Light->Power=2;
	Light->RGB[0]=1;
	Light->RGB[1]=1;
	Light->RGB[2]=1;
	Properties.push_back(new GameObjectProperty(PropertyKind_Impassable));
	Name = "Факел";
	selected = false;
}