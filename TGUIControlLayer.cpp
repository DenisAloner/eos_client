#include "TGUIControlLayer.h"
#include "glew.h"
#include "TGraph.h"


TGUIControlLayer::TGUIControlLayer(TGUI* _Owner):TGUILayer(_Owner)
{
}


TGUIControlLayer::~TGUIControlLayer(void)
{
}

bool TGUIControlLayer::Select(int x,int y)
{
	for(std::list<TGUIControl*>::iterator Current=Items.begin();Current!=Items.end();Current++)
	{
		if((*Current)->Select(x,y))
		{
			return true;
			break;
		}
	}
}

void TGUIControlLayer::Render(TGraph* Graph)
{
	glEnable(GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgramObjectARB(0);
	for(std::list<TGUIControl*>::iterator Current=Items.begin();Current!=Items.end();Current++)
	{
		(*Current)->Render(Graph);
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
