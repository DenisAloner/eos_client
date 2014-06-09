#pragma once
#include "GUI_Object.h"
#include <string>


struct TColor
{
	float R, G, B, A;

	TColor(float _R, float _G, float _B, float _A) : R(_R), G(_G), B(_B), A(_A){};

	float TColor::operator[](const std::size_t& index)
	{
		switch (index)
		{
		case 0: return R;
		case 1: return G;
		case 2: return B;
		case 3: return A;
		}
	}
};

class GUI_TextFormat
{
public:

	GUI_TextFormat(int _SizeX, int _SizeY, TColor _Color) : m_symbol_size(dimension_t(_SizeX,_SizeY)), m_color(_Color) {};
	~GUI_TextFormat();
	TColor m_color;
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

