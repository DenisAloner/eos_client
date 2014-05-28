#pragma once
#include "GUI_Layer.h"
#include "Description_GUI.h"
#include "GUI_MapViewer.h"
#include "GUI_Timer.h"


class GUI_MapViewer;
class GUI_Timer;

class ApplicationGUI :
	public GUI_Layer
{
public:
	ApplicationGUI();
	~ApplicationGUI();

	GUI_MapViewer* MapViewer;
	GUI_TextBox* DescriptionBox;
	GUI_Timer* Timer;
};

