#ifndef IMPACT_COPY_CHANCE_H
#define	IMPACT_COPY_CHANCE_H


#include "Effect.h"
class Impact_copy_chance :
	public Interaction_slot
{
public:

	int m_chance;

	Impact_copy_chance();
	~Impact_copy_chance();

	virtual Object_interaction* clone();
	std::u16string get_description() override;

	virtual void save();
	virtual void load();
};

#endif // IMPACT_COPY_CHANCE_H