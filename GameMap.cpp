#include "GameMap.h"

MapCell::MapCell(int x0, int y0)
{
	x = x0;
	y = y0;
}


void MapCell::AddObject(GameObject* Object)
 {
	 Objects.push_back(Object);
 }

GameObjectProperty* MapCell::FindProperty(PropertyKind kind,GameObject* excluded)
{
	GameObjectProperty* result(nullptr);
	for(std::list<GameObject*>::iterator item=Objects.begin();item!=Objects.end();item++)
	{
		if(excluded!=(*item))
		{
			result=(*item)->FindProperty(kind);
			if(result!=nullptr)
			{
				return result;
			}
		}
	}
	return nullptr;
}

void GameMap::AddObject(GameObject* Object0,MapCell* Cell0)
{
	Object0->Cell=Cell0;
	for(int i=0;i<Object0->Size.y;i++)
	{
		for(int j=0;j<Object0->Size.x;j++)
		{
			Items[Cell0->y+i][Cell0->x-j]->AddObject(Object0);
		}
	}
}

void GameMap::GenerateRoom(void)
{
	for(int i=0;i<Height;i++)
	{
		for(int j=0;j<Width;j++)
		{
			if ((i==0)||(j==0)||(i==Height-1)||(j==Width-1))
			{
				Items[i][j]=new MapCell(j,i);
				TWall* w = new TWall(app);
				int Choise(rand() % 100);
				if (Choise < 50)
				{
					w->Sprites[ObjectDirection_Left][0] = 15;
					w->Sprites[ObjectDirection_Left][1] = 15;
					w->Sprites[ObjectDirection_Left][2] = 15;
					w->Sprites[ObjectDirection_Left][3] = 15;
				}
				AddObject(w,Items[i][j]);
			} else {
				Items[i][j]=new MapCell(j,i);
				TFloor* w =new TFloor(app);
				int Choise(rand() % 100);
				if (Choise < 50)
				{
					w->Sprites[ObjectDirection_Left][0] = 16;
					w->Sprites[ObjectDirection_Left][1] = 16;
					w->Sprites[ObjectDirection_Left][2] = 16;
					w->Sprites[ObjectDirection_Left][3] = 16;
				}
				AddObject(w,Items[i][j]);
			}
		}
	}
}

void GameMap::MoveObject(GameObject* Obj,MapCell* Pos)
{
	for(int i=0;i<Obj->Size.y;i++)
	{
		for(int j=0;j<Obj->Size.x;j++)
		{
			Items[Obj->Cell->y+i][Obj->Cell->x-j]->Objects.remove(Obj);
		}
	}
	AddObject(Obj,Pos);
}

void GameMap::AddLight(GameObject* Object)
{
	Light.push_front(Object);
}

void GameMap::AddNewObject(GameObject* Object, MapCell* Element)
{
	AddObject(Object, Element);
	AddLight(Object);
}