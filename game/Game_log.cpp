#include "Game_log.h"

Game_log::Game_log()
{
}

Game_log::~Game_log()
{
}

void Game_log::add(game_log_message_t& value)
{
	m_items.push_back(value);
	add_item(value);
}