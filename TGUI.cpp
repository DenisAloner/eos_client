#include "TGUI.h"
#include "TMap.h"
#include "TGUIlayer.h"

TGUI::TGUI()
{
}


TGUI::~TGUI(void)
{
}

void TGUI::Select(int x,int y)
{
	for(std::list<TGUILayer*>::iterator Current=Layers.begin();Current!=Layers.end();Current++)
	{
		if((*Current)->Select(x,y))
		{
			return;
		}

	}
}

void TGUI::Render(TGraph* Graph)
{
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );
		glPushMatrix();
		for(std::list<TGUILayer*>::reverse_iterator Current=Layers.rbegin();Current!=Layers.rend();Current++)
		{
			(*Current)->Render(Graph);
		}
		glPopMatrix();
}
