#include "TGCButton.h"
#include "glew.h"
#include "TGraph.h"


TGCButton::TGCButton(void)
{
	Click+= Delegate(this, &TGCButton::OnClick);
}


TGCButton::~TGCButton(void)
{
}

void TGCButton::OnClick()
{
	MessageBox(NULL,"Cool","Cool",MB_OK);
}

void TGCButton::Render(TGraph* Graph)
{
	glBindTexture(GL_TEXTURE_2D, Graph->ButtonTexture);
	Graph->DrawSprite(x,y,x,y+height,x+width,y+height,x+width,y);
	Graph->CenterText(x+width/2,y+height/2,Text,16);
}
