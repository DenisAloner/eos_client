#include "TMapViewer.h"

TMapViewer::TMapViewer(TGUI* _Owner):TGUILayer(_Owner)
{
}

TMapViewer::~TMapViewer(void)
{
}

void TMapViewer::Update()
{
	int x;
	int y;
	for(int i=0;i<Height;i++)
	{
		for(int j=0;j<Width;j++)
		{
			x=Player->Cell->x+j-Width/2;
			y=Player->Cell->y+i-Height/2;
			if((x<0)||(x>Map->Width-1)||(y<0)||(y>Map->Height-1))
			{
				Items[i][j]=nullptr;
			} else {
				Items[i][j]=Map->Items[y][x];
			}
		}
	}
}

bool TMapViewer::Select(int x,int y)
{
	int CellX=x/32;
	int CellY=63-y/16;
	//char buf2[32];
	//itoa(CellX, buf2, 10);
	//MessageBox(0, buf2, "X", MB_OK);
	//itoa(CellY, buf2, 10);
	//MessageBox(0, buf2, "Y", MB_OK);
	if(Items[CellY][CellX]!=nullptr)
	{
		Items[CellY][CellX]->IsSelected=!Items[CellY][CellX]->IsSelected;
	}
	return true;
}

	void TMapViewer::LoadMaptextures(TGraph* Graph)
	{
		Sprites[0]=Graph->LoadTexture("C:\\ExplorersOfSaarum\\sprite_0_3D.bmp",false);
		Sprites[1]=Graph->LoadTexture("C:\\ExplorersOfSaarum\\sprite_9.bmp",false);
		Sprites[2]=Graph->LoadTexture("C:\\ExplorersOfSaarum\\sprite_8.bmp",false);
		Sprites[3]=Graph->LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-1.bmp",false);
		Sprites[4]=Graph->LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-2.bmp",false);
		Sprites[5]=Graph->LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-3.bmp",false);
		Sprites[6]=Graph->LoadTexture("C:\\ExplorersOfSaarum\\ElfSprite-4.bmp",false);
		Sprites[7]=Graph->LoadTexture("C:\\ExplorersOfSaarum\\orc.bmp",false);
		Sprites[8]=Graph->LoadTexture("C:\\ExplorersOfSaarum\\select.bmp",false);
	}


void TMapViewer::Render(TGraph* Graph)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable (GL_BLEND);
		glColor4d(1,1,1,1);
			int x0,y0,x1,y1,x2,y2,x3,y3;
			float LightInfo[4];
			float LightPos[4];
			bool IsDraw;
			for(int y=Height-1;y>-1;y--)
			{
				for(int x=0;x<Width;x++)
				{
					if(Items[y][x]!=nullptr)
					{
						TMapObjectList* Current;
						Current=Items[y][x]->Objects;
						while(Current!=nullptr)
						{
							IsDraw=false;
							if(Current->Item->Cell->x==Items[y][x]->x&&Current->Item->Cell->y==Items[y][x]->y)
							{
								x0=(x+1-Current->Item->Width)*32;
								y0=(64-y-Current->Item->Height)*16;
								x1=(x+1-Current->Item->Width)*32;
								y1=(64-y)*16;
								x2=(x+1)*32;
								y2=(64-y)*16;
								x3=(x+1)*32;
								y3=(64-y-Current->Item->Height)*16;
								LightPos[0]=Items[y][x]->x;
								LightPos[1]=Items[y][x]->y;
								LightPos[2]=0;
								LightPos[3]=0;
								IsDraw=true;
							} else {
								/*if((x==Width-1&&Current->Item->Cell->y==Items[y][x]->y)||(y==0&&Current->Item->Cell->x==Items[y][x]->x)||(x==Width-1&&y==0))
								{
									x0=(Current->Item->Cell->x+1-Current->Item->Width)*32;
									y0=(64-Current->Item->Cell->y-Current->Item->Height)*16;
									x1=(Current->Item->Cell->x+1-Current->Item->Width)*32;
									y1=(64-Current->Item->Cell->y)*16;
									x2=(Current->Item->Cell->x+1)*32;
									y2=(64-Current->Item->Cell->y)*16;
									x3=(Current->Item->Cell->x+1)*32;
									y3=(64-Current->Item->Cell->y-Current->Item->Height)*16;
									LightPos[0]=Current->Item->Cell->x;
									LightPos[1]=Current->Item->Cell->y;
									LightPos[2]=0;
									LightPos[3]=0;
									IsDraw=true;
								}*/
							}
							if(IsDraw)
							{
								Current->Item->Render(x,y);
								glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
								glUseProgramObjectARB(Graph->RenderShader);
								glActiveTextureARB(GL_TEXTURE0_ARB);
								glBindTexture( GL_TEXTURE_2D, Sprites[Current->Item->Sprite]);
								Graph->setUniformSampler(Graph->RenderShader,"Map");
								Graph->DrawSprite(x0,y0,x1,y1,x2,y2,x3,y3);
								glUseProgramObjectARB(Graph->LightShader);
								glActiveTextureARB(GL_TEXTURE0_ARB);
								glBindTexture( GL_TEXTURE_2D, Sprites[Current->Item->Sprite]);
								Graph->setUniformSampler(Graph->LightShader,"Map");
								TMapObjectList* CurrentLight=Map->Light;
								glBlendFunc (GL_SRC_ALPHA, GL_ONE);
								while(CurrentLight!=nullptr)
								{
									LightPos[2]=CurrentLight->Item->Cell->x;
									LightPos[3]=CurrentLight->Item->Cell->y;
									Graph->setUniformVector(Graph->LightShader,"LightPos",LightPos);
									LightInfo[0]=CurrentLight->Item->Light->RGB[0];
									LightInfo[1]=CurrentLight->Item->Light->RGB[1];
									LightInfo[2]=CurrentLight->Item->Light->RGB[2];
									LightInfo[3]=CurrentLight->Item->Light->Power;
									Graph->setUniformVector(Graph->LightShader,"LightInfo",LightInfo);
									//char buf2[32];
									//itoa(glGetAttribLocationARB(Graph->LightShader,"in_vertex"), buf2, 10);
									//MessageBox(0, buf2, "Caption", MB_OK);
									Graph->DrawSprite(x0,y0,x1,y1,x2,y2,x3,y3);
									CurrentLight=CurrentLight->Tail;
								}
							}
							if(Items[y][x]->IsSelected)
							{
								glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
								glUseProgramObjectARB(0);
								glActiveTextureARB(GL_TEXTURE0_ARB);
								glBindTexture(GL_TEXTURE_2D, Sprites[8]);
								x0=x*32;
								y0=(63-y)*16;
								x1=x*32;
								y1=(64-y)*16;
								x2=(x+1)*32;
								y2=(64-y)*16;
								x3=(x+1)*32;
								y3=(63-y)*16;
								LightPos[0]=Items[y][x]->x;
								LightPos[1]=Items[y][x]->y;
								LightPos[2]=0;
								LightPos[3]=0;
								Graph->DrawSprite(x0,y0,x1,y1,x2,y2,x3,y3);
							}
							Current=Current->Tail;
						}
					}
				}
			}
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
}
