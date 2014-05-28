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
#include "GameEntity.h"


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

class MapCell : public GameEntity
{

public:

	int x; 
	int y;

	std::list<GameObject*> Objects;

	MapCell(int x0, int y0);

	void AddObject(GameObject* Object);
	virtual GameObjectProperty* FindProperty(PropertyKind kind,GameObject* excluded);

};

class GameMap: public GameEntity
{
public:

	Application* app;

    int Width;
    int Height;

	MapCell* Items[MapHeight][MapWidth];
	std::list<GameObject*> Light;

	GameMap(Application* C = nullptr)
	{
		app=C;
		Width=MapWidth;
		Height=MapHeight;
	}

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

	
	void AddObject(GameObject* Object0,MapCell* Cell0);

	void GenerateRoom(void);

	void DivideBlock(GameMapBlock* Block,int Depth,int Current)
	{
		int Choise(rand()%100);
		if(Choise<50)
		{
			int Range=Block->coor[2]-Block->coor[0];
			if(Range<6){return;}
			Choise=rand()%(Range-2)+3;
			if(Range-Choise<3)
			{
				Choise=3;
				if(Range-Choise-3<3){return;}
			} 
			Block->Left=new GameMapBlock;
			Block->Left->coor[0]=Block->coor[0];
			Block->Left->coor[1]=Block->coor[1];
			Block->Left->coor[2]=Block->coor[0]+Choise;
			Block->Left->coor[3]=Block->coor[3];
			Block->Right=new GameMapBlock;
			Block->Right->coor[0]=Block->coor[0]+Choise;
			Block->Right->coor[1]=Block->coor[1];
			Block->Right->coor[2]=Block->coor[2];
			Block->Right->coor[3]=Block->coor[3];
			Block->Left->Left=nullptr;
			Block->Left->Right=nullptr;
			Block->Right->Left=nullptr;
			Block->Right->Right=nullptr;
			if(Current<Depth)
			{
				DivideBlock(Block->Left,Depth,Current+1);
				DivideBlock(Block->Right,Depth,Current+1);
			}
		} else {
			int Range=Block->coor[3]-Block->coor[1];
			if(Range<6){return;}
			Choise=rand()%(Range-2)+3;
			if(Range-Choise<3)
			{
				Choise=3;
				if(Range-Choise-3<3){return;}
			} 
			Block->Left=new GameMapBlock;
			Block->Left->coor[0]=Block->coor[0];
			Block->Left->coor[1]=Block->coor[1];
			Block->Left->coor[2]=Block->coor[2];
			Block->Left->coor[3]=Block->coor[1]+Choise;
			Block->Right=new GameMapBlock;
			Block->Right->coor[0]=Block->coor[0];
			Block->Right->coor[1]=Block->coor[1]+Choise;
			Block->Right->coor[2]=Block->coor[2];
			Block->Right->coor[3]=Block->coor[3];
			Block->Left->Left=nullptr;
			Block->Left->Right=nullptr;
			Block->Right->Left=nullptr;
			Block->Right->Right=nullptr;
			if(Current<Depth)
			{
				DivideBlock(Block->Left,Depth,Current+1);
				DivideBlock(Block->Right,Depth,Current+1);
			}
		}
	}

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

	void GenerateLevel(void)
	{
		srand(time(0));
		GameMapBlock* Block;
		Block=new GameMapBlock;
		Block->coor[0]=0;
		Block->coor[1]=0;
		Block->coor[2]=MapWidth-1;
		Block->coor[3]=MapHeight-1;
		Block->Left=nullptr;
	    Block->Right=nullptr;
		DivideBlock(Block,100,0);
		//Fill(Block);
	}

	void MoveObject(GameObject* Obj,MapCell* Pos);
	void AddLight(GameObject* Object);
	void AddNewObject(GameObject* Object, MapCell* Element);

};

