#ifndef IMPACT_RANDOM_VALUE_H
#define	IMPACT_RANDOM_VALUE_H

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

#endif // IMPACT_RANDOM_VALUE_H