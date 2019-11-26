#include "GameMap.h"

std::u16string GameMap::vector_mapcell_to_json(std::vector<MapCell>& value, SerializationContext& context)
{
    std::u16string result = u"[";
    for (auto element : value) {
        if (result != u"[") {
            result += u",";
        }
        result += parser_to_json<std::list<GameObject*>>(element.m_items, context);
    }
    result += u"]";
    return result;
}

void GameMap::vector_mapcell_from_json(const std::u16string& value, std::vector<MapCell>& prop, SerializationContext& context)
{
    auto temp = value;
    const auto s = read_array(temp);
    if (s) {
        std::size_t i = 0;
        prop.resize(s->size());
        for (const auto& element : (*s)) {
            parser_from_json<std::list<GameObject*>>(element, prop[i].m_items, context);
            i += 1;
        }
    }
    delete s;
}

std::string GameMap::vector_mapcell_to_binary(std::vector<MapCell>& value, SerializationContext& context)
{
    auto s = value.size();
    auto result = std::string(reinterpret_cast<const char*>(&s), sizeof(std::size_t));
    for (auto element : value) {
        result += parser_to_binary<std::list<GameObject*>>(element.m_items, context);
    }
    return result;
}

void GameMap::vector_mapcell_from_binary(const std::string& value, std::vector<MapCell>& prop, std::size_t& pos, SerializationContext& context)
{
    const auto s = *reinterpret_cast<const std::size_t*>(&value[pos]);
    pos += sizeof(std::size_t);
    prop.resize(s);
    for (std::size_t i = 0; i < s; ++i) {
        parser_from_binary<std::list<GameObject*>>(value, prop[i].m_items, pos, context);
    }
}

std::u16string GameMap::serialize_to_json_reference(JsonWriter& value)
{
    return value.write(*this);
}

std::u16string GameMap::serialize_to_json_pointer(JsonWriter& value)
{
    return value.write(this);
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

MapCell& GameMap::get(const int z, const int y, const int x)
{
    return m_items[long(z) * (m_size.dy * m_size.dx) + m_size.dy * y + x];
}

void GameMap::init(const dimension3_t size)
{
    m_size = size;
    Logger::instance().info("Карта инициализирована с размером: " + std::to_string(m_size.dx) < +"x" + std::to_string(m_size.dy) + "x" + std::to_string(m_size.dz));
    m_items.resize(long(m_size.dz) * m_size.dy * m_size.dx);
    auto i = 0;
    for (auto z = 0; z < m_size.dz; ++z) {
        for (auto y = 0; y < m_size.dy; ++y) {
            for (auto x = 0; x < m_size.dx; ++x) {
                auto& m = m_items[i];
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
    for (auto z = 0; z < 6; ++z) {
        for (auto y = 0; y < m_size.dy; y++) {
            for (auto x = 0; x < m_size.dx; x++) {
                if (z == 0) {
                    const auto obj = Application::instance().m_game_object_manager->new_object("floor");
                    add_object(obj, get(z, y, x));
                } else {
                    if ((y == 0) || (x == 0) || (y == m_size.dy - 1) || (x == m_size.dx - 1)) {
                        if ((x == 0) || (x == m_size.dx - 1) || (y == 0) || (y == m_size.dy - 1)) {
                            const auto obj = Application::instance().m_game_object_manager->new_object("wall");
                            add_object(obj, get(z, y, x));
                        }
                    }
                }
            }
        }
    }

    int light_source_count = 0;
    for (int y = 1; y < m_size.dy; y = y + 50) {
        for (int x = 1; x < m_size.dx; x = x + 50) {
	        const auto obj = Application::instance().m_game_object_manager->new_object("torch");
            add_to_map(obj, get(1, y, x));
            light_source_count += 1;
        }
    }

    Logger::instance().info(" -= Всего источников света: " + std::to_string(light_source_count) + "=-");

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
    for (int z = 0; z < object->m_active_state->m_size.dz; ++z) {
        for (int y = 0; y < object->m_active_state->m_size.dy; ++y) {
            for (int x = 0; x < object->m_active_state->m_size.dx; ++x) {
                get(cell.z + z, cell.y - y, cell.x + x).add_object(object);
            }
        }
    }
}

void GameMap::remove_object(GameObject* object)
{
    auto& cell = *object->cell();
    for (auto z = 0; z < object->m_active_state->m_size.dz; z++) {
        for (auto y = 0; y < object->m_active_state->m_size.dy; y++) {
            for (auto x = 0; x < object->m_active_state->m_size.dx; x++) {
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

void GameMap::turn_object(GameObject* object)
{
    const auto position = object->cell();
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

bool GameMap::divide_block(block_t* block, int depth, int current)
{
    auto choice(rand() % 100);
    block_t* sub_block;
    if (choice < 50) {
        const auto range = block->rect.w;
        if (range < 22) {
            return true;
        }
        choice = rand() % (range - 6) + 11;
        if (range - choice < 11) {
            choice = 11;
            if (range - choice - 11 < 11) {
                return true;
            }
        }
        if (current < depth) {
            sub_block = new block_t(block->rect.x, block->rect.y, choice, block->rect.h);
            if (divide_block(sub_block, depth, current + 1)) {
                m_rooms.push_back(sub_block);
            }
            sub_block = new block_t(block->rect.x + choice, block->rect.y, block->rect.w - choice, block->rect.h);
            if (divide_block(sub_block, depth, current + 1)) {
                m_rooms.push_back(sub_block);
            }
            return false;
        }
    } else {
        const auto range = block->rect.h;
        if (range < 22) {
            return true;
        }
        choice = rand() % (range - 6) + 11;
        if (range - choice < 11) {
            choice = 11;
            if (range - choice - 11 < 11) {
                return true;
            }
        }
        if (current < depth) {
            sub_block = new block_t(block->rect.x, block->rect.y, block->rect.w, choice);
            if (divide_block(sub_block, depth, current + 1)) {
                m_rooms.push_back(sub_block);
            }
            sub_block = new block_t(block->rect.x, block->rect.y + choice, block->rect.w, block->rect.h - choice);
            if (divide_block(sub_block, depth, current + 1)) {
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
    auto len = rand() % block->rect.h;
    if (len > 10) {
        block->rect.y = block->rect.y + rand() % (block->rect.h - len);
        block->rect.h = len;
    }
    len = rand() % block->rect.w;
    if (len > 10) {
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
    int index;
    while (!m_rooms.empty()) {
        auto temp = m_rooms.front();
        if (!m_link_rooms.empty()) {
            index = rand() % m_link_rooms.size();
            auto nx = std::next(m_link_rooms.begin(), index);
            link_room(temp, *nx);
            m_link_rooms.push_front(temp);
            m_rooms.pop_front();
        } else {
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

void GameMap::add_wall()
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

void GameMap::add_doors()
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

void GameMap::blur_lighting()
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

void GameMap::bresenham_line(MapCell* a, MapCell* b, const std::function<void(MapCell*)>& fn)
{

    int i, err_1, err_2;
    int point[3];

    point[0] = a->x;
    point[1] = a->y;
    point[2] = a->z;
    const auto dx = b->x - a->x;
    const auto dy = b->y - a->y;
    const auto dz = b->z - a->z;
    const auto x_inc = (dx < 0) ? -1 : 1;
    const auto l = abs(dx);
    const auto y_inc = (dy < 0) ? -1 : 1;
    const auto m = abs(dy);
    const auto z_inc = (dz < 0) ? -1 : 1;
    const auto n = abs(dz);
    const auto dx2 = l << 1;
    const auto dy2 = m << 1;
    const auto dz2 = n << 1;

    if (l >= m && l >= n) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (i = 0; i < l; i++) {
            fn(&get(point[2], point[1], point[0]));
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            point[0] += x_inc;
        }
    } else if (m >= l && m >= n) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (i = 0; i < m; i++) {
            fn(&get(point[2], point[1], point[0]));
            if (err_1 > 0) {
                point[0] += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            point[1] += y_inc;
        }
    } else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (i = 0; i < n; i++) {
            fn(&get(point[2], point[1], point[0]));
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                point[0] += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            point[2] += z_inc;
        }
    }
    fn(&get(point[2], point[1], point[0]));
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
    for (auto item : (m_items)) {
        if (item.m_serialization_index != 0) {
            item.reset_serialization_index();
        }
    }
}

iPacker& GameMap::get_packer()
{
    return Packer<GameMap>::instance();
}