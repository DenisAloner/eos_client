#pragma once
#include "TGUILayer.h"
#include <list>
#include "TGUIControl.h"

class TGUIControlLayer :
	public TGUILayer
{
public:

	std::list<TGUIControl*> Items;

	TGUIControlLayer(TGUI* _Owner);
	~TGUIControlLayer(void);

	virtual bool Select(int x,int y);
	virtual void Render(TGraph* Graph);
};

