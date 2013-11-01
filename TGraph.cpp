#include "TGraph.h"

void TGraph::DrawSprite(double x0,double y0,double x1,double y1,double x2,double y2,double x3,double y3)
	{
		glBegin(GL_QUADS);
		glTexCoord2d(0,1); glVertex2d(x0, y0);
		glTexCoord2d(0,0); glVertex2d(x1, y1);
		glTexCoord2d(1,0); glVertex2d(x2, y2);
		glTexCoord2d(1,1); glVertex2d(x3, y3);
		glEnd();
	}
void TGraph::CenterText(int x,int y,char* Text,int size)
{
	int cx=x-(strlen(Text))*(size+1)/2;
	int cy=y-size/2;
	TextXY(cx,cy,Text,size);
}