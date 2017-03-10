#ifndef GUI_TEXT_H
#define	GUI_TEXT_H

#include "GUI_Object.h"
#include <string>
#include "Application.h"

class GUI_Text :
	public GUI_Object
{
public:

	GUI_Text(std::u16string text);
	GUI_Text(std::u16string text, GUI_TextFormat* format);
	~GUI_Text();

	GUI_TextFormat* m_format;

	void render(GraphicalController* Graph, int px, int py) override;
	void set(std::u16string t);

	std::u16string m_text;
};

#endif //GUI_TEXT_H
