#pragma once
#include "TGraph.h"
#include "Event.h"
class TGUIControl
{
public:

	int x;
	int y;
	int width;
	int height;
	Event Click;

	TGUIControl(void);
	~TGUIControl(void);

	virtual bool Select(int x,int y);
	virtual void Render(TGraph* Graph);
};

