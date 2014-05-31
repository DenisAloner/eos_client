#include "GameMap.h"

MapCell::MapCell(int x0, int y0)
{
	x = x0;
	y = y0;
}


void MapCell::add_object(GameObject* Object)
 {
	 m_items.push_back(Object);
 }

GameObjectProperty* MapCell::find_property(PropertyKind kind,GameObject* excluded)
{
	GameObjectProperty* result(nullptr);
	for(std::list<GameObject*>::iterator item=m_items.begin();item!=m_items.end();item++)
	{
		if(excluded!=(*item))
		{
			result=(*item)->find_property(kind);
			if(result!=nullptr)
			{
				return result;
			}
		}
	}
	return nullptr;
}

GameMap::GameMap()
{
	m_size = GSize(MapWidth, MapHeight);
}

void GameMap::add_object(GameObject* Object0,MapCell* Cell0)
{
	Object0->m_cell=Cell0;
	for(int i=0;i<Object0->m_size.y;i++)
	{
		for(int j=0;j<Object0->m_size.x;j++)
		{
			m_items[Cell0->y+i][Cell0->x-j]->add_object(Object0);
		}
	}
}

void GameMap::generate_room(void)
{
	for(int i=0;i<m_size.y;i++)
	{
		for(int j=0;j<m_size.x;j++)
		{
			if ((i==0)||(j==0)||(i==m_size.y-1)||(j==m_size.x-1))
			{
				m_items[i][j]=new MapCell(j,i);
				TWall* w = new TWall();
				int Choise(rand() % 100);
				if (Choise < 50)
				{
					w->m_sprites[ObjectDirection_Left][0] = 15;
					w->m_sprites[ObjectDirection_Left][1] = 15;
					w->m_sprites[ObjectDirection_Left][2] = 15;
					w->m_sprites[ObjectDirection_Left][3] = 15;
				}
				add_object(w,m_items[i][j]);
			} else {
				m_items[i][j]=new MapCell(j,i);
				TFloor* w =new TFloor();
				int Choise(rand() % 100);
				if (Choise < 50)
				{
					w->m_sprites[ObjectDirection_Left][0] = 16;
					w->m_sprites[ObjectDirection_Left][1] = 16;
					w->m_sprites[ObjectDirection_Left][2] = 16;
					w->m_sprites[ObjectDirection_Left][3] = 16;
				}
				add_object(w,m_items[i][j]);
			}
		}
	}
}

void GameMap::move_object(GameObject* Obj,MapCell* Pos)
{
	for (int i = 0; i<Obj->m_size.y; i++)
	{
		for (int j = 0; j<Obj->m_size.x; j++)
		{
			m_items[Obj->m_cell->y + i][Obj->m_cell->x - j]->m_items.remove(Obj);
		}
	}
	add_object(Obj,Pos);
}

void GameMap::add_light(GameObject* Object)
{
	m_lights.push_front(Object);
}

void GameMap::add_new_object(GameObject* Object, MapCell* Element)
{
	add_object(Object, Element);
	add_light(Object);
}

void GameMap::generate_level(void)
{
	srand(time(0));
	GameMapBlock* Block;
	Block = new GameMapBlock;
	Block->coor[0] = 0;
	Block->coor[1] = 0;
	Block->coor[2] = MapWidth - 1;
	Block->coor[3] = MapHeight - 1;
	Block->Left = nullptr;
	Block->Right = nullptr;
	divide_block(Block, 100, 0);
	//Fill(Block);
}

void GameMap::divide_block(GameMapBlock* Block, int Depth, int Current)
{
	int Choise(rand() % 100);
	if (Choise<50)
	{
		int Range = Block->coor[2] - Block->coor[0];
		if (Range<6){ return; }
		Choise = rand() % (Range - 2) + 3;
		if (Range - Choise<3)
		{
			Choise = 3;
			if (Range - Choise - 3<3){ return; }
		}
		Block->Left = new GameMapBlock;
		Block->Left->coor[0] = Block->coor[0];
		Block->Left->coor[1] = Block->coor[1];
		Block->Left->coor[2] = Block->coor[0] + Choise;
		Block->Left->coor[3] = Block->coor[3];
		Block->Right = new GameMapBlock;
		Block->Right->coor[0] = Block->coor[0] + Choise;
		Block->Right->coor[1] = Block->coor[1];
		Block->Right->coor[2] = Block->coor[2];
		Block->Right->coor[3] = Block->coor[3];
		Block->Left->Left = nullptr;
		Block->Left->Right = nullptr;
		Block->Right->Left = nullptr;
		Block->Right->Right = nullptr;
		if (Current<Depth)
		{
			divide_block(Block->Left, Depth, Current + 1);
			divide_block(Block->Right, Depth, Current + 1);
		}
	}
	else {
		int Range = Block->coor[3] - Block->coor[1];
		if (Range<6){ return; }
		Choise = rand() % (Range - 2) + 3;
		if (Range - Choise<3)
		{
			Choise = 3;
			if (Range - Choise - 3<3){ return; }
		}
		Block->Left = new GameMapBlock;
		Block->Left->coor[0] = Block->coor[0];
		Block->Left->coor[1] = Block->coor[1];
		Block->Left->coor[2] = Block->coor[2];
		Block->Left->coor[3] = Block->coor[1] + Choise;
		Block->Right = new GameMapBlock;
		Block->Right->coor[0] = Block->coor[0];
		Block->Right->coor[1] = Block->coor[1] + Choise;
		Block->Right->coor[2] = Block->coor[2];
		Block->Right->coor[3] = Block->coor[3];
		Block->Left->Left = nullptr;
		Block->Left->Right = nullptr;
		Block->Right->Left = nullptr;
		Block->Right->Right = nullptr;
		if (Current<Depth)
		{
			divide_block(Block->Left, Depth, Current + 1);
			divide_block(Block->Right, Depth, Current + 1);
		}
	}
}