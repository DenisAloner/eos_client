#include "GUI_Item.h"


GUI_Item::GUI_Item(int _x, int _y, int _width, int _height, std::string text, GameTask* tag) :GUI_Container(_x, _y, _width, _height)
{

	Tag = tag;
	GUI_Text* m_text = new GUI_Text(text);
	m_text->x = 2;
	m_text->y = 2;
	AddItemControl(m_text);
	GUI_ItemButton* m_button = new GUI_ItemButton();
	m_button->x = width - (height - 4) - 2;
	m_button->y =2;
	m_button->width = height - 4;
	m_button->height = height - 4;
	AddManagingControl(m_button);
	m_button->MouseClick += std::bind(&GUI_Item::OnCloseButtonClick, this);
}


GUI_Item::~GUI_Item()
{
}

void GUI_Item::OnCloseButtonClick()
{
	Close(this);
}

void GUI_Item::Resize(int _width, int _height)
{
	width = _width;
	height = _height;
	ManagingControls->width = width;
	ManagingControls->height = height;
	ItemControls->width = width;
	ItemControls->height = height;
	ManagingControls->Items.front()->x = width - (height - 4) - 2;
	ManagingControls->Items.front()->y = 2;
	ManagingControls->Items.front()->width = height - 4;
	ManagingControls->Items.front()->height= height - 4;

}

//void  GUI_Item::OnMouseClick(MouseEventArgs const& e)
//{
//	MessageBox(NULL, "Yes", "", MB_OK);
//}