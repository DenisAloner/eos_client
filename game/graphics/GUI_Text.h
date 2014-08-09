#pragma once
#include "GUI_Object.h"
#include <string>


class GUI_TextFormat
{
public:

	GUI_TextFormat(int _SizeX, int _SizeY, RGBA_t _Color) : m_symbol_size(dimension_t(_SizeX,_SizeY)), m_color(_Color) {};
	~GUI_TextFormat();
	RGBA_t m_color;
	dimension_t m_symbol_size;

};

class GUI_Text :
	public GUI_Object
{
public:

	GUI_Text(std::string _Text);
	GUI_Text(std::string _Text, GUI_TextFormat* _Format);
	~GUI_Text();

	GUI_TextFormat* m_format;

	virtual void render(GraphicalController* Graph, int px, int py);
	void set(std::string t);

private:
	std::string m_text;
};

