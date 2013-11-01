#pragma once
#include "TGraph.h"
class TGUI;
class TGUILayer
{
public:

	TGUI* Owner;
	TGUILayer(TGUI* _Owner);
	~TGUILayer(void);
	virtual bool Select(int x,int y);
	virtual void Render(TGraph* Graph);
};

