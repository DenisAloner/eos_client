#pragma once

#include "TGUI.h"
#include "Event.h"


class TClient
{
public:

	WPARAM Key;

	TGUI* GUI;
	TGraph* Graph;

	Event KeyPress;

	TClient(void);
	~TClient(void);
};

