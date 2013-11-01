#pragma once
#include <stdio.h>
#include <windows.h>
#include <math.h>

struct TLight
{
	float Power;
	float RGB[3];
};

struct TArea
{
	int x;
	int y;
};

class TMapElement;
class TMapObject
{
public:


	int Width;
	int Height;
	TArea Area;
	TLight* Light;

	unsigned int Sprite;

	TMapElement* Cell;


	TMapObject()
	{
		Sprite=0;
		Cell=nullptr;
	}

	virtual bool AccessToMove(TMapObject* Obj)
	{
		
	 return true;
	
	}

	void Render(int x,int y)
	{
		
	}

	virtual void Move(TMapElement* Cell0)
	{
		Cell=Cell0;
	}

};

class TPlayer :
	public TMapObject
{
public:

	TPlayer()
	{
		Cell=nullptr;
		Sprite=3;
		Width=2;
		Height=6;
		Area.x=2;
		Area.y=1;
		Light=nullptr;
	}

};

class TOrc :
	public TMapObject
{
public:

	TOrc()
	{
		Cell=nullptr;
		Sprite=7;
		Width=3;
		Height=6;
		Area.x=3;
		Area.y=2;
		Light=nullptr;
	}

	virtual bool AccessToMove(TMapObject* Obj)
	{
		return false;	 
	}

};

class TFloor :
	public TMapObject
{
public:

	TFloor()
	{
		Cell=nullptr;
		Sprite=0;
		Width=1;
		Height=1;
		Light=nullptr;
		Area.x=1;
		Area.y=1;
	}


};

class TWall :
	public TMapObject
{
public:

	TWall()
	{
		Cell=nullptr;
		Sprite=1;
		Width=1;
		Height=5;
		Light=nullptr;
		Area.x=1;
		Area.y=1;
	}

	virtual bool AccessToMove(TMapObject* Obj)
	{
		return false;	 
	}

};

class TTorch:
	public TMapObject
{
public:

	TTorch()
	{
		Cell=nullptr;
		Sprite=2;
		Width=1;
		Height=4;
		Area.x=1;
		Area.y=1;
		Light=new TLight;
		Light->Power=2;
		Light->RGB[0]=1;
		Light->RGB[1]=1;
		Light->RGB[2]=1;
	}

	virtual bool AccessToMove(TMapObject* Obj)
	{
		return false;	 
	}
};