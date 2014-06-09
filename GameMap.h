#pragma once
#include "GraphicalController.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include "GameObject.h"
#include <list>
#include "GameObjectProperty.h"
#include "Application.h"


const int MapWidth=40;
const int MapHeight=60;

class Application;

struct GameMapBlock
{
	int coor[4];
	GameMapBlock* Left;
	GameMapBlock* Right;
};

class GameObject;
class GameObjectProperty;

class MapCell
{

public:

	int x; 
	int y;

	std::list<GameObject*> m_items;

	MapCell(int x0, int y0);

	void add_object(GameObject* Object);
	virtual GameObjectProperty* find_property(PropertyKind kind,GameObject* excluded);

};

class GameMap
{
public:

	dimension_t m_size;

	MapCell* m_items[MapHeight][MapWidth];
	std::list<GameObject*> m_lights;

	GameMap();

	/*void Fill(GameMapBlock* Block)
	{
	if(Block->Left==nullptr&&Block->Right==nullptr)
	{
		int Choise(rand()%100);
		if(Choise<50)
		{
			for(int i=Block->coor[1];i<Block->coor[3]+1;i++)
			{
				for(int j=Block->coor[0];j<Block->coor[2]+1;j++)
				{
					if ((i==Block->coor[1])||(j==Block->coor[0])||(i==Block->coor[3])||(j==Block->coor[2]))
					{
						Items[i][j]=new TWall(j,i);
					} else {
						Items[i][j]=new TFloor(j,i);
					}
				}
			}
		} else {
			for(int i=Block->coor[1];i<Block->coor[3]+1;i++)
			{
				for(int j=Block->coor[0];j<Block->coor[2]+1;j++)
				{
					Items[i][j]=nullptr;
				}
			}
		}
	} else {
		Fill(Block->Left);
		Fill(Block->Right);
	}
	}*/

	//void ListBlock(GameMapBlock* Block)
	//{
	//if(Block->Left==nullptr&&Block->Right==nullptr)
	//{
	//	std::cout<<Block->coor[0]<<"  "<<Block->coor[1]<<"  "<<Block->coor[2]<<"  "<<Block->coor[3]<<"  "<<"\n";
	//} else {
	//	ListBlock(Block->Left);
	//	ListBlock(Block->Right);
	//}
	//}
	

	void add_object(GameObject* Object0, MapCell* Cell0);
	void generate_room(void);
	void divide_block(GameMapBlock* Block, int Depth, int Current);
	void generate_level(void);
	void move_object(GameObject* Obj,MapCell* Pos);
	void add_light(GameObject* Object);
	void add_new_object(GameObject* Object, MapCell* Element);

};

