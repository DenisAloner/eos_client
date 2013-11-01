#pragma once
#include "TGraph.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include "TMapObject.h"



const int MapWidth=40;
const int MapHeight=60;

struct TMapBlock
{
	int coor[4];
	TMapBlock* Left;
	TMapBlock* Right;
};

class TMapObject;

struct TMapObjectList
{
	TMapObject* Item;
	TMapObjectList* Tail;
};

class TMapElement
{
public:

	int x; 
	int y;
	double Light;

	bool IsSelected;

	TMapObjectList* Objects;

	TMapElement(int x0,int y0)
	{
		x=x0;
		y=y0;
		Objects=nullptr;
		IsSelected=false;
	}

	void AddObjectToCell(TMapObject* Object)
	{
		if(Objects==nullptr)
		{
			Objects=new TMapObjectList;
			Objects->Item=Object;
			Objects->Tail=nullptr;

		} else {
			TMapObjectList* Current=Objects;
			while(Current->Tail!=nullptr)
			{
				Current=Current->Tail;
			}
			Current->Tail=new TMapObjectList;
			Current->Tail->Item=Object;
			Current->Tail->Tail=nullptr;
		}
	}

		bool AccessToMove(TMapObject* Obj)
		{
			TMapObjectList* Current=Objects;
			while(Current!=nullptr)
			{
				if(!Current->Item->AccessToMove(Obj)){return false;}
				Current=Current->Tail;
			}
			return true;
	}
};

class TMap
{
public:

    int Width;
    int Height;

	TMapElement* Items[MapHeight][MapWidth];

	TMapObjectList* Light;

	TMap()
	{
		Width=MapWidth;
		Height=MapHeight;
		Light=nullptr;
	}

	/*void Fill(TMapBlock* Block)
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

	
	void AddObject(TMapObject* Object0,TMapElement* Cell0)
	{
		Object0->Cell=Cell0;
		for(int i=0;i<Object0->Area.y;i++)
		{
			for(int j=0;j<Object0->Area.x;j++)
			{
				Items[Cell0->y+i][Cell0->x-j]->AddObjectToCell(Object0);
			}
		}
	}

	void GenerateRoom(void)
	{
		for(int i=0;i<Height;i++)
		{
			for(int j=0;j<Width;j++)
			{
				if ((i==0)||(j==0)||(i==Height-1)||(j==Width-1))
				{
					Items[i][j]=new TMapElement(j,i);
					AddObject(new TWall(),Items[i][j]);
				} else {
					Items[i][j]=new TMapElement(j,i);
					AddObject(new TFloor(),Items[i][j]);
				}
			}
		}

	}

	void DivideBlock(TMapBlock* Block,int Depth,int Current)
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
			Block->Left=new TMapBlock;
			Block->Left->coor[0]=Block->coor[0];
			Block->Left->coor[1]=Block->coor[1];
			Block->Left->coor[2]=Block->coor[0]+Choise;
			Block->Left->coor[3]=Block->coor[3];
			Block->Right=new TMapBlock;
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
			Block->Left=new TMapBlock;
			Block->Left->coor[0]=Block->coor[0];
			Block->Left->coor[1]=Block->coor[1];
			Block->Left->coor[2]=Block->coor[2];
			Block->Left->coor[3]=Block->coor[1]+Choise;
			Block->Right=new TMapBlock;
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

	//void ListBlock(TMapBlock* Block)
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
		TMapBlock* Block;
		Block=new TMapBlock;
		Block->coor[0]=0;
		Block->coor[1]=0;
		Block->coor[2]=MapWidth-1;
		Block->coor[3]=MapHeight-1;
		Block->Left=nullptr;
	    Block->Right=nullptr;
		DivideBlock(Block,100,0);
		//Fill(Block);
	}

	void MoveObject(TMapObject* Obj,int x,int y)
	{
		for(int i=0;i<Obj->Area.y;i++)
		{
			for(int j=0;j<Obj->Area.x;j++)
			{
				if(Items[y+i][x-j]==nullptr){return;}
				if(!Items[y+i][x-j]->AccessToMove(Obj)){return;}
			}
		}
		for(int i=0;i<Obj->Area.y;i++)
		{
			for(int j=0;j<Obj->Area.x;j++)
			{
				TMapObjectList* Current;
				Current=Items[Obj->Cell->y+i][Obj->Cell->x-j]->Objects;
				while(Current->Tail!=nullptr)
				{
					if(Current->Tail->Item==Obj)
					{
						Current->Tail=Current->Tail->Tail;
						break;
					}
					Current=Current->Tail;
				}
			}
		}
		AddObject(Obj,Items[y][x]);
	}

	void AddLight(TMapObject* Object)
	{
		TMapObjectList* Element;
		Element=new TMapObjectList;
		Element->Item=Object;
		Element->Tail=Light;
		Light=Element;
	}

	void AddNewObject(TMapObject* Object,TMapElement* Element)
	{
		AddObject(Object,Element);
		AddLight(Object);
	}
};

