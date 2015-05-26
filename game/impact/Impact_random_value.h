#pragma once
#include "Effect.h"

class Impact_random_value :
	public Effect
{
public:

	int m_min_value;

	Impact_random_value();
	~Impact_random_value();

	virtual Effect* clone();
};

