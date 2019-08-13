#include "GUI_Timer.h"


GUI_Timer::GUI_Timer(int _x, int _y, int _width, int _height, int time) :GUI_Container(_x, _y, _width, _height)
{
	GUI_Layer::add(new GUI_Text(u""));
	Update(time);
}


GUI_Timer::~GUI_Timer()
{
}

void GUI_Timer::Update(int time)
{
	GUI_Text* Text = static_cast<GUI_Text*>(m_items.front());
	Text->m_format->m_symbol_size.w = 48;
	Text->m_format->m_symbol_size.h = 57;
	Text->m_format->m_color.r = 1 - time / 15.0F;
	Text->m_format->m_color.g = time / 15.0F;
	Text->m_format->m_color.b = 0.0;
	Text->set(cp1251_to_utf16(std::to_string(time)));
	Text->m_position.x = m_size.w*0.5 - Text->m_size.w*0.5;
	Text->m_position.y = m_size.h*0.5 - Text->m_size.h*0.5 + 2;
}