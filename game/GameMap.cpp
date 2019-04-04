#include "game/GameMap.h"
#include "game/GameObject.h"
#include "game/Application.h"
#include "Effect.h"
#include "AI.h"

void Object_manager::update_buff()
{
	Interaction_list* list;
	Interaction_list* e;
	Visitor_onturn_updater visitor;

	for (auto object = m_items.begin(); object !=m_items.end(); ++object)
	{
		list = static_cast<Interaction_list*>(object->get_effect(interaction_e::buff));
		if (list)
		{
			e = list->clone();
			e->apply_effect(&(*object), nullptr);
			visitor.result = false;
			list->apply_visitor(visitor);
		}
		for (auto item = object->m_active_state->m_items.begin(); item != object->m_active_state->m_items.end(); ++item)
		{
			if ((*item).second->m_interaction_message_type == interaction_message_type_e::list)
			{
				e = static_cast<Interaction_list*>((*item).second);
				if (e->m_list_type == feature_list_type_e::parameter|| e->m_list_type == feature_list_type_e::parts|| e->m_list_type == feature_list_type_e::tag)
				{
					visitor.result = false;
					e->apply_visitor(visitor);
				}
			}
		}
		object->update_interaction();
		object->event_update(VoidEventArgs());
	}
}

void Object_manager::calculate_ai()
{
	for (auto object = m_items.begin(); object != m_items.end(); ++object)
	{
		if (object->m_owner->m_kind == entity_e::cell)
		{
			if ((object->m_active_state->m_ai) && (&(*object) != Application::instance().m_world->m_player->m_object))
			{
				switch (object->m_active_state->m_ai->m_ai_type)
				{
				case ai_type_e::trap:
				{
					AI_trap obj;
					obj = *static_cast<AI_trap*>(object->m_active_state->m_ai);
					obj.m_object = &*object;
					obj.create();
					break;
				}
				case ai_type_e::non_humanoid:
				{
					AI_enemy* obj;
					obj = static_cast<AI_enemy*>(object->m_active_state->m_ai);
					obj->m_map = static_cast<MapCell*>(object->m_owner)->m_map;
					obj->m_object = &*object;
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
		item->reset_serialization_index();
	}
}

Packer_generic& Object_manager::get_packer()
{
	return Packer<Object_manager>::Instance();
}


std::u16string GameMap::vector_mapcell_to_json(std::vector<MapCell>& value, Parser_context& context)
{
	std::u16string result = u"[";
	for (auto element : (value))
	{
		if (result != u"[") { result += u","; }
		result += Parser::to_json<std::list<GameObject*>>(element.m_items,context);
	}
	result += u"]";
	return result;
}

void GameMap::vector_mapcell_from_json(std::u16string value, std::vector<MapCell>& prop, Parser_context& context)
{
	std::u16string temp = value;
	scheme_list_t* s = Parser::read_array(temp);
	if (s)
	{
		std::size_t i = 0;
		prop.resize(s->size());
		for (auto element : (*s))
		{
			Parser::from_json<std::list<GameObject*>>(element, prop[i].m_items,context);
			i += 1;
		}
	}
	delete s;
}

std::string GameMap::vector_mapcell_to_binary(std::vector<MapCell>& value, Parser_context& context)
{
	std::size_t s = value.size();
	std::string result = std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t));
	for (auto element : (value))
	{
		result += Parser::to_binary<std::list<GameObject*>>(element.m_items,context);
	}
	return result;
}

void GameMap::vector_mapcell_from_binary(const std::string& value, std::vector<MapCell>& prop, std::size_t& pos, Parser_context& context)
{
	std::size_t s = *reinterpret_cast<const std::size_t*>(&value[pos]);
	pos += sizeof(std::size_t);
	prop.resize(s);
	for (std::size_t i = 0; i < s; ++i)
	{
		Parser::from_binary<std::list<GameObject*>>(value, prop[i].m_items, pos,context);
	}
}

GameMap::GameMap(dimension3_t size)
{
	m_dijkstra_map = new Dijkstra_map();
	init(size);
}

GameMap::GameMap()
{
	m_dijkstra_map = new Dijkstra_map();
}

MapCell& GameMap::get(int z,int y, int x)
{
	return m_items[z*(m_size.dy*m_size.dx) + m_size.dy*y + x];
}

void GameMap::init(dimension3_t size)
{
	m_size = size;
	Logger::Instance().info("Карта инициализирована с размером: " + std::to_string(m_size.dx) < +"x" + std::to_string(m_size.dy) + "x" + std::to_string(m_size.dz));
	m_items.resize(m_size.dz*m_size.dy*m_size.dx);
	int i = 0;
	for (int z = 0; z < m_size.dz; ++z)
	{
		for (int y = 0; y < m_size.dy; ++y)
		{
			for (int x = 0; x < m_size.dx; ++x)
			{
				MapCell& m = m_items[i];
				m.x = x;
				m.y = y;
				m.z = z;
				m.m_map = this;
				++i;
			}
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

void GameMap::generate_room()
{
	for (int z = 0; z < 6; ++z)
	{
		for (int y = 0; y < m_size.dy; y++)
		{
			for (int x = 0; x < m_size.dx; x++)
			{
				if(z==0)
				{
					GameObject* obj = Application::instance().m_game_object_manager->new_object("floor");
					add_object(obj, get(z, y, x));
				}
				else {
					if ((y == 0) || (x == 0) || (y == m_size.dy - 1) || (x == m_size.dx - 1))
					{
						if ((x == 0) || (x == m_size.dx - 1) || (y == 0) || (y == m_size.dy - 1))
						{
							GameObject* obj = Application::instance().m_game_object_manager->new_object("wall");
							add_object(obj, get(z, y, x));
						}
					}
				}
			}
		}
	}

	int light_source_count = 0;
	for (int y = 1; y < m_size.dy; y = y + 50)
	{
		for (int x = 1; x < m_size.dx; x = x + 50)
		{
			GameObject* obj = Application::instance().m_game_object_manager->new_object("torch");
			add_to_map(obj, get(1, y, x));
			light_source_count += 1;
		}
	}

	Logger::Instance().info (" -= Всего источников света: " + std::to_string(light_source_count) + "=-");

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


void GameMap::add_object(GameObject* object, MapCell& cell)
{
	object->m_owner = &cell;
	for (int z = 0; z<object->m_active_state->m_size.dz; ++z)
	{
		for (int y = 0; y<object->m_active_state->m_size.dy; ++y)
		{
			for (int x = 0; x<object->m_active_state->m_size.dx; ++x)
			{
				get(cell.z + z, cell.y - y, cell.x + x).add_object(object);
			}
		}
	}
}

void GameMap::remove_object(GameObject* object)
{
	MapCell& cell = *object->cell();
	for (int z = 0; z<object->m_active_state->m_size.dz; z++)
	{
		for (int y = 0; y<object->m_active_state->m_size.dy; y++)
		{
			for (int x = 0; x<object->m_active_state->m_size.dx; x++)
			{
				get(cell.z + z, cell.y - y, cell.x + x).m_items.remove(object);
			}
		}
	}
	object->m_owner = nullptr;
}

void GameMap::move_object(GameObject* object, MapCell& cell)
{
	remove_object(object);
	add_object(object, cell);
}

void  GameMap::turn_object(GameObject* object)
{
	MapCell* position = object->cell();
	remove_object(object);
	object->turn();
	add_object(object, *position);
}

void GameMap::add_to_map(GameObject* object, MapCell& cell)
{
	add_object(object, cell);
}

void GameMap::generate_level(void)
{
	/*srand(time(0));
	block_t* block;
	block = new block_t(0, 0, m_size.w - 1, m_size.h - 1);
	divide_block(block, 5, 0);
	fill();
	connect_room();
	add_wall();
	add_doors();
	generate_traps();
	add_lighting();*/
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
	//block_t* block;
	//for (std::list<block_t*>::iterator Current = m_rooms.begin(); Current != m_rooms.end(); ++Current)
	//{
	//	block = (*Current);
	//	random_block(block);
	//	for (int i = block->rect.y; i < block->rect.y + block->rect.h + 1; i++)
	//	{
	//		for (int j = block->rect.x; j < block->rect.x + block->rect.w + 1; j++)
	//		{
	//			if ((i == block->rect.y) || (j == block->rect.x) || (i == block->rect.y + block->rect.h) || (j == block->rect.x + block->rect.w))
	//			{
	//			/*	m_items[i][j] = new MapCell(j, i);
	//				GameObject* obj = Application::instance().m_game_object_manager.new_object("wall");
	//				add_object(obj, m_items[i][j]);*/
	//			}
	//			else {
	//				MapCell& m = get(i, j);
	//				m = MapCell(j, i, this);
	//				GameObject* obj = Application::instance().m_game_object_manager->new_object("floor");
	//				add_to_map(obj,m);
	//			}
	//		}
	//	}
	//}
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
	/*bool empty;
	for (int y = 1; y < m_size.h-1; y++)
	{
		for (int x = 1; x< m_size.w-1; x++)
		{
			MapCell& c = get(y, x);
			if (c.m_items.empty())
			{
				empty = true;
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						MapCell& nc = get(y + i,x + j);
						if (!nc.m_items.empty()&&(!(i==0&&j==0)))
						{
							if (nc.m_items.front()->m_name == u"floor")
								empty = false;
						}
					}
				}
				if (empty)
				{
					add_to_map(Application::instance().m_game_object_manager->new_object("dirt"), c);
				}
				else
				{
					add_to_map(Application::instance().m_game_object_manager->new_object("wall"),c);
				}
			}
		}
	}*/
}

void GameMap::link_room(block_t* a, block_t* b)
{
	/*position_t ac = position_t(a->rect.x + a->rect.w / 2, a->rect.y + a->rect.h / 2);
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
			MapCell& c = get(ac.y + j,i);
			c.m_items.clear();
			add_to_map(Application::instance().m_game_object_manager->new_object("floor"), c);
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
			MapCell& c = get(i,bc.x + j);
			c.m_items.clear();
			add_to_map(Application::instance().m_game_object_manager->new_object("floor"), c);
		}
	}*/
}

void  GameMap::add_doors()
{
	/*rectangle_t rect;
	int s;
	bool b;
	for (std::list<block_t*>::iterator Current = m_link_rooms.begin(); Current != m_link_rooms.end(); ++Current)
	{
		rect = (*Current)->rect;
		s = rect.x;
		b = false;
		for (int i = rect.x; i < rect.x + rect.w+1; i++)
		{
			MapCell& c = get(rect.y,i);
			if (c.m_items.front()->m_name == u"floor") 
			{ 
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (c.m_items.front()->m_name == u"wall") 
			{
				if (b)
				{
					if (i - s == 7)
					{
						
						GameObject* lever1 = Application::instance().m_game_object_manager->new_object("lever");
						ObjectTag::Activator* l1 = new ObjectTag::Activator();
						lever1->add_effect(interaction_e::tag, l1);
						add_object(lever1, get(rect.y - 1,i - 1));
						GameObject* lever2 = Application::instance().m_game_object_manager->new_object("lever");
						ObjectTag::Activator* l2 = new ObjectTag::Activator();
						lever2->add_effect(interaction_e::tag, l2);
						add_object(lever2, get(rect.y + 1,i - 7));

						l1->m_link.push_front(lever2);
						l2->m_link.push_front(lever1);

						add_object(Application::instance().m_game_object_manager->new_object("wall"),get(rect.y,i - 1));
						GameObject*  obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(rect.y,i - 2));
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(rect.y,i - 3));
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(rect.y,i - 4));
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj,get(rect.y,i - 5));
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(rect.y,i - 6));
						add_object(Application::instance().m_game_object_manager->new_object("wall"), get(rect.y,i - 7));

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
			if (get(rect.y+rect.h,i).m_items.front()->m_name == u"floor")
			{
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (get(rect.y + rect.h,i).m_items.front()->m_name == u"wall")
			{
				if (b)
				{
					if (i - s == 7)
					{
						GameObject* lever1 = Application::instance().m_game_object_manager->new_object("lever");
						ObjectTag::Activator* l1 = new ObjectTag::Activator();
						lever1->add_effect(interaction_e::tag, l1);
						add_object(lever1, get(rect.y + rect.h - 1,i - 1));
						GameObject* lever2= Application::instance().m_game_object_manager->new_object("lever");
						ObjectTag::Activator* l2 = new ObjectTag::Activator();
						lever2->add_effect(interaction_e::tag, l2);
						add_object(lever2, get(rect.y + rect.h + 1,i - 7));

						l1->m_link.push_front(lever2);
						l2->m_link.push_front(lever1);

						add_object(Application::instance().m_game_object_manager->new_object("wall"), get(rect.y + rect.h,i - 1));
						GameObject*  obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(rect.y + rect.h,i - 2));
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(rect.y + rect.h,i - 3));
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(rect.y + rect.h,i - 4));
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(rect.y + rect.h,i - 5));
						obj = Application::instance().m_game_object_manager->new_object("door");
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(rect.y + rect.h,i - 6));
						add_object(Application::instance().m_game_object_manager->new_object("wall"), get(rect.y + rect.h,i - 7));

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
			if (get(i,rect.x).m_items.front()->m_name == u"floor")
			{
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (get(i,rect.x).m_items.front()->m_name == u"wall")
			{
				if (b)
				{
					if (i - s == 7)
					{
						GameObject* lever1 = Application::instance().m_game_object_manager->new_object("lever");
						lever1->set_direction(object_direction_e::left);
						ObjectTag::Activator* l1 = new ObjectTag::Activator();
						lever1->add_effect(interaction_e::tag, l1);
						add_object(lever1, get(i - 1,rect.x - 1));
						GameObject* lever2 = Application::instance().m_game_object_manager->new_object("lever");
						lever2->set_direction(object_direction_e::left);
						ObjectTag::Activator* l2 = new ObjectTag::Activator();
						lever2->add_effect(interaction_e::tag, l2);
						add_object(lever2, get(i - 7,rect.x + 1));

						l1->m_link.push_front(lever2);
						l2->m_link.push_front(lever1);

						add_object(Application::instance().m_game_object_manager->new_object("wall"),get(i - 1,rect.x));

						GameObject* obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(i - 2,rect.x));
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(i - 3,rect.x));
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(i - 4,rect.x));
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(i - 5,rect.x));
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(i - 6,rect.x));
						add_object(Application::instance().m_game_object_manager->new_object("wall"), get(i - 7,rect.x));

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
			if (get(i,rect.x + rect.w).m_items.front()->m_name == u"floor")
			{
				if (!b)
				{
					b = true;
					s = i;
				}
			}
			if (get(i,rect.x + rect.w).m_items.front()->m_name == u"wall")
			{
				if (b)
				{
					if (i - s == 7)
					{
						GameObject* lever1 = Application::instance().m_game_object_manager->new_object("lever");
						lever1->set_direction(object_direction_e::left);
						ObjectTag::Activator* l1 = new ObjectTag::Activator();
						lever1->add_effect(interaction_e::tag, l1);
						add_object(lever1, get(i - 1,rect.x + rect.w - 1));
						GameObject* lever2 = Application::instance().m_game_object_manager->new_object("lever");
						lever2->set_direction(object_direction_e::left);
						ObjectTag::Activator* l2 = new ObjectTag::Activator();
						lever2->add_effect(interaction_e::tag, l2);
						add_object(lever2, get(i - 7,rect.x + rect.w + 1));

						l1->m_link.push_front(lever2);
						l2->m_link.push_front(lever1);

						add_object(Application::instance().m_game_object_manager->new_object("wall"), get(i - 1,rect.x + rect.w));
						GameObject* obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(i - 2,rect.x + rect.w));
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(i - 3,rect.x + rect.w));
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(i - 4,rect.x + rect.w));
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(i - 5,rect.x + rect.w));
						obj = Application::instance().m_game_object_manager->new_object("door");
						obj->set_direction(object_direction_e::left);
						l1->m_link.push_front(obj);
						l2->m_link.push_front(obj);
						add_object(obj, get(i - 6,rect.x + rect.w));
						add_object(Application::instance().m_game_object_manager->new_object("wall"), get(i - 7,rect.x + rect.w));

						Object_state* state = lever1->get_state(object_state_e::on);
						state->add_effect(interaction_e::tag, l1);
						state = lever2->get_state(object_state_e::on);
						state->add_effect(interaction_e::tag, l2);
					}
				}
				b = false;
			}
		}
	}*/
}

void  GameMap::blur_lighting()
{
	//light_t l;
	//float Gauss[] = { 0.44198F, 0.27901F };
	////float Gauss[] = { 0.39894228F, 0.241970725F, 0.053990967F };
	////float Gauss[] = { 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 };
	//for (int y = 0; y < m_size.h; y++)
	//{
	//	for (int x = 0; x < m_size.w; x++)
	//	{
	//		l = light_t();
	//		for (int i = -1; i < 2; i++)
	//		{
	//			int mx = x + i;
	//			int my = y;
	//			if (mx < 0){ mx = 0; }
	//			if (mx > m_size.w-1){ mx = m_size.w-1; }
	//			if (my < 0){ my = 0; }
	//			if (my > m_size.h-1){ my = m_size.h-1; }
	//			l.R = l.R + get(my,mx).m_light.R*Gauss[abs(i)];
	//			l.G = l.G + get(my, mx).m_light.G*Gauss[abs(i)];
	//			l.B = l.B + get(my, mx).m_light.B*Gauss[abs(i)];
	//		}
	//		get(y,x).m_light_blur = l;
	//	}
	//}
	//for (int y = 0; y < m_size.h; y++)
	//{
	//	for (int x = 0; x < m_size.w; x++)
	//	{
	//		l = light_t();
	//		for (int i = -1; i < 2; i++)
	//		{
	//			int mx = x;
	//			int my = y + i;
	//			if (mx < 0) { mx = 0; }
	//			if (mx > m_size.w - 1) { mx = m_size.w - 1; }
	//			if (my < 0) { my = 0; }
	//			if (my > m_size.h - 1) { my = m_size.h - 1; }
	//			l.R = l.R + get(my, mx).m_light_blur.R*Gauss[abs(i)];
	//			l.G = l.G + get(my, mx).m_light_blur.G*Gauss[abs(i)];
	//			l.B = l.B + get(my, mx).m_light_blur.B*Gauss[abs(i)];
	//		}
	//		get(y, x).m_light = l;
	//	}
	//}
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

//TODO Адаптировать для 3D
//void GameMap::bresenham_line(MapCell* a, MapCell* b, std::function<void(MapCell*)> f)
//{
//	int e = 0;
//	int dx = abs(b->x - a->x);
//	int dy = abs(b->y - a->y);
//	int x;
//	int y;
//	int k;
//	if (dx >= dy)
//	{
//		y = a->y;
//		k = (a->y < b->y ? 1 : -1);
//		if (a->x < b->x)
//		{
//			for (x = a->x; x < b->x + 1; x++)
//			{
//				f(&get(y,x));
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
//			for (x = a->x; x > b->x - 1; x--)
//			{
//				f(&get(y,x));
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
//		x = a->x;
//		k = (a->x < b->x ? 1 : -1);
//		if (a->y < b->y)
//		{
//			for (y = a->y; y < b->y + 1; y++)
//			{
//				f(&get(y,x));
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
//			for (y = a->y; y > b->y - 1; y--)
//			{
//				f(&get(y, x));
//				e = e + dx;
//				if (2 * e >= dy)
//				{
//					x = x + k;
//					e = e - dy;
//				}
//			}
//		}
//	}
//}

void GameMap::bresenham_line(MapCell* a, MapCell* b, std::function<void(MapCell*)> f)
{
	if (a == b)
	{
		f(a); 
		return;
	}
	float dx = b->x - a->x;
	float dy = b->y - a->y;
	float dz = b->z - a->z;
	float max = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	max = max > abs(dz) ? max : abs(dz);
	dx /= max;
	dy /= max;
	dz /= max;
	float x = a->x;
	float y = a->y;
	float z = a->z;
	for (float i = 0.0; i < max + 1.0; i = i + 1)
	{
		f(&get(z, y, x));
		x += dx;
		y += dy;
		z += dz;
	}
}

//TODO Адаптировать для 3D
//MapCell* GameMap::bresenham_line2(MapCell* a, MapCell* b, std::function<bool(MapCell*)> f)
//{
//	int e = 0;
//	int dx = abs(b->x - a->x);
//	int dy = abs(b->y - a->y);
//	int x;
//	int y;
//	int k;
//	if (dx >= dy)
//	{
//		y = a->y;
//		k = (a->y < b->y ? 1 : -1);
//		if (a->x < b->x)
//		{
//			for (x = a->x; x < b->x + 1; x++)
//			{
//				if (f(&get(y, x)))
//				{
//					return &get(y, x);
//				}
//				e = e + dy;
//				if (2 * e >= dx)
//				{
//					y = y + k;
//					e = e - dx;
//				}
//			}
//			return &get(y,x - 1);
//		}
//		else
//		{
//			for (x = a->x; x > b->x - 1; x--)
//			{
//				if (f(&get(y, x)))
//				{
//					return &get(y, x);
//				}
//				e = e + dy;
//				if (2 * e >= dx)
//				{
//					y = y + k;
//					e = e - dx;
//				}
//			}
//			return &get(y,x + 1);
//		}
//	}
//	else
//	{
//		x = a->x;
//		k = (a->x < b->x ? 1 : -1);
//		if (a->y < b->y)
//		{
//			for (y = a->y; y < b->y + 1; y++)
//			{
//				if (f(&get(y, x)))
//				{
//					return &get(y, x);
//				}
//				e = e + dx;
//				if (2 * e >= dy)
//				{
//					x = x + k;
//					e = e - dy;
//				}
//			}
//			return &get(y - 1,x);
//		}
//		else
//		{
//			for (y = a->y; y > b->y - 1; y--)
//			{
//				if (f(&get(y, x)))
//				{
//					return &get(y, x);
//				}
//				e = e + dx;
//				if (2 * e >= dy)
//				{
//					x = x + k;
//					e = e - dy;
//				}
//			}
//			return &get(y + 1,x);
//		}
//	}
//}

//TODO Адаптировать для 3D
//MapCell* GameMap::bresenham_line2(MapCell* a, MapCell* b, Parameter* p, std::function<bool(Parameter*, MapCell*)> f)
//{
//	int e = 0;
//	int dx = abs(b->x - a->x);
//	int dy = abs(b->y - a->y);
//	int x;
//	int y;
//	int k;
//	if (dx >= dy)
//	{
//		y = a->y;
//		k = (a->y < b->y ? 1 : -1);
//		if (a->x < b->x)
//		{
//			for (x = a->x; x < b->x + 1; x++)
//			{
//				if (f(p, &get(y, x)))
//				{
//					return &get(y, x);
//				}
//				e = e + dy;
//				if (2 * e >= dx)
//				{
//					y = y + k;
//					e = e - dx;
//				}
//			}
//			return &get(y, x-1);
//		}
//		else
//		{
//			for (x = a->x; x > b->x - 1; x--)
//			{
//				if (f(p, &get(y, x)))
//				{
//					return &get(y, x);
//				}
//				e = e + dy;
//				if (2 * e >= dx)
//				{
//					y = y + k;
//					e = e - dx;
//				}
//			}
//			return &get(y, x+1);
//		}
//	}
//	else
//	{
//		x = a->x;
//		k = (a->x < b->x ? 1 : -1);
//		if (a->y < b->y)
//		{
//			for (y = a->y; y < b->y + 1; y++)
//			{
//				if (f(p, &get(y, x)))
//				{
//					return &get(y, x);
//				}
//				e = e + dx;
//				if (2 * e >= dy)
//				{
//					x = x + k;
//					e = e - dy;
//				}
//			}
//			return &get(y-1, x);
//		}
//		else
//		{
//			for (y = a->y; y > b->y - 1; y--)
//			{
//				if (f(p, &get(y, x)))
//				{
//					return &get(y, x);
//				}
//				e = e + dx;
//				if (2 * e >= dy)
//				{
//					x = x + k;
//					e = e - dy;
//				}
//			}
//			return &get(y+1, x);
//		}
//	}
//}


void GameMap::add_lighting()
{
	//block_t* block;
	//for (std::list<block_t*>::iterator Current = m_link_rooms.begin(); Current != m_link_rooms.end(); ++Current)
	//{
	//	block = (*Current);
	//	//int tk;
	//	//std::string torch_kind[] = { "torch", "red torch", "green torch", "blue torch" };
	//	//tk = rand() % 1;
	//	add_to_map(Application::instance().m_game_object_manager->new_object("torch"), get(block->rect.y + 1,block->rect.x + 1));
	//	//tk = rand() % 1;
	//	add_to_map(Application::instance().m_game_object_manager->new_object("torch"), get(block->rect.y + 1,block->rect.x + block->rect.w - 1));
	//	//tk = rand() % 1;
	//	add_to_map(Application::instance().m_game_object_manager->new_object("torch"), get(block->rect.y + block->rect.h - 1,block->rect.x + block->rect.w - 1));
	//	//tk = rand() % 1;
	//	add_to_map(Application::instance().m_game_object_manager->new_object("torch"), get(block->rect.y + block->rect.h - 1,block->rect.x + 1));
	//	int j=0;
	//	for (int i = block->rect.x + 1; i < block->rect.x + block->rect.w; i++)
	//	{
	//		if (j == 30)
	//		{
	//			//tk = rand() % 4;
	//			add_to_map(Application::instance().m_game_object_manager->new_object("torch"), get(block->rect.y + 1,i));
	//			//tk = rand() % 4;
	//			add_to_map(Application::instance().m_game_object_manager->new_object("torch"), get(block->rect.y + block->rect.h - 1,i));
	//			j = -1;
	//		}
	//		j = j + 1;
	//	}
	//	j = 0;
	//	for (int i = block->rect.y + 1; i < block->rect.y + block->rect.h; i++)
	//	{
	//		if (j == 30)
	//		{
	//			//tk = rand() % 4;
	//			add_to_map(Application::instance().m_game_object_manager->new_object("torch"), get(i,block->rect.x + 1));
	//			//tk = rand() % 4;
	//			add_to_map(Application::instance().m_game_object_manager->new_object("torch"), get(i,block->rect.x + block->rect.w - 1));
	//			j = -1;
	//		}
	//		j = j + 1;
	//	}
	//}
}

//bool GameMap::check(int x, int y)
//{
//	if (x<0 && x>m_size.h - 1 && y<0 && y>m_size.w - 1) { return false; };
//}

//MapCell* GameMap::get_cell(int x, int y)
//{
//	if (x<0 && x>m_size.h - 1 && y<0 && y>m_size.w - 1) { return false; };
//}

void GameMap::reset_serialization_index()
{
	m_serialization_index = 0;
	for (auto item : (m_items))
	{
		if (item.m_serialization_index != 0)
		{
			item.reset_serialization_index();
		}
	}
}


Packer_generic& GameMap::get_packer()
{
	return Packer<GameMap>::Instance();
}

Game_world::Game_world()
{
	float l;
	for (int z = 0; z < 21; z++)
	{
		for (int y = 0; y < 21; y++)
		{
			for (int x = 0; x < 21; x++)
			{
				l = static_cast<float>(sqrt(x * x + y * y + z * z));
				l = l / 20.0;
				m_coefficient[z][y][x] = 1 - l;
				if (m_coefficient[z][y][x] < 0) { m_coefficient[z][y][x] = 0; }
			}
		}
	}
}

void Game_world::reset_serialization_index()
{
	m_serialization_index = 0;
	m_object_manager.reset_serialization_index();
	for (auto m_map : m_maps)
	{
		m_map->reset_serialization_index();
	}
}

std::u16string Game_world::serialize(Parser_context& context)
{
	//LOG(INFO) << "std::u16string Object_manager::items_to_json(std::list<GameObject>& value)";
	std::size_t i = 0;
	for (auto& element : m_object_manager.m_items)
	{
		element.m_serialization_index = i;
		i += 1;
	}
	std::size_t ms = m_object_manager.m_items.size();
	//LOG(INFO) << "std::u16string Game_world::serialize() " << std::to_string(ms);
	std::u16string result;
	for (auto& element : m_maps.front()->m_items)
	{
		if (!result.empty()) { result += u","; };
		result += Parser::to_json<bool>(element.m_notable, context);
		result += u",";
		result += Parser::to_json<std::list<GameObject*>>(element.m_items, context);
	}
	result = u"{\"map_size\":" + Parser::to_json<dimension3_t>(m_maps.front()->m_size,context) + u",\"manager_size\":" + Parser::to_json<std::size_t>(ms,context) +u",\"cells\":[" + result + u"],\"items\":";
	result += Parser::to_json<std::list<GameObject>>(m_object_manager.m_items,context);
	result += u",\"player\":" + Parser::to_json<GameObject*>(m_player->m_object, context);
	result += u"}";
	return result;
}

void Game_world::deserialize(std::u16string& value, Parser_context& context)
{
	//LOG(INFO) << "deserialize -> " << Parser::UTF16_to_CP1251(value);
	scheme_map_t* s = Parser::read_object(value);
	dimension3_t map_size;
	for (const auto& element : *s)
	{
		//LOG(INFO) << "deserialize -> " << Parser::UTF16_to_CP1251(element.first);
	}
	Parser::from_json<dimension3_t>((*s)[u"map_size"], map_size,context);
	GameMap* map = new GameMap(map_size);
	m_maps.push_front(map);

	std::size_t manager_size;
	Parser::from_json<std::size_t>((*s)[u"manager_size"], manager_size,context);
	m_object_manager.m_items.resize(manager_size);
	
	scheme_list_t* items = Parser::read_array((*s)[u"cells"]);
	if (items)
	{
		auto it = items->begin();
		for (auto& element : m_maps.front()->m_items)
		{
			Parser::from_json<bool>(*it, element.m_notable, context);
			++it;
			Parser::from_json<std::list<GameObject*>>(*it, element.m_items,context);
			++it;
		}
		delete items;
	}

	items = Parser::read_array((*s)[u"items"]);
	if (items)
	{
		std::size_t i = 0;
		auto it = m_object_manager.m_items.begin();
		for (const auto& element : *items)
		{
			Parser::from_json<GameObject>(element, *it,context);
			i += 1;
			++it;
		}
		delete items;
	}

	GameObject* obj;
	Parser::from_json<GameObject*>((*s)[u"player"], obj, context);
	m_player = new Player(obj, map);

	delete s;
}

std::string Game_world::bin_serialize(Parser_context& context)
{
	//LOG(INFO) << "std::string Game_world::bin_serialize()";
	std::size_t i = 0;
	for (auto& element : m_object_manager.m_items)
	{
		element.m_serialization_index = i;
		i += 1;
	}
	std::size_t ms = m_object_manager.m_items.size();
	//LOG(INFO) << "std::string Game_world::bin_serialize() "<<std::to_string(ms);
	std::string result = Parser::to_binary<dimension3_t>(m_maps.front()->m_size,context) + Parser::to_binary<std::size_t>(ms,context);
	for (auto& element : m_maps.front()->m_items)
	{
		result += Parser::to_binary<std::list<GameObject*>>(element.m_items,context);
		result += Parser::to_binary<bool>(element.m_notable, context);
	}
	for (auto& element : m_object_manager.m_items)
	{
		result += Parser::to_binary<GameObject>(element,context);
	}
	result += Parser::to_binary<GameObject*>(m_player->m_object, context);
	return result;
}

void Game_world::bin_deserialize(std::string& value, Parser_context& context)
{
	//LOG(INFO) << "Game_world::bin_deserialize(std::string& value)";

	std::size_t pos = 0;
	dimension3_t map_size;
	Parser::from_binary<dimension3_t>(value, map_size,pos,context);
	GameMap* map = new GameMap(map_size);
	m_maps.push_front(map);

	std::size_t manager_size;
	Parser::from_binary<std::size_t>(value, manager_size,pos,context);
	//LOG(INFO) << "void Game_world::bin_deserialize(std::string& value) " << std::to_string(manager_size);
	m_object_manager.m_items.resize(manager_size);

	for (auto& element : m_maps.front()->m_items)
	{
		Parser::from_binary<std::list<GameObject*>>(value,element.m_items,pos,context);
		Parser::from_binary<bool>(value, element.m_notable, pos, context);
	}

	std::size_t i=0;
	for (auto& element : m_object_manager.m_items)
	{
		Parser::from_binary<GameObject>(value, element, pos,context);
		++i;
	}
	GameObject* obj;
	Parser::from_binary<GameObject*>(value, obj, pos, context);
	m_player = new Player(obj, map);
}

Packer_generic& Game_world::get_packer()
{
	return Packer<Game_world>::Instance();
}



void Game_world::calculate_lighting()
{
	int lx;
	int ly;
	int lz;
	int c;
	FOV_light fl;

	for (auto m = m_maps.begin(); m != m_maps.end(); ++m)
	{
		GameMap& map = *(*m);
		for (int z = 0; z < map.m_size.dz; ++z)
		{
			for (int y = 0; y < map.m_size.dy; ++y)
			{
				for (int x = 0; x < map.m_size.dx; ++x)
				{
					map.get(z, y, x).m_light = light_t();
				}
			}
		}
	}
	for (auto l = m_object_manager.m_items.begin(); l != m_object_manager.m_items.end(); ++l)
	{
		GameObject& object = (*l);
		if (!object.m_active_state->m_light) { continue; }
		if (object.m_owner->m_kind != entity_e::cell) { continue; }
		GameMap* map = dynamic_cast<MapCell*>(object.m_owner)->m_map;
		fl.calculate(20, &*l, map);
		for (int z = 0; z < 41; ++z)
		{
			for (int y = 0; y < 41; ++y)
			{
				for (int x = 0; x < 41; ++x)
				{
					/*if(fl.get(z,y,x).visible)
					{*/
						lx = abs(x - 20);
						ly = abs(y - 20);
						lz = abs(z - 20);
						c = (l->m_active_state->m_light->R * m_coefficient[lz][ly][lx]);// *fl.m_map[y][x].damping.R;
						if (c < 0) { c = 0; }
						fl.get(z, y, x).light.R = c;
						c = (l->m_active_state->m_light->G * m_coefficient[lz][ly][lx]);// *fl.m_map[y][x].damping.G;
						if (c < 0) { c = 0; }
						fl.get(z, y, x).light.G = c;
						c = (l->m_active_state->m_light->B * m_coefficient[lz][ly][lx]);// *fl.m_map[y][x].damping.B;
						if (c < 0) { c = 0; }
						fl.get(z, y, x).light.B = c;
				/*	}
					else
					{
						fl.get(z, y, x).light.R = 0;
						fl.get(z, y, x).light.G = 0;
						fl.get(z, y, x).light.B = 0;
					}*/
				}
			}
		}
		for (int z = 0; z < 41; ++z)
		{
			lz = l->cell()->z + z - 20;
			if (!((lz < 0) || (lz > map->m_size.dz - 1)))
			{
				for (int y = 0; y < 41; y++)
				{
					ly = l->cell()->y + y - 20;
					if (!((ly < 0) || (ly > map->m_size.dy - 1)))
					{
						for (int x = 0; x < 41; x++)
						{
							lx = l->cell()->x + x - 20;
							if (!((lx < 0) || (lx > map->m_size.dx - 1)))
							{
								map->get(lz, ly, lx).m_light.R += fl.get(z, y, x).light.R;
								map->get(lz, ly, lx).m_light.G += fl.get(z, y, x).light.G;
								map->get(lz, ly, lx).m_light.B += fl.get(z, y, x).light.B;
							}
						}
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
