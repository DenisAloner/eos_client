#pragma once
#include "tguicontrol.h"
class TGCButton :
	public TGUIControl
{
public:
	
	char* Text;

	TGCButton(void);
	~TGCButton(void);

	virtual void Render(TGraph* Graph);
	void OnClick();
};

