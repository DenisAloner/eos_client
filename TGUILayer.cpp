#include "TGUILayer.h"
#include <Windows.h>
#include "TGUI.h"


TGUILayer::TGUILayer(TGUI* _Owner)
{
	Owner=_Owner;
}


TGUILayer::~TGUILayer(void)
{
}

bool TGUILayer::Select(int x,int y)
{
	return false;
}

void TGUILayer::Render(TGraph* Graph)
{
}

