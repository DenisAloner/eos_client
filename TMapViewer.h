#pragma once
#include "tguilayer.h"
#include "TMap.h"
#include "TGUI.h"
class TMapViewer :
	public TGUILayer
{ 
public:

	TMapViewer(TGUI* _Owner);
	~TMapViewer(void);

	static const int Width=32;
    static const int Height=64;
	TPlayer* Player;
	TMap* Map;
	TMapElement* Items[Height][Width];

	void Update();
	virtual bool Select(int x,int y);
	virtual void Render(TGraph* Graph);
	
	GLuint Sprites[9];

	void LoadMaptextures(TGraph* Graph);

};