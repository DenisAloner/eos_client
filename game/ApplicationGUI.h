#pragma once

#include "Description_GUI.h"
#include "graphics/GUI_Layer.h"
#include "graphics/GUI_MapViewer.h"
#include "graphics/GUI_Timer.h"


class GUI_MapViewer;
class GUI_Timer;

class ApplicationGUI : public GUI_Layer
{
public:
	ApplicationGUI();
	~ApplicationGUI();

	GUI_MapViewer* MapViewer;
	GUI_TextBox* DescriptionBox;
	GUI_Timer* Timer;
};
