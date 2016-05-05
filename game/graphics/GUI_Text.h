#ifndef GUI_TEXT_H
#define	GUI_TEXT_H

#include "GUI_Object.h"
#include <string>
#include "Application.h"

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

	std::string m_text;
};

#endif //GUI_TEXT_H
