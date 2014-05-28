#include "GUI_MapViewer.h"
#include "GUI_Layer.h"
#include "GUI_PopMenu.h"

GCMapElement::GCMapElement()
{
}

GCMapElement::~GCMapElement(void)
{
}

GUI_MapViewer::GUI_MapViewer(Application* app = nullptr)
{
	JustFocused = false;
	IsMoving = false;
	this->app=app;
	focus=nullptr;
	cursored = nullptr;
	AppliedAction = nullptr;
	CursorX = 1;
	CursorY = 1;
	for (int i = 0; i<128; i++)
	{
		for (int j = 0; j<64; j++)
		{
			Items[i][j]=new GCMapElement();
			Items[i][j]->GetFocus+=std::bind(&GUI_MapViewer::OnItemGetFocus,this,std::placeholders::_1);
			Items[i][j]->x = j;
			Items[i][j]->y = i;
		}
	}
	StartMoving += std::bind(&GUI_MapViewer::OnStartMoving, this, std::placeholders::_1);
	Move += std::bind(&GUI_MapViewer::OnMove, this, std::placeholders::_1);
	EndMoving += std::bind(&GUI_MapViewer::OnEndMoving, this, std::placeholders::_1);
}

GUI_MapViewer::~GUI_MapViewer(void)
{
}

void GUI_MapViewer::Ñalculate()
{
	TileSizeX = static_cast<double>(width) / static_cast<double>(TileCountX);
	TileSizeY = static_cast<double>(height) / static_cast<double>(TileCountY);
}

void GUI_MapViewer::Update()
{
	Ñalculate();
	int x;
	int y;
	for (int i = 0; i<TileCountY; i++)
	{
		for (int j = 0; j<TileCountX; j++)
		{
			x = center.x + j - TileCountX / 2;
			y = center.y + i - TileCountY / 2;
			if((x<0)||(x>Map->Width-1)||(y<0)||(y>Map->Height-1))
			{
				Items[i][j]->MapElement=nullptr;
			} else {
				Items[i][j]->MapElement=Map->Items[y][x];
			}
		}
	}
}

bool GUI_MapViewer::Select(int x,int y)
{
	//int CellX=x/32;
	//int CellY=63-y/16;
	////char buf2[32];
	////itoa(CellX, buf2, 10);
	////MessageBox(0, buf2, "X", MB_OK);
	////itoa(CellY, buf2, 10);
	////MessageBox(0, buf2, "Y", MB_OK);
	//if(Items[CellY][CellX]!=nullptr)
	//{
	//	Items[CellY][CellX]->IsSelected=!Items[CellY][CellX]->IsSelected;
	//}
	return false;
}


void GUI_MapViewer::RenderAt(GraphicalController* Graph, int px, int py)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable (GL_BLEND);
		glColor4d(1.0,1.0,1.0,1.0);
			double x0,y0,x1,y1,x2,y2,x3,y3;
			float LightInfo[4];
			float LightPos[4];
			bool IsDraw;
			for (int y = TileCountY - 1; y>-1; y--)
			{
				for (int x = 0; x<TileCountX; x++)
				{
					if(Items[y][x]->MapElement!=nullptr)
					{
						for(std::list<GameObject*>::iterator Current=Items[y][x]->MapElement->Objects.begin();Current!=Items[y][x]->MapElement->Objects.end();Current++)
						{
							IsDraw=false;
							if((*Current)->Cell->x==Items[y][x]->MapElement->x&&(*Current)->Cell->y==Items[y][x]->MapElement->y)
							{
								x0=(px+x+1-(*Current)->Width)*TileSizeX;
								y0 = (TileCountY -py-y - (*Current)->Height)*TileSizeY;
								x1 = (px+x + 1 - (*Current)->Width)*TileSizeX;
								y1 = (TileCountY - py-y)*TileSizeY;
								x2 = (px +x+ 1)*TileSizeX;
								y2 = (TileCountY - py-y)*TileSizeY;
								x3 = (px+x + 1)*TileSizeX;
								y3 = (TileCountY - py -y- (*Current)->Height)*TileSizeY;
								LightPos[0]=Items[y][x]->MapElement->x;
								LightPos[1]=Items[y][x]->MapElement->y;
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
								unsigned int Sprite = (*Current)->Sprites[(*Current)->direction][app->m_timer->tick / 2];
								(*Current)->Render(x,y);
								glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
								glUseProgramObjectARB(Graph->RenderShader);
								glActiveTextureARB(GL_TEXTURE0_ARB);
								glBindTexture( GL_TEXTURE_2D, Graph->Sprites[Sprite]);
								Graph->setUniformSampler(Graph->RenderShader,"Map");
								Graph->DrawSprite(x0,y0,x1,y1,x2,y2,x3,y3);
								glUseProgramObjectARB(Graph->LightShader);
								glActiveTextureARB(GL_TEXTURE0_ARB);
								glBindTexture(GL_TEXTURE_2D, Graph->Sprites[Sprite]);
								Graph->setUniformSampler(Graph->LightShader,"Map");
								glBlendFunc (GL_SRC_ALPHA, GL_ONE);
								for(std::list<GameObject*>::iterator CurrentLight=Map->Light.begin();CurrentLight!=Map->Light.end();CurrentLight++)
								{
									LightPos[2]=(*CurrentLight)->Cell->x;
									LightPos[3]=(*CurrentLight)->Cell->y;
									Graph->setUniformVector(Graph->LightShader,"LightPos",LightPos);
									LightInfo[0]=(*CurrentLight)->Light->RGB[0];
									LightInfo[1]=(*CurrentLight)->Light->RGB[1];
									LightInfo[2]=(*CurrentLight)->Light->RGB[2];
									LightInfo[3]=(*CurrentLight)->Light->Power;
									Graph->setUniformVector(Graph->LightShader,"LightInfo",LightInfo);
									//char buf2[32];
									//itoa(glGetAttribLocationARB(Graph->LightShader,"in_vertex"), buf2, 10);
									//MessageBox(0, buf2, "Caption", MB_OK);
									Graph->DrawSprite(x0,y0,x1,y1,x2,y2,x3,y3);
								}
								if ((*Current)->selected)
								{

									glColor4d(0.0,abs( (app->m_timer->tick-3.5)/3.5), 0.0, 1.0);
									glBlendFunc(GL_SRC_ALPHA, GL_ONE);
									glUseProgramObjectARB(0);
									glActiveTextureARB(GL_TEXTURE0_ARB);
									glBindTexture(GL_TEXTURE_2D, Graph->Sprites[Sprite]);
									Graph->DrawSprite(x0, y0, x1, y1, x2, y2, x3, y3);
									glColor4d(1.0, 1.0, 1.0, 1.0);
								}
							}
						}
					}
				}
			}
			if(cursored != nullptr)
			{
				GCMapElement* Item = cursored;
				if(Item->MapElement!=nullptr)
				{
					int x = px+Item->x;
					int y =py+ Item->y;
					x0 = (x - CursorX + 1) * TileSizeX;
					y0 = (TileCountY - y - CursorY) * TileSizeY;
					x1 = (x - CursorX + 1) * TileSizeX;
					y1 = (TileCountY - y) * TileSizeY;
					x2 = (x + 1) * TileSizeX;
					y2 = (TileCountY - y) * TileSizeY;
					x3 = (x + 1) * TileSizeX;
					y3 = (TileCountY - y - CursorY) * TileSizeY;
					glUseProgramObjectARB(0);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_TEXTURE_2D);
					glActiveTextureARB(GL_TEXTURE0_ARB);
					glBindTexture(GL_TEXTURE_2D, Graph->Sprites[8]);
					glColor4f(1.0, 1.0, 1.0, 0.5);
					Graph->DrawSprite(x0, y0, x1, y1, x2, y2, x3, y3);
					glDisable(GL_BLEND);
					glDisable(GL_TEXTURE_2D);
					glBegin(GL_LINES);
					glColor4f(0.0, 1.0, 0.0, 1.0);
					glVertex2d(x0, y0);
					glVertex2d(x1, y1);
					glVertex2d(x1, y1);
					glVertex2d(x2, y2);
					glVertex2d(x2, y2);
					glVertex2d(x3, y3);
					glVertex2d(x3, y3);
					glVertex2d(x0, y0);
					glEnd();
					glEnable(GL_BLEND);
					glEnable(GL_TEXTURE_2D);
					glColor4f(1.0, 1.0, 1.0, 1);
				}
			}
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
}

KeyboardEventCallback GUI_MapViewer::GetKeyboardEventCallback()
{
	return std::bind(&GUI_MapViewer::OnKeyPress,this,std::placeholders::_1);
}

void GUI_MapViewer::OnKeyPress(WPARAM w)
{
	Parameter_Position* P;
	switch (w)
	{
	case VK_ESCAPE:
		PostQuitMessage(0);
		return;
	case VK_UP:
		P = new Parameter_Position();
		P->object=Player;
		P->place=Map->Items[Player->Cell->y+1][Player->Cell->x];
		P->map=Map;
		app->ActionManager->Actions.push_back(new GameTask(app->Actions[ActionKind_Move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_DOWN:
		P = new Parameter_Position();
		P->object=Player;
		P->place=Map->Items[Player->Cell->y-1][Player->Cell->x];
		P->map=Map;
		app->ActionManager->Actions.push_back(new GameTask(app->Actions[ActionKind_Move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_LEFT:
		P = new Parameter_Position();
		P->object=Player;
		P->place=Map->Items[Player->Cell->y][Player->Cell->x-1];
		P->map=Map;
		app->ActionManager->Actions.push_back(new GameTask(app->Actions[ActionKind_Move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_RIGHT:
		P = new Parameter_Position();
		P->object=Player;
		P->place=Map->Items[Player->Cell->y][Player->Cell->x+1];
		P->map=Map;
		app->ActionManager->Actions.push_back(new GameTask(app->Actions[ActionKind_Move], P));
		P->~Parameter_Position();
		//app->TurnEnd=true;
		return;
	case VK_SPACE:
		if (app->MessageQueue.Reader)
		{
			TParameter* p = new TParameter(ParameterKind_Cancel);
			app->MessageQueue.Push(p);
		}
		return;
		}
}

MouseEventCallback GUI_MapViewer::GetMouseEventCallback()
{
	return std::bind(&GUI_MapViewer::OnMouseClick,this,std::placeholders::_1);
}

void GUI_MapViewer::OnMouseClick(MouseEventArgs const& e)
{
	if (e.Flags &MK_LBUTTON)
	{
		Point p = LocalXY(Point(e.Position.x, e.Position.y));
		if (Items[p.y][p.x] != nullptr)
		{
			Items[p.y][p.x]->MouseClick(e);
			Parameter_MapCell* pr = new Parameter_MapCell();
			if ((Items[p.y][p.x]->MapElement != nullptr) && (!JustFocused))
			{
				pr->place = Items[p.y][p.x]->MapElement;
				if (app->MessageQueue.Reader)
				{
					app->MessageQueue.Push(pr);
				}
			}
			JustFocused = false;
		}
	} else {
		GUI_PopMenu* PopMenu;
		PopMenu=new GUI_PopMenu;
		PopMenu->x=e.Position.x;
		PopMenu->y=e.Position.y;
		PopMenu->Name="PopMenu";
		PopMenu->Game = app;
		Point p = LocalXY(Point(e.Position.x, e.Position.y));
		if (Items[p.y][p.x] != nullptr)
		{
			//UnderCursor(CursorEventArgs(e.x, e.y));
			for (std::list<GameObject*>::iterator Current = Items[p.y][p.x]->MapElement->Objects.begin(); Current != Items[p.y][p.x]->MapElement->Objects.end(); Current++)
			{
				PopMenu->add((*Current)->Name, (*Current));
			}
		}
		if(PopMenu->Items.size()==0)
		{
			PopMenu->add("Íåò äåéñòâèé",nullptr);
		}
		PopMenu->Destroy+=std::bind(&GUI_Layer::remove,GUI,std::placeholders::_1);
		GUI->AddFront(PopMenu);
		PopMenu->Items.front()->SetFocus(true);
	}
}

void GUI_MapViewer :: OnMouseDown(MouseEventArgs const& e)
{
	if ((e.Flags &MK_LBUTTON) || (e.Flags &MK_RBUTTON))
	{
		Point p = LocalXY(e.Position);
		if (Items[p.y][p.x] != nullptr)
		{
			Items[p.y][p.x]->MouseDown(e);
		}

	}
}

void GUI_MapViewer::OnMouseScroll(MouseEventArgs const& e)
{
	if (e.Flags &MK_LBUTTON)
	{
		int dx=0;
		int dy=0;
		int limit = 1;
		if (e.Position.x > 2*width / 3){ dx = limit; }
		if (e.Position.x < width / 3){ dx = -limit; }
		//if (e.p.x - width/ 2>width/4){ dx = 4*limit; }
		if (e.Position.y > 2 * height / 3){ dy = -limit; }
		if (e.Position.y < height / 3){ dy = limit; }
		center.x = center.x + dx;
		center.y = center.y + dy;
	}
}

void GUI_MapViewer::OnMouseWheel(MouseEventArgs const& e)
{
	int delta = GET_WHEEL_DELTA_WPARAM(e.Flags);
	if (delta>0)
	{
		TileCountX += 2;
		TileCountY += 4;
		if (TileCountX > 64){ TileCountX = 64; }
		if (TileCountY > 128){ TileCountY = 128;}
	}
	else
	{
		TileCountX -= 2;
		TileCountY -= 4;
		if (TileCountX < 16){ TileCountX = 16; }
		if (TileCountY < 32){ TileCountY = 32; }
	}
}

void GUI_MapViewer::SetFocus(bool state)
{
	if (!focused&&state)
	{
		JustFocused = true;
	}
	if (!state)
	{
		JustFocused = false;
	}
	focused=state;
	if(state)
	{
		if(focus!=nullptr)
		{
			focus->SetFocus(false);
		}
		GetFocus(this);
	} else {
		if(focus!=nullptr)
		{
			focus->SetFocus(false);
		}
		LoseFocus(this);
	}
}

void GUI_MapViewer::OnItemGetFocus(GUI_Object* sender)
{
	if(focus!=sender)
	{
		SetFocus(true);
		focus=sender;
	}
}

void GUI_MapViewer::OnUnderCursor(MouseEventArgs const& e)
{
	Point p = LocalXY(Point(e.Position.x,e.Position.y));
	if (Items[p.y][p.x] != nullptr)
	{
		cursored = Items[p.y][p.x];
	}
}

void GUI_MapViewer::OnLoseFocus(GUI_Object* sender)
{
	cursored = nullptr;
}

Point GUI_MapViewer::LocalXY(Point p)
{
	Point Result = Point(p.x / TileSizeX, TileCountY - p.y / TileSizeY);
	if (Result.x > TileCountX - 1){ Result.x = TileCountX - 1; }
	if (Result.x <0){ Result.x = 0; }
	if (Result.y > TileCountY - 1){ Result.y = TileCountY - 1; }
	if (Result.y <0){ Result.y = 0; }
	return Result;
}
void GUI_MapViewer::OnMouseMove(MouseEventArgs const& e)
{
	if (e.Flags&MK_LBUTTON)
	{
		if (!IsMoving)
		{
			StartMoving(e);
		}
		Move(e);
	}
	else
	{
		if (IsMoving)
		{
			EndMoving(e);
		}
		MouseEventArgs LocalMouseEventArgs = SetLocalMousePosition(e);
		if (focus)
		{
			focus->MouseMove(LocalMouseEventArgs);
		}
	}
}

void GUI_MapViewer::OnStartMoving(MouseEventArgs const& e)
{
	IsMoving = true;
	Point p = LocalXY(Point(e.Position.x, e.Position.y));
	InitialPosition.x = center.x - p.x;
	InitialPosition.y =center.y - p.y;
	//MessageBox(NULL, "Start", "", MB_OK);
}

void GUI_MapViewer::OnMove(MouseEventArgs const& e)
{
	Point p = LocalXY(Point(e.Position.x, e.Position.y));
	center.x = InitialPosition.x + p.x;
	center.y = InitialPosition.y + p.y;
}

void GUI_MapViewer::OnEndMoving(MouseEventArgs const& e)
{
	IsMoving = false;
	//MessageBox(NULL, "End", "", MB_OK);

}

MouseEventArgs GUI_MapViewer::SetLocalMousePosition(MouseEventArgs const& source)
{
	return MouseEventArgs(Point(source.Position.x - x, source.Position.y - y), source.Flags);
}