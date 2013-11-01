#pragma once
#include <list>
#include "TGraph.h"

class TGUILayer;
class TGUI
{
public:
	TGUI(void);
	~TGUI(void);
	std::list<TGUILayer*> Layers;
	void Select(int x,int y);
	void Render(TGraph* _Graph);
};

