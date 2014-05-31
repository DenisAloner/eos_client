#include "TGCButton.h"
#include "glew.h"
#include "GraphicalController.h"


TGCButton::TGCButton(void)
{
	focused=false;
	MouseClick += std::bind(&TGCButton::OnMouseClick,this,std::placeholders::_1);
}


TGCButton::~TGCButton(void)
{
}

void TGCButton::OnMouseClick(MouseEventArgs const& e)
{
	GetFocus(this);
	//MessageBox(NULL,"Cool","Cool",MB_OK);
	//PostQuitMessage(0);
}

void TGCButton::Render(GraphicalController* Graph)
{
	//glColor4d(1,1,1,1);
	//glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, Graph->Sprites[17]);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glUseProgramObjectARB(0);
	//glActiveTextureARB(GL_TEXTURE0_ARB);
	//Graph->DrawSprite(x,y,x,y+height,x+width,y+height,x+width,y);
	////Graph->CenterText(x+width/2,y+height/2,Text,16,32);
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	glEnable(GL_SCISSOR_TEST);
	glScissor(x -1, 1023 - y - height, width+1, height+1);
	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glVertex2d(x, y);
	glVertex2d(x, y + height);
	glVertex2d(x, y + height);
	glVertex2d(x + width, y + height);
	glVertex2d(x + width, y + height);
	glVertex2d(x + width, y);
	glVertex2d(x + width, y);
	glVertex2d(x, y);
	glEnd();
	glDisable(GL_SCISSOR_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Graph->Sprites[19]);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 1024, 1024);
	GLuint fb;
	glGenFramebuffersEXT(1, &fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Graph->Sprites[19], 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
	glUseProgramObjectARB(Graph->HorizontalShader);
	Graph->setUniformSampler(Graph->HorizontalShader, "Map");
	Graph->DrawSprite(0, 0, 0, 1024, 1024, 1024, 1024, 0);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 1024, 1024);
	glUseProgramObjectARB(Graph->VerticalShader);
	Graph->setUniformSampler(Graph->VerticalShader, "Map");
	Graph->DrawSprite(0, 0, 0, 1024, 1024, 1024, 1024, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_BLEND);
	if (Graph->AddScissor(new GLint[4]{x, y, width, height}))
	{
		glColor4d(0.0, 0.0, 0.0, 0.0);
		Graph->DrawSprite(0, 0, 0, 1024, 1024, 1024, 1024, 0);
		Graph->RemoveScissor();
	}
	glDisable(GL_SCISSOR_TEST);
	//Graph->DrawSprite(x, y, x, y + height, x + width, y + height, x + width, y);
	glUseProgramObjectARB(0);
}

