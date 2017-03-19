#include "game/GameMap.h"
#include "game/GameObject.h"
#include "game/Application.h"
#include "log.h"
#include "Effect.h"
#include "AI.h"

void Object_manager::update_buff()
{
	Interaction_list* list;
	Interaction_list* e;

	for (auto object = m_items.begin(); object !=m_items.end(); ++object)
	{
		list = static_cast<Interaction_list*>((*object)->get_effect(interaction_e::buff));
		if (list)
		{
			e = list->clone();
			e->apply_effect(*object, nullptr);
			list->on_turn();
		}
		for (auto item = (*object)->m_active_state->m_items.begin(); item != (*object)->m_active_state->m_items.end(); item++)
		{
			if ((*item).second->m_interaction_message_type == interaction_message_type_e::list)
			{
				e = static_cast<Interaction_list*>((*item).second);
				if (e->m_list_type == feature_list_type_e::parameter|| e->m_list_type == feature_list_type_e::parts|| e->m_list_type == feature_list_type_e::tag)
				{
					e->on_turn();
				}
			}
		}
		(*object)->update_interaction();
		(*object)->event_update(VoidEventArgs());
	}
}

void Object_manager::calculate_ai()
{
	for (auto object = m_items.begin(); object != m_items.end(); ++object)
	{
		if ((*object)->m_owner->m_kind == entity_e::cell)
		{
			if (((*object)->m_active_state->m_ai) && ((*object) != Application::instance().m_world->m_player->m_object))
			{
				switch ((*object)->m_active_state->m_ai->m_ai_type)
				{
				case ai_type_e::trap:
				{
					AI_trap obj;
					obj = *static_cast<AI_trap*>((*object)->m_active_state->m_ai);
					obj.m_object = (*object);
					obj.create();
					break;
				}
				case ai_type_e::non_humanoid:
				{
					AI_enemy* obj;
					obj = static_cast<AI_enemy*>((*object)->m_active_state->m_ai);
					obj->m_map = static_cast<MapCell*>((*object)->m_owner)->m_map;
					obj->m_object = (*object);
					obj->create();
					break;
				}
				}
			}
		}
	}
}

void Object_manager::reset_serialization_index()
{
	m_serialization_index = 0;
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		(*item)->reset_serialization_index();
	}
}

void Object_manager::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	size_t s = m_items.size();
	fwrite(&s, sizeof(size_t), 1, file);
	for (auto item = m_items.begin(); item != m_items.end(); ++item)
	{
		Serialization_manager::instance().serialize(*item);
	}
}

void Object_manager::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	GameObject* value;
	for (size_t i = 0; i < s; i++)
	{
		value = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
		m_items.push_back(value);
	}
}

GameMap::GameMap(dimension_t size)
{
	m_dijkstra_map = new Dijkstra_map();
	init(size);
}

GameMap::GameMap()
{
	m_dijkstra_map = new Dijkstra_map();
}

void GameMap::init(dimension_t size)
{
	m_size = size;
	LOG(INFO) << "Карта инициализирована с размером: " <<std::to_string( m_size.w )<< "x" << std::to_string(m_size.h);
	m_items.resize(m_size.h);
	for (int i = 0; i < m_size.h; i++)
	{
		m_items[i].resize(m_size.w);
		for (int j = 0; j < m_size.w; j++)
		{
			m_items[i][j] = new MapCell(j, i, this);
		}
	}
}

//void GameMap::generate_room(void)
//{
//	for(int i=0;i<m_size.h;i++)
//	{
//		for(int j=0;j<m_size.w;j++)
//		{
//			if ((i==0)||(j==0)||(i==m_size.h-1)||(j==m_size.w-1))
//			{
//				m_items[i][j] = new MapCell(j, i,this);
//				GameObject* obj = Application::instance().m_game_object_manager->new_object("wall");
//				add_object(obj, m_items[i][j]);
//			} else {
//				m_items[i][j] = new MapCell(j, i,this);
//				GameObject* obj = Application::instance().m_game_object_manager->new_object("floor");
//				add_object(obj, m_items[i][j]);
//			}
//		}
//	}
//}

void GameMap::generate_room(void)
{
	for (int i = 0; i<m_size.h; i++)
	{
		for (int j = 0; j<m_size.w; j++)
		{
			if ((i == 0) || (j == 0) || (i == m_size.h - 1) || (j == m_size.w - 1))
			{
				if ((j == 0)|| (j == m_size.w - 1))
				{
					GameObject* obj = Application::instance().m_game_object_manager->new_object("wall");
					add_object(obj, m_items[i][j]);
				}
				if ((i == 0)||(i == m_size.h - 1))
				{
					GameObject* obj = Application::instance().m_game_object_manager->new_object("wall");
					add_object(obj, m_items[i][j]);
				}
			}
			else {
				GameObject* obj = Application::instance().m_game_object_manager->new_object("floor");
				add_object(obj, m_items[i][j]);
			}
		}
	}

	int light_source_count = 0;
	for (int i = 1; i<m_size.h; i=i+20)
	{
		for (int j = 1; j<m_size.w; j=j+20)
		{
			GameObject* obj = Application::instance().m_game_object_manager->new_object("torch");
			add_to_map(obj, m_items[i][j]);
			light_source_count += 1;
		}
	}
	LOG(INFO) << " -= Всего источников света: "+std::to_string(light_source_count) << "=-";

	//GameObject* obj = Application::instance().m_game_object_manager->new_object("torch");
	//*obj->m_active_state->m_light = light_t(80, 0, 0);
	//add_to_map(obj, m_items[15][15]);
	//obj = Application::instance().m_game_object_manager->new_object("torch");
	//*obj->m_active_state->m_light = light_t(0, 80, 0);
	//add_to_map(obj, m_items[15][35]);
	//obj = Application::instance().m_game_object_manager->new_object("torch");
	//*obj->m_active_state->m_light = light_t(0, 0, 80);
	//add_to_map(obj, m_items[35][25]);

}


void GameMap::add_object(GameObject* object, MapCell* cell)
{
	object->m_owner = cell;
	for (int i = 0; i<object->m_active_state->m_size.y; i++)
	{
		for (int j = 0; j<object->m_active_state->m_size.x; j++)
		{
			m_items[cell->y - i][cell->x + j]->add_object(object);
		}
	}
}

void GameMap::remove_object(GameObject* object)
{
	for (int i = 0; i<object->m_active_state->m_size.y; i++)
	{
		for (int j = 0; j<object->m_active_state->m_size.x; j++)
		{
			m_items[object->cell()->y - i][object->cell()->x + j]->m_items.remove(object);
		}
	}
	object->m_owner = nullptr;
}

void GameMap::move_object(GameObject* object,MapCell* cell)
{
	remove_object(object);
	add_object(object, cell);
}

void  GameMap::turn_object(GameObject* object)
{
	MapCell* position = object->cell();
	remove_object(object);
	object->turn();
	add_object(object, position);
}

void GameMap::add_to_map(GameObject* object, MapCell* cell)
{
	add_object(object, cell);
}

void GameMap::generate_level(void)
{
	srand(time(0));
	block_t* block;
	block = new block_t(0, 0, m_size.w - 1, m_size.h - 1);
	divide_block(block, 5, 0);
	fill();
	connect_room();
	add_wall();
	add_doors();
	generate_traps();
	add_lighting();
}


void GameMap::generate_traps()
{

}

bool GameMap::divide_block(block_t* block, int Depth, int Current)
{
	int choise(rand() % 100);
	block_t* sub_block;
	if (choise<50)
	{
		int Range = block->rect.w;
		if (Range<22){ return true; }
		choise = rand() % (Range - 6) + 11;
		if (Range - choise<11)
		{
			choise = 11;
			if (Range - choise - 11<11){ return true; }
		}
		if (Current<Depth)
		{
			sub_block = new block_t(block->rect.x, block->rect.y, choise, block->rect.h);
			if (divide_block(sub_block, Depth, Current + 1))
			{
				m_rooms.push_back(sub_block);
			}
			sub_block = new block_t(block->rect.x + choise, block->rect.y, block->rect.w - choise, block->rect.h);
			if (divide_block(sub_block, Depth, Current + 1))
			{
				m_rooms.push_back(sub_block);
			}
			return false;
		}
	}
	else {
		int Range = block->rect.h;
		if (Range<22){ return true; }
		choise = rand() % (Range - 6) + 11;
		if (Range - choise<11)
		{
			choise = 11;
			if (Range - choise - 11<11){ return true; }
		}
		if (Current<Depth)
		{
			sub_block = new block_t(block->rect.x, block->rect.y, block->rect.w, choise);
			if (divide_block(sub_block, Depth, Current + 1))
			{
				m_rooms.push_back(sub_block);
			}
			sub_block = new block_t(block->rect.x, block->rect.y + choise, block->rect.w, block->rect.h - choise);
			if (divide_block(sub_block, Depth, Current + 1))
			{
				m_rooms.push_back(sub_block);
			}
			return false;
		}
	}
	return true;
}

void GameMap::random_block(block_t* block)
{
	(*block) = block_t(block->rect.x + 1, block->rect.y + 1, block->rect.w - 2, block->rect.h - 2);
	int len;
	len = rand() % block->rect.h;
	if (len > 10)
	{
		block->rect.y = block->rect.y + rand() % (block->rect.h - len);
		block->rect.h = len;
	}
	len = rand() % block->rect.w;
	if (len > 10)
	{
		block->rect.x = block->rect.x + rand() % (block->rect.w - len);
		block->rect.w = len;
	}
}

void GameMap::fill()
{
	block_t* block;
	for (std::list<block_t*>::iterator Current = m_rooms.begin(); Current != m_rooms.end(); ++Current)
	{
		block = (*Current);
		random_block(block);
		for (int i = block->rect.y; i < block->rect.y + block->rect.h + 1; i++)
		{
			for (int j = block->rect.x; j < block->rect.x + block->rect.w + 1; j++)
			{
				if ((i == block->rect.y) || (j == block->rect.x) || (i == block->rect.y + block->rect.h) || (j == block->rect.x + block->rect.w))
				{
				/*	m_items[i][j] = new MapCell(j, i);
					GameObject* obj = Application::instance().m_game_object_manager.new_object("wall");
					add_object(obj, m_items[i][j]);*/
				}
				else {
					m_items[i][j] = new MapCell(j, i, this);
					GameObject* obj = Application::instance().m_game_object_manager->new_object("floor");
					add_to_map(obj, m_items[i][j]);
				}
			}
		}
	}
}

void GameMap::connect_room()
{ 
	block_t* temp;
	int index;
	while (!m_rooms.empty())
	{
		temp = m_rooms.front();
		if (!m_link_rooms.empty())
		{
			index = rand() % m_link_rooms.size();
			auto nx = std::next(m_link_rooms.begin(),index);
			link_room(temp, *nx);
			m_link_rooms.push_front(temp);
			m_rooms.pop_front();
		}
		else {
			index = (rand() % (m_rooms.size() - 1)) + 1;
			auto nx = std::next(m_rooms.begin(), index);
			m_link_rooms.push_front(temp);
			m_link_rooms.push_front(*nx);
			link_room(temp, *nx);
			m_rooms.pop_front();
			m_rooms.remove(*nx);
		}
	}
}

void  GameMap::add_wall()
{ 
	bool empty;
	for (int y = 1; y < m_size.h-1; y++)
	{
		for (int x = 1; x< m_size.w-1; x++)
		{
			if (m_items[y][x]->m_items.empty())
			{
				empty = true;
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						if (!m_items[y+i][x +j]->m_items.empty()&&(!(i==0&&j==0)))
						{
							if (m_items[y + i][x + j]->m_items.front()->m_name == u"floor")
								empty = false;
						}
					}
				}
				if (empty)
				{
					add_to_map(Application::instance().m_game_object_manager->new_object("dirt"), m_items[y][x]);
				}
				else
				{
					add_to_map(Application::instance().m_game_object_manager->new_object("wall"), m_items[y][x]);
				}
			}
		}
	}
}

void GameMap::link_room(block_t* a, block_t* b)
{
	position_t ac = position_t(a->rect.x + a->rect.w / 2, a->rect.y + a->rect.h / 2);
	position_t bc = position_t(b->rect.x + b->rect.w / 2, b->rect.y + b->rect.h / 2);
	int s;
	int e;
	if (ac.x < bc.x)
	{
		s = ac.x;
		e = bc.x;
	} else {
		e = ac.x;
		s = bc.x;
	}
	for (int i = s; i < e+4; i++)
	{
		for (int j = -3; j < 4; j++)
		{
			m_items[ac.y+j][i]->m_items.clear();
			add_to_map(Application::instance().m_game_object_manager->new_object("floor"), m_items[ac.y + j][i]);
		}
	}
	if (ac.y < bc.y)
	{
		s = ac.y;
		e = bc.y;
	}
	else {
		e = ac.y;
		s = bc.y;
	}
	for (int i = s; i < e+4; i++)
	{
		for (int j = -3; j < 4; j++)
		{
			m_items[i][bc.x+j]->m_items.clear();
			add_to_map(Application::instance().m_game_object_manager->new_object("floor"), m_items[i][bc.x + j]);
		}
	}
}

void  GameMap::add_doors()
{
	rectangle_t rect;
	int s;
	bool b;
	for (std::list<block_t*>::iterator Current = m_link_rooms.begin(); Current != m_link_rooms.end(); ++Current)
	{
		rect = (*Current)->rect;
		s = rect.x;
		b = false;
		for (int i = rect.x; i < rect.x + rect.w+1; i++)
		{
			if (m_items[rect.y][i]->m_items.front()->m_name == u"floor") 
			{ 
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (m_items[rect.y][i]->m_items.front()->m_name == u"wall") 
			{
				if (b)
				{
					if (i - s == 7)
					{
						
						GameObject* lever1 = Application::instance().m_game_object_manager->new_object("lever");
						ObjectTag::Activator* l1 = new ObjectTag::Activator();
						lever1->add_effect(interaction_e::tag, l1);
						add_object(lever1, m_items[rect.y - 1][i - 1]);
						GameObject* lever2 = Application::instance().m_game_object_manager->new_object("lever");
						ObjectTag::Activator* l2 = new ObjectTag::Activator();
						lever2->add_effect(interaction_e::tag, l2);
						add_object(lever2, m_items[rect.y + 1][i - 7]);

						l1->m_link.push_front(lever2);
						l2->m_link.push_front(lever1);

						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[rect.y][i - 1]);
						GameObject*  obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[rect.y][i - 2]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[rect.y][i - 3]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[rect.y][i - 4]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[rect.y][i - 5]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[rect.y][i - 6]);
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[rect.y][i - 7]);

						Object_state* state = lever1->get_state(object_state_e::on);
						state->add_effect(interaction_e::tag, l1);
						state = lever2->get_state(object_state_e::on);
						state->add_effect(interaction_e::tag, l2);


					}
				}
				b = false;
			}
		}
		s = rect.x;
		b = false;
		for (int i = rect.x; i < rect.x + rect.w + 1; i++)
		{
			if (m_items[rect.y+rect.h][i]->m_items.front()->m_name == u"floor")
			{
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (m_items[rect.y + rect.h][i]->m_items.front()->m_name == u"wall")
			{
				if (b)
				{
					if (i - s == 7)
					{
						GameObject* lever1 = Application::instance().m_game_object_manager->new_object("lever");
						ObjectTag::Activator* l1 = new ObjectTag::Activator();
						lever1->add_effect(interaction_e::tag, l1);
						add_object(lever1, m_items[rect.y + rect.h - 1][i - 1]);
						GameObject* lever2= Application::instance().m_game_object_manager->new_object("lever");
						ObjectTag::Activator* l2 = new ObjectTag::Activator();
						lever2->add_effect(interaction_e::tag, l2);
						add_object(lever2, m_items[rect.y + rect.h + 1][i - 7]);

						l1->m_link.push_front(lever2);
						l2->m_link.push_front(lever1);

						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[rect.y + rect.h][i - 1]);
						GameObject*  obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[rect.y + rect.h][i - 2]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[rect.y + rect.h][i - 3]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[rect.y + rect.h][i - 4]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[rect.y + rect.h][i - 5]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[rect.y + rect.h][i - 6]);
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[rect.y + rect.h][i - 7]);

						Object_state* state = lever1->get_state(object_state_e::on);
						state->add_effect(interaction_e::tag, l1);
						state = lever2->get_state(object_state_e::on);
						state->add_effect(interaction_e::tag, l2);
					}
				}
				b = false;
			}
		}
		s = rect.y;
		b = false;
		for (int i = rect.y; i < rect.y + rect.h + 1; i++)
		{
			if (m_items[i][rect.x]->m_items.front()->m_name == u"floor")
			{
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (m_items[i][rect.x]->m_items.front()->m_name == u"wall")
			{
				if (b)
				{
					if (i - s == 7)
					{
						GameObject* lever1 = Application::instance().m_game_object_manager->new_object("lever");
						lever1->set_direction(object_direction_e::left);
						ObjectTag::Activator* l1 = new ObjectTag::Activator();
						lever1->add_effect(interaction_e::tag, l1);
						add_object(lever1, m_items[i - 1][rect.x - 1]);
						GameObject* lever2 = Application::instance().m_game_object_manager->new_object("lever");
						lever2->set_direction(object_direction_e::left);
						ObjectTag::Activator* l2 = new ObjectTag::Activator();
						lever2->add_effect(interaction_e::tag, l2);
						add_object(lever2, m_items[i - 7][rect.x + 1]);

						l1->m_link.push_front(lever2);
						l2->m_link.push_front(lever1);

						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[i - 1][rect.x]);

						GameObject* obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[i - 2][rect.x]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[i - 3][rect.x]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[i - 4][rect.x]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[i - 5][rect.x]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[i - 6][rect.x]);
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[i - 7][rect.x]);

						Object_state* state = lever1->get_state(object_state_e::on);
						state->add_effect(interaction_e::tag, l1);
						state = lever2->get_state(object_state_e::on);
						state->add_effect(interaction_e::tag, l2);
					}
				}
				b = false;
			}
		}
		s = rect.y;
		b = false;
		for (int i = rect.y; i < rect.y + rect.h + 1; i++)
		{
			if (m_items[i][rect.x + rect.w]->m_items.front()->m_name == u"floor")
			{
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (m_items[i][rect.x + rect.w]->m_items.front()->m_name == u"wall")
			{
				if (b)
				{
					if (i - s == 7)
					{
						GameObject* lever1 = Application::instance().m_game_object_manager->new_object("lever");
						lever1->set_direction(object_direction_e::left);
						ObjectTag::Activator* l1 = new ObjectTag::Activator();
						lever1->add_effect(interaction_e::tag, l1);
						add_object(lever1, m_items[i - 1][rect.x + rect.w - 1]);
						GameObject* lever2 = Application::instance().m_game_object_manager->new_object("lever");
						lever2->set_direction(object_direction_e::left);
						ObjectTag::Activator* l2 = new ObjectTag::Activator();
						lever2->add_effect(interaction_e::tag, l2);
						add_object(lever2, m_items[i - 7][rect.x + rect.w + 1]);

						l1->m_link.push_front(lever2);
						l2->m_link.push_front(lever1);

						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[i - 1][rect.x + rect.w]);
						GameObject* obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[i - 2][rect.x + rect.w]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[i - 3][rect.x + rect.w]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[i - 4][rect.x + rect.w]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[i - 5][rect.x + rect.w]);
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, m_items[i - 6][rect.x + rect.w]);
						add_object(Application::instance().m_game_object_manager->new_object("wall"), m_items[i - 7][rect.x + rect.w]);

						Object_state* state = lever1->get_state(object_state_e::on);
						state->add_effect(interaction_e::tag, l1);
						state = lever2->get_state(object_state_e::on);
						state->add_effect(interaction_e::tag, l2);
					}
				}
				b = false;
			}
		}
	}
}

void  GameMap::blur_lighting()
{
	light_t l;
	float Gauss[] = { 0.44198F, 0.27901F };
	//float Gauss[] = { 0.39894228F, 0.241970725F, 0.053990967F };
	//float Gauss[] = { 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 };
	for (int y = 0; y < m_size.h; y++)
	{
		for (int x = 0; x < m_size.w; x++)
		{
			l = light_t();
			for (int i = -1; i < 2; i++)
			{
				int mx = x + i;
				int my = y;
				if (mx < 0){ mx = 0; }
				if (mx > m_size.w-1){ mx = m_size.w-1; }
				if (my < 0){ my = 0; }
				if (my > m_size.h-1){ my = m_size.h-1; }
				l.R = l.R + m_items[my][mx]->m_light.R*Gauss[abs(i)];
				l.G = l.G + m_items[my][mx]->m_light.G*Gauss[abs(i)];
				l.B = l.B + m_items[my][mx]->m_light.B*Gauss[abs(i)];
			}
			m_items[y][x]->m_light_blur = l;
		}
	}
	for (int y = 0; y < m_size.h; y++)
	{
		for (int x = 0; x < m_size.w; x++)
		{
			l = light_t();
			for (int i = -1; i < 2; i++)
			{
				int mx = x;
				int my = y + i;
				if (mx < 0) { mx = 0; }
				if (mx > m_size.w - 1) { mx = m_size.w - 1; }
				if (my < 0) { my = 0; }
				if (my > m_size.h - 1) { my = m_size.h - 1; }
				l.R = l.R + m_items[my][mx]->m_light_blur.R*Gauss[abs(i)];
				l.G = l.G + m_items[my][mx]->m_light_blur.G*Gauss[abs(i)];
				l.B = l.B + m_items[my][mx]->m_light_blur.B*Gauss[abs(i)];
			}
			m_items[y][x]->m_light = l;
		}
	}
}


//bool GameMap::line2(int x1, int y1, int x2, int y2)
//{
//	int e = 0;
//	int dx = abs(x2 - x1);
//	int dy = abs(y2 - y1);
//	int x;
//	int y;
//	int k;
//	if (dx >= dy)
//	{
//		y = y1;
//		k = (y1 < y2 ? 1 : -1);
//		if (x1 < x2)
//		{
//			for (x = x1; x < x2+1; x++)
//			{
//				if (m_barrier_map[y][x]){ return false; }
//				e = e + dy;
//				if (2 * e >= dx)
//				{
//					y = y + k;
//					e = e - dx;
//				}
//			}
//		}
//		else
//		{
//			for (x = x1; x > x2-1; x--)
//			{
//				if (m_barrier_map[y][x]){ return false; }
//				e = e + dy;
//				if (2 * e >= dx)
//				{
//					y = y + k;
//					e = e - dx;
//				}
//			}
//		}
//	}
//	else
//	{
//		x = x1;
//		k = (x1 < x2 ? 1 : -1);
//		if (y1 <= y2)
//		{
//			for (y = y1; y < y2+1; y++)
//			{
//				if (m_barrier_map[y][x]){ return false; }
//				e = e + dx;
//				if (2 * e >= dy)
//				{
//					x = x + k;
//					e = e - dy;
//				}
//			}
//		}
//		else
//		{
//
//			for (y = y1; y > y2-1; y--)
//			{
//				if (m_barrier_map[y][x]){ return false; }
//				e = e + dx;
//				if (2 * e >= dy)
//				{
//					x = x + k;
//					e = e - dy;
//				}
//			}
//		}
//	}
//	return true;
//}

void GameMap::bresenham_line(MapCell* a, MapCell* b, std::function<void(MapCell*)> f)
{
	int e = 0;
	int dx = abs(b->x - a->x);
	int dy = abs(b->y - a->y);
	int x;
	int y;
	int k;
	if (dx >= dy)
	{
		y = a->y;
		k = (a->y < b->y ? 1 : -1);
		if (a->x < b->x)
		{
			for (x = a->x; x < b->x + 1; x++)
			{
				f(m_items[y][x]);
				e = e + dy;
				if (2 * e >= dx)
				{
					y = y + k;
					e = e - dx;
				}
			}
		}
		else
		{
			for (x = a->x; x > b->x - 1; x--)
			{
				f(m_items[y][x]);
				e = e + dy;
				if (2 * e >= dx)
				{
					y = y + k;
					e = e - dx;
				}
			}
		}
	}
	else
	{
		x = a->x;
		k = (a->x < b->x ? 1 : -1);
		if (a->y < b->y)
		{
			for (y = a->y; y < b->y + 1; y++)
			{
				f(m_items[y][x]);
				e = e + dx;
				if (2 * e >= dy)
				{
					x = x + k;
					e = e - dy;
				}
			}
		}
		else
		{
			for (y = a->y; y > b->y - 1; y--)
			{
				f(m_items[y][x]);
				e = e + dx;
				if (2 * e >= dy)
				{
					x = x + k;
					e = e - dy;
				}
			}
		}
	}
}

MapCell* GameMap::bresenham_line2(MapCell* a, MapCell* b, std::function<bool(MapCell*)> f)
{
	int e = 0;
	int dx = abs(b->x - a->x);
	int dy = abs(b->y - a->y);
	int x;
	int y;
	int k;
	if (dx >= dy)
	{
		y = a->y;
		k = (a->y < b->y ? 1 : -1);
		if (a->x < b->x)
		{
			for (x = a->x; x < b->x + 1; x++)
			{
				if (f(m_items[y][x]))
				{
					return m_items[y][x];
				}
				e = e + dy;
				if (2 * e >= dx)
				{
					y = y + k;
					e = e - dx;
				}
			}
			return m_items[y][x - 1];
		}
		else
		{
			for (x = a->x; x > b->x - 1; x--)
			{
				if (f(m_items[y][x]))
				{
					return m_items[y][x];
				}
				e = e + dy;
				if (2 * e >= dx)
				{
					y = y + k;
					e = e - dx;
				}
			}
			return m_items[y][x + 1];
		}
	}
	else
	{
		x = a->x;
		k = (a->x < b->x ? 1 : -1);
		if (a->y < b->y)
		{
			for (y = a->y; y < b->y + 1; y++)
			{
				if (f(m_items[y][x]))
				{
					return m_items[y][x];
				}
				e = e + dx;
				if (2 * e >= dy)
				{
					x = x + k;
					e = e - dy;
				}
			}
			return m_items[y - 1][x];
		}
		else
		{
			for (y = a->y; y > b->y - 1; y--)
			{
				if (f(m_items[y][x]))
				{
					return m_items[y][x];
				}
				e = e + dx;
				if (2 * e >= dy)
				{
					x = x + k;
					e = e - dy;
				}
			}
			return m_items[y + 1][x];
		}
	}
}

MapCell* GameMap::bresenham_line2(MapCell* a, MapCell* b, Parameter* p, std::function<bool(Parameter*, MapCell*)> f)
{
	int e = 0;
	int dx = abs(b->x - a->x);
	int dy = abs(b->y - a->y);
	int x;
	int y;
	int k;
	if (dx >= dy)
	{
		y = a->y;
		k = (a->y < b->y ? 1 : -1);
		if (a->x < b->x)
		{
			for (x = a->x; x < b->x + 1; x++)
			{
				if (f(p,m_items[y][x]))
				{
					return m_items[y][x];
				}
				e = e + dy;
				if (2 * e >= dx)
				{
					y = y + k;
					e = e - dx;
				}
			}
			return m_items[y][x - 1];
		}
		else
		{
			for (x = a->x; x > b->x - 1; x--)
			{
				if (f(p,m_items[y][x]))
				{
					return m_items[y][x];
				}
				e = e + dy;
				if (2 * e >= dx)
				{
					y = y + k;
					e = e - dx;
				}
			}
			return m_items[y][x + 1];
		}
	}
	else
	{
		x = a->x;
		k = (a->x < b->x ? 1 : -1);
		if (a->y < b->y)
		{
			for (y = a->y; y < b->y + 1; y++)
			{
				if (f(p,m_items[y][x]))
				{
					return m_items[y][x];
				}
				e = e + dx;
				if (2 * e >= dy)
				{
					x = x + k;
					e = e - dy;
				}
			}
			return m_items[y - 1][x];
		}
		else
		{
			for (y = a->y; y > b->y - 1; y--)
			{
				if (f(p,m_items[y][x]))
				{
					return m_items[y][x];
				}
				e = e + dx;
				if (2 * e >= dy)
				{
					x = x + k;
					e = e - dy;
				}
			}
			return m_items[y + 1][x];
		}
	}
}


void GameMap::add_lighting()
{
	block_t* block;
	for (std::list<block_t*>::iterator Current = m_link_rooms.begin(); Current != m_link_rooms.end(); ++Current)
	{
		block = (*Current);
		//int tk;
		//std::string torch_kind[] = { "torch", "red torch", "green torch", "blue torch" };
		//tk = rand() % 1;
		add_to_map(Application::instance().m_game_object_manager->new_object("torch"), m_items[block->rect.y + 1][block->rect.x + 1]);
		//tk = rand() % 1;
		add_to_map(Application::instance().m_game_object_manager->new_object("torch"), m_items[block->rect.y + 1][block->rect.x + block->rect.w - 1]);
		//tk = rand() % 1;
		add_to_map(Application::instance().m_game_object_manager->new_object("torch"), m_items[block->rect.y + block->rect.h - 1][block->rect.x + block->rect.w - 1]);
		//tk = rand() % 1;
		add_to_map(Application::instance().m_game_object_manager->new_object("torch"), m_items[block->rect.y + block->rect.h - 1][block->rect.x + 1]);
		int j=0;
		for (int i = block->rect.x + 1; i < block->rect.x + block->rect.w; i++)
		{
			if (j == 30)
			{
				//tk = rand() % 4;
				add_to_map(Application::instance().m_game_object_manager->new_object("torch"), m_items[block->rect.y + 1][i]);
				//tk = rand() % 4;
				add_to_map(Application::instance().m_game_object_manager->new_object("torch"), m_items[block->rect.y + block->rect.h - 1][i]);
				j = -1;
			}
			j = j + 1;
		}
		j = 0;
		for (int i = block->rect.y + 1; i < block->rect.y + block->rect.h; i++)
		{
			if (j == 30)
			{
				//tk = rand() % 4;
				add_to_map(Application::instance().m_game_object_manager->new_object("torch"), m_items[i][block->rect.x + 1]);
				//tk = rand() % 4;
				add_to_map(Application::instance().m_game_object_manager->new_object("torch"), m_items[i][block->rect.x + block->rect.w - 1]);
				j = -1;
			}
			j = j + 1;
		}
	}
}

bool GameMap::check(int x, int y)
{
	if (x<0 && x>m_size.h - 1 && y<0 && y>m_size.w - 1) { return false; };
}

//MapCell* GameMap::get_cell(int x, int y)
//{
//	if (x<0 && x>m_size.h - 1 && y<0 && y>m_size.w - 1) { return false; };
//}

void GameMap::reset_serialization_index()
{
	m_serialization_index = 0;
}

void GameMap::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::gamemap;
	fwrite(&t, sizeof(type_e), 1, file);
	size_t s = m_index;
	fwrite(&s, sizeof(std::size_t), 1, file);
	MapCell* cell;
	for (int i = 0; i < m_size.h; i++)
	{
		for (int j = 0; j < m_size.w; j++)
		{
			cell = m_items[i][j];
			fwrite(&cell->m_notable, sizeof(bool), 1, file);
			s = cell->m_items.size();
			fwrite(&s, sizeof(size_t), 1, file);
			for (auto item = cell->m_items.begin(); item != cell->m_items.end(); ++item)
			{
				Serialization_manager::instance().serialize(*item);
			}
		}
	}
}

void GameMap::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	size_t s;
	MapCell* cell;
	for (int i = 0; i < m_size.h; i++)
	{
		for (int j = 0; j < m_size.w; j++)
		{
			cell = m_items[i][j];
			fread(&cell->m_notable, sizeof(bool), 1, file);
			fread(&s, sizeof(size_t), 1, file);
			for (int k = 0; k < s; k++)
			{
				cell->m_items.push_back(dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize()));
			}
		}
	}
}

Game_world::Game_world()
{
	float l;
	for (int y = 0; y < 21; y++)
	{
		for (int x = 0; x< 21; x++)
		{
			l = (float)sqrt(x*x + y*y);
			l = l / 20.0;
			m_coefficient[y][x] = 1 - l;
			if (m_coefficient[y][x] < 0) { m_coefficient[y][x] = 0; }
		}
	}
}

void Game_world::reset_serialization_index()
{
	m_serialization_index = 0;
	m_object_manager.reset_serialization_index();
	for (auto i = m_maps.begin(); i != m_maps.end(); ++i)
	{
		(*i)->reset_serialization_index();
	}
}

void Game_world::save()
{
	FILE* file = Serialization_manager::instance().m_file;
	type_e t = type_e::game_world;
	fwrite(&t, sizeof(type_e), 1, file);
	m_object_manager.save();
	size_t s = m_maps.size();
	fwrite(&s, sizeof(std::size_t), 1, file);
	std::size_t index = 0;
	for (auto m = m_maps.begin(); m != m_maps.end(); ++m)
	{ 
		Serialization_manager::instance().serialize(*m);
	}
	Serialization_manager::instance().serialize(m_player->m_object);
}

void Game_world::load()
{
	FILE* file = Serialization_manager::instance().m_file;
	m_object_manager.load();
	size_t s;
	fread(&s, sizeof(size_t), 1, file);
	GameMap* map;
	for (size_t i = 0; i < s; i++)
	{
		map = dynamic_cast<GameMap*>(Serialization_manager::instance().deserialize());
		m_maps.push_back(map);
	}
	GameObject* object = dynamic_cast<GameObject*>(Serialization_manager::instance().deserialize());
	map = static_cast<MapCell*>(object->m_owner)->m_map;
	m_player = new Player(object, map);
}

void Game_world::calculate_lighting()
{
	int lx;
	int ly;
	int c;
	FOV_light fl;
	light_t m_temp[41][41];

	for (auto m = m_maps.begin(); m != m_maps.end(); ++m)
	{
		GameMap& map = *(*m);
		for (int y = 0; y < map.m_size.h; y++)
		{
			for (int x = 0; x < map.m_size.w; x++)
			{
				map.m_items[y][x]->m_light = light_t();
			}
		}
	}
	for (auto l = m_object_manager.m_items.begin(); l != m_object_manager.m_items.end(); ++l)
	{
		GameObject& object = *(*l);
		if (!object.m_active_state->m_light) { continue; }
		if (object.m_owner->m_kind != entity_e::cell) { continue; }
		GameMap* map = static_cast<MapCell*>(object.m_owner)->m_map;
		fl.calculate(20, *l, map);
		for (int y = 0; y < 41; y++)
		{
			for (int x = 0; x < 41; x++)
			{
				lx = abs(x - 20);
				ly = abs(y - 20);
				c = ((*l)->m_active_state->m_light->R * m_coefficient[ly][lx])*fl.m_map[y][x].damping.R;
				if (c < 0) { c = 0; }
				fl.m_map[y][x].light.R = c;
				c = ((*l)->m_active_state->m_light->G * m_coefficient[ly][lx])*fl.m_map[y][x].damping.G;
				if (c < 0) { c = 0; }
				fl.m_map[y][x].light.G = c;
				c = ((*l)->m_active_state->m_light->B * m_coefficient[ly][lx])*fl.m_map[y][x].damping.B;
				if (c < 0) { c = 0; }
				fl.m_map[y][x].light.B = c;
			}
		}
		for (int y = 0; y < 41; y++)
		{
			ly = (*l)->cell()->y + y - 20;
			if (!((ly < 0) || (ly > map->m_size.h - 1)))
			{
				for (int x = 0; x < 41; x++)
				{
					lx = (*l)->cell()->x + x - 20;
					if (!((lx < 0) || (lx > map->m_size.w - 1)))
					{
						map->m_items[ly][lx]->m_light.R += fl.m_map[y][x].light.R;
						map->m_items[ly][lx]->m_light.G += fl.m_map[y][x].light.G;
						map->m_items[ly][lx]->m_light.B += fl.m_map[y][x].light.B;
					}
				}
			}
		}
	}
	for (auto m = m_maps.begin(); m != m_maps.end(); ++m)
	{
		(*m)->blur_lighting();
	}
}