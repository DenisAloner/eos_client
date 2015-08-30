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
	virtual std::string get_description();
	virtual void description(std::list<std::string>* info, int level);

	virtual void save();
	virtual void load();
};

#endif // IMPACT_COPY_CHANCE_H