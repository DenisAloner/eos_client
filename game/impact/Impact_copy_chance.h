#ifndef IMPACT_COPY_CHANCE_H
#define	IMPACT_COPY_CHANCE_H


#include "game\Effect.h"
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
};

#endif // IMPACT_COPY_CHANCE_H