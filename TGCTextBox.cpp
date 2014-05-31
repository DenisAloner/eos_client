#include "TGCTextBox.h"


TGCTextBox::TGCTextBox(void)
{
}


TGCTextBox::~TGCTextBox(void)
{
}

void TGCTextBox::OnKeyPress(WPARAM w)
{
  /*  char buf2[32];
	itoa((char)w, buf2, 10);
	MessageBox(0, buf2, "X", MB_OK);*/
	int len;
	char* result;
	switch (w)
	{
	case 8:
		len=strlen(Text)-1;
		result=new char[len];
		strcpy(result,Text);
		result[len]='\0';
		Text=result;
		return;
	default: 
		len=strlen(Text);
		result=new char[len+2];
		strcpy(result,Text);
		result[len]=(char)w;
		result[len+1]='\0';
		Text=result;
		return;
	}
}

void TGCTextBox::Render(GraphicalController* Graph)
{
	//glColor4d(1,1,1,1);
	//glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, Graph->PaperTexture);
	//Graph->DrawSprite(x,y,x,y+height,x+width,y+height,x+width,y);
	//Graph->CenterText(x+width/2,y+height/2,Text,8,17);
	//glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4d(1,1,1,0.7);
	Graph->DrawSprite(x,y,x,y+height,x+width,y+height,x+width,y);
	glEnable(GL_TEXTURE_2D);
	Graph->CenterText(x+width/2,y+height/2,Text,8,17);
	glDisable(GL_BLEND);
}

void TGCTextBox::OnMouseClick(MouseEventArgs const& e)
{
	GetFocus(this);
}