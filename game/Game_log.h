#ifndef GAME_LOG_H
#define	GAME_LOG_H

#include <list>
#include "Event.h"

class Game_log
{

public:

	std::list<game_log_message_t> m_items;

	Event<game_log_message_t&> add_item;

	Game_log();
	~Game_log();

	void add(game_log_message_t& value);


};

#endif //GAME_LOG_H