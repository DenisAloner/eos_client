#include "GUI_Window.h"


GUI_Window::GUI_Window(int _x, int _y, int _width, int _height, std::string _Name) :GUI_Container(_x, _y, _width, _height)
{
	IsMoving = false;
	GUI_Item* Header = new GUI_Item(2,2,width-4,21,_Name,nullptr);
	Header->Close += std::bind(&GUI_Window::OnHeaderClose, this);
	AddManagingControl(Header);
	StartMoving += std::bind(&GUI_Window::OnStartMoving, this, std::placeholders::_1);
	Move += std::bind(&GUI_Window::OnMove, this, std::placeholders::_1);
	EndMoving += std::bind(&GUI_Window::OnEndMoving, this, std::placeholders::_1);
	Close += std::bind(&GUI_Window::OnClose, this, std::placeholders::_1);
}


GUI_Window::~GUI_Window()
{
} 


void GUI_Window::OnMouseMove(MouseEventArgs const& e)
{
	MouseEventArgs LocalMouseEventArgs = SetLocalMousePosition(e);
	for (std::list<GUI_Object*>::iterator Current = Items.begin(); Current != Items.end(); Current++)
	{
		if ((*Current)->CheckRegion(LocalMouseEventArgs))
		{
			(*Current)->MouseMove(LocalMouseEventArgs);
			return;
		}
	}
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
	}
}

void GUI_Window::OnStartMoving(MouseEventArgs const& e)
{
	IsMoving = true;
	InitialPosition.x = x-e.Position.x;
	InitialPosition.y = y-e.Position.y;
	//MessageBox(NULL, "Start", "", MB_OK);
}

void GUI_Window::OnMove(MouseEventArgs const& e)
{
	x = InitialPosition.x + e.Position.x;
	y = InitialPosition.y + e.Position.y;
}

void GUI_Window::OnEndMoving(MouseEventArgs const& e)
{
	IsMoving = false;
	//MessageBox(NULL, "End", "", MB_OK);

}

void GUI_Window::OnClose(GUI_Object* e)
{
	Destroy(this);
}

void GUI_Window::OnHeaderClose()
{
	Close(this);
}

void GUI_Window::Resize(int _width, int _height)
{
	//width = _width;
	//height = _height;
	//ManagingControls->width = width;
	//ManagingControls->height = height;
	//ItemControls->width = width;
	//ItemControls->height = height;
}