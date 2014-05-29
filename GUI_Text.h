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

	GUI_TextFormat(int _SizeX, int _SizeY, TColor _Color) : SizeX(_SizeX), SizeY(_SizeY), Color(_Color) {};
	~GUI_TextFormat();
	TColor Color;
	int SizeX;
	int SizeY;

};

class GUI_Text :
	public GUI_Object
{
public:

	GUI_Text(std::string _Text);
	GUI_Text(std::string _Text, GUI_TextFormat* _Format);
	~GUI_Text();

	GUI_TextFormat* Format;

	virtual void RenderAt(GraphicalController* Graph, int px, int py);
	void Set(std::string t);

private:
	std::string Text;
};

