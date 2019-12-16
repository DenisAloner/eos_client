#include "AI.h"

min_heap::min_heap()
{
}

void min_heap::push(Node* value)
{
    m_items.push_back(value);
    auto index = m_items.size() - 1;
    while (index > 0) {
        const auto parent = (index - 1) >> 1;
        if (m_items[index]->f < m_items[parent]->f) {
            std::swap(m_items[index], m_items[parent]);
        }
        index = parent;
    }
}

Node* min_heap::pop()
{
    const auto n = m_items[0];
    m_items[0] = m_items[m_items.size() - 1];
    m_items.pop_back();
    sort_down(0);
    return n;
}

void min_heap::sort_down(unsigned int index)
{
    const int size = m_items.size();
    const int left = 2 * index + 1;
    const int right = 2 * index + 2;
    if (left < size) {
        if (m_items[index]->f > m_items[left]->f) {
            std::swap(m_items[index], m_items[left]);
            sort_down(left);
        }
    }
    if (right < size) {
        if (m_items[index]->f > m_items[right]->f) {
            std::swap(m_items[index], m_items[right]);
            sort_down(right);
        }
    }
}

void min_heap::sort_up(std::size_t index)
{
    while (index > 0) {
        const auto parent = (index - 1) >> 1;
        if (m_items[index]->f < m_items[parent]->f) {
            std::swap(m_items[index], m_items[parent]);
        }
        index = parent;
    }
}

void min_heap::edit(const unsigned int index, const int value)
{
    if (m_items[index]->f != value) {
        if (m_items[index]->f < value) {
            m_items[index]->f = value;
            sort_down(index);
        } else {
            m_items[index]->f = value;
            sort_up(index);
        }
    }
}

Path::Path()
{
}

void Path::calculate(GameMap* map, GameObject* object, MapCell* gc, GameObject* goal, int radius)
{
    /*int left_limit = (object->cell()->x - radius < 0) ? 0 : object->cell()->x - radius;
	int right_limit = (object->cell()->x + radius + 1 > map->m_size.w) ? map->m_size.w : object->cell()->y + radius + 1;
	int bottom_limit = (object->cell()->y - radius < 0) ? 0 : object->cell()->y - radius;
	int top_limit = (object->cell()->y + radius + 1 > map->m_size.h) ? map->m_size.h : object->cell()->y + radius + 1;*/
    m_open = 0;
    for (auto z = 0; z < map->m_size.dz; z++) {
        for (auto y = 0; y < map->m_size.dy; y++) {
            for (auto x = 0; x < map->m_size.dx; x++) {
                auto& c = map->get(z, y, x);
                c.m_closed = false;
            }
        }
    }
    m_game_map = map;
    m_unit = object;
    m_start_cell = object->cell();
    m_start_size = object->m_active_state->m_size;
    m_goal_cell = gc;
    m_goal_size = goal->m_active_state->m_size;
    const auto qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicate;
    for (auto z = 0; z < map->m_size.dz; z++) {
        for (auto y = 0; y < map->m_size.dy; y++) {
            for (auto x = 0; x < map->m_size.dx; x++) {
                auto& c = map->get(z, y, x);
                c.m_closed = false;
                c.m_state = 0;
                c.m_mark = false;
                for (auto& item : c.m_items) {
                    if (item != object && item != goal && qualifier(item)) {
                        c.m_state = 1;
                        break;
                    }
                }
            }
        }
    }
}

void Path::map_costing(GameMap* map, GameObject* object, MapCell* gc, int radius)
{
    /*int left_limit = (object->cell()->x - radius < 0) ? 0 : object->cell()->x - radius;
	int right_limit = (object->cell()->x + radius + 1 > map->m_size.w) ? map->m_size.w : object->cell()->y + radius + 1;
	int bottom_limit = (object->cell()->y - radius < 0) ? 0 : object->cell()->y - radius;
	int top_limit = (object->cell()->y + radius + 1 > map->m_size.h) ? map->m_size.h : object->cell()->y + radius + 1;*/

    for (auto z = 0; z < map->m_size.dz; z++) {
        for (auto y = 0; y < map->m_size.dy; y++) {
            for (auto x = 0; x < map->m_size.dx; x++) {
                auto& c = map->get(z, y, x);
                c.m_closed = false;
            }
        }
    }
    m_game_map = map;
    m_unit = object;
    m_start_cell = object->cell();
    m_start_size = object->m_active_state->m_size;
    m_goal_cell = gc;
    const auto qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicate;
    for (auto z = 0; z < map->m_size.dz; z++) {
        for (auto y = 0; y < map->m_size.dy; y++) {
            for (auto x = 0; x < map->m_size.dx; x++) {
                auto& c = map->get(z, y, x);
                c.m_closed = false;
                c.m_state = 0;
                for (auto& item : c.m_items) {
                    if (item != object && qualifier(item)) {
                        c.m_state = 1;
                        break;
                    }
                }
            }
        }
    }
}

//int Path::manhattan(MapCell* a, MapCell* b) {
//	return abs(a->x - b->x) + abs(a->y - b->y);
//}

int Path::manhattan(MapCell* a, MapCell* b, MapCell* c)
{
    return (abs(a->x - b->x) + abs(a->y - b->y) + abs(a->z - b->z)) * 10;
}

int Path::is_in_open(MapCell* c)
{
    for (std::size_t i = 0; i < m_heap.m_items.size(); ++i) {
        if (m_heap.m_items[i]->cell == c) {
            return i;
        }
    }
    return -1;
}

void Path::insert_into_open(const int x, const int y, const int z, const int dg, Node* p)
{
    for (auto iz = 0; iz < m_start_size.dy; iz++) {
        for (auto iy = 0; iy < m_start_size.dy; iy++) {
            for (auto ix = 0; ix < m_start_size.dx; ix++) {
                if (x + ix >= 0 && y - iy >= 0 && z + iz >= 0 && x + ix < m_game_map->m_size.dx && y - iy < m_game_map->m_size.dy && z + iz < m_game_map->m_size.dz) {
                    if (m_game_map->get(z + iz, y - iy, x + ix).m_state != 0) {
                        return;
                    }
                } else
                    return;
            }
        }
    }

    const auto c = &m_game_map->get(z, y, x);
    if (!c->m_closed) {
        m_open += 1;
        c->m_mark = true;
        const auto n = is_in_open(c);
        if (n == -1) {
            m_heap.push(new Node(c, p->g + dg, manhattan(c, m_goal_cell, m_start_cell), p));
        } else {
            const auto item = m_heap.m_items[n];
            if (p->g + dg < item->g) {
                item->parent = p;
                item->g = p->g + dg;
                item->h = manhattan(item->cell, m_goal_cell, m_start_cell);
                m_heap.edit(n, item->g + item->h);
            }
        }
    }
}

std::vector<MapCell*>* Path::back(Node* c)
{
    //auto cell = static_cast<MapCell*>(m_unit->m_owner);
    auto* result = new std::vector<MapCell*>();
    auto current = c;
    while (current) {
        result->push_back(current->cell);
        current = current->parent;
    }
    return result;
}

std::vector<MapCell*>* Path::get_path()
{
    m_heap.push(new Node(m_start_cell, 0, manhattan(m_start_cell, m_goal_cell, m_start_cell)));
    while (!m_heap.m_items.empty()) {
        const auto current = m_heap.pop();
        if (Game_algorithm::check_distance(current->cell, m_start_size, m_goal_cell, m_goal_size)) {
            return back(current);
        }
        auto cell = *current->cell;
        cell.m_closed = true;

        insert_into_open(cell.x, cell.y, cell.z - 1, 10, current);
        insert_into_open(cell.x + 1, cell.y, cell.z - 1, 14, current);
        insert_into_open(cell.x - 1, cell.y, cell.z - 1, 14, current);
        insert_into_open(cell.x, cell.y + 1, cell.z - 1, 14, current);
        insert_into_open(cell.x, cell.y - 1, cell.z - 1, 14, current);
        insert_into_open(cell.x + 1, cell.y + 1, cell.z - 1, 17, current);
        insert_into_open(cell.x + 1, cell.y - 1, cell.z - 1, 17, current);
        insert_into_open(cell.x - 1, cell.y + 1, cell.z - 1, 17, current);
        insert_into_open(cell.x - 1, cell.y - 1, cell.z - 1, 17, current);

        insert_into_open(cell.x + 1, cell.y, cell.z, 10, current);
        insert_into_open(cell.x - 1, cell.y, cell.z, 10, current);
        insert_into_open(cell.x, cell.y + 1, cell.z, 10, current);
        insert_into_open(cell.x, cell.y - 1, cell.z, 10, current);
        insert_into_open(cell.x + 1, cell.y + 1, cell.z, 14, current);
        insert_into_open(cell.x + 1, cell.y - 1, cell.z, 14, current);
        insert_into_open(cell.x - 1, cell.y + 1, cell.z, 14, current);
        insert_into_open(cell.x - 1, cell.y - 1, cell.z, 14, current);

        insert_into_open(cell.x, cell.y, cell.z + 1, 10, current);
        insert_into_open(cell.x + 1, cell.y, cell.z + 1, 14, current);
        insert_into_open(cell.x - 1, cell.y, cell.z + 1, 14, current);
        insert_into_open(cell.x, cell.y + 1, cell.z + 1, 14, current);
        insert_into_open(cell.x, cell.y - 1, cell.z + 1, 14, current);
        insert_into_open(cell.x + 1, cell.y + 1, cell.z + 1, 17, current);
        insert_into_open(cell.x + 1, cell.y - 1, cell.z + 1, 17, current);
        insert_into_open(cell.x - 1, cell.y + 1, cell.z + 1, 17, current);
        insert_into_open(cell.x - 1, cell.y - 1, cell.z + 1, 17, current);
    }
    return nullptr;
}

std::vector<MapCell*>* Path::get_path_to_cell()
{
    m_heap.push(new Node(m_start_cell, 0, manhattan(m_start_cell, m_goal_cell, m_start_cell)));
    while (!m_heap.m_items.empty()) {
        const auto current = m_heap.pop();
        if ((current->cell->x == m_goal_cell->x) && (current->cell->y == m_goal_cell->y)) {
            return back(current);
        }
        auto cell = *current->cell;
        cell.m_closed = true;

        insert_into_open(cell.x, cell.y, cell.z - 1, 10, current);
        insert_into_open(cell.x + 1, cell.y, cell.z - 1, 14, current);
        insert_into_open(cell.x - 1, cell.y, cell.z - 1, 14, current);
        insert_into_open(cell.x, cell.y + 1, cell.z - 1, 14, current);
        insert_into_open(cell.x, cell.y - 1, cell.z - 1, 14, current);
        insert_into_open(cell.x + 1, cell.y + 1, cell.z - 1, 17, current);
        insert_into_open(cell.x + 1, cell.y - 1, cell.z - 1, 17, current);
        insert_into_open(cell.x - 1, cell.y + 1, cell.z - 1, 17, current);
        insert_into_open(cell.x - 1, cell.y - 1, cell.z - 1, 17, current);

        insert_into_open(cell.x + 1, cell.y, cell.z, 10, current);
        insert_into_open(cell.x - 1, cell.y, cell.z, 10, current);
        insert_into_open(cell.x, cell.y + 1, cell.z, 10, current);
        insert_into_open(cell.x, cell.y - 1, cell.z, 10, current);
        insert_into_open(cell.x + 1, cell.y + 1, cell.z, 14, current);
        insert_into_open(cell.x + 1, cell.y - 1, cell.z, 14, current);
        insert_into_open(cell.x - 1, cell.y + 1, cell.z, 14, current);
        insert_into_open(cell.x - 1, cell.y - 1, cell.z, 14, current);

        insert_into_open(cell.x, cell.y, cell.z + 1, 10, current);
        insert_into_open(cell.x + 1, cell.y, cell.z + 1, 14, current);
        insert_into_open(cell.x - 1, cell.y, cell.z + 1, 14, current);
        insert_into_open(cell.x, cell.y + 1, cell.z + 1, 14, current);
        insert_into_open(cell.x, cell.y - 1, cell.z + 1, 14, current);
        insert_into_open(cell.x + 1, cell.y + 1, cell.z + 1, 17, current);
        insert_into_open(cell.x + 1, cell.y - 1, cell.z + 1, 17, current);
        insert_into_open(cell.x - 1, cell.y + 1, cell.z + 1, 17, current);
        insert_into_open(cell.x - 1, cell.y - 1, cell.z + 1, 17, current);
    }
    return nullptr;
}

Dijkstra_map::Dijkstra_map()
{
}

bool Dijkstra_map::opaque_check(int x, int y, int size)
{
    for (int i = 0; i < size; i++) {
        if (y - i < 0) {
            return false;
        }
        for (int j = 0; j < size; j++) {
            if (x + j > m_map->m_size.dx - 1) {
                return false;
            }
            if (m_map->get(0, y - i, x + j).m_pathfind_info.opaque) {
                return false;
            }
        }
    }
    return true;
}

void Dijkstra_map::calculate_cost(GameMap* map, GameObject* object, GameObject* goal)
{
    m_map = map;
    MapCell* c;
    std::function<bool(GameObject*)> qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicate;
    for (int y = 0; y < m_map->m_size.dy; y++) {
        for (int x = 0; x < m_map->m_size.dx; x++) {
            c = &m_map->get(0, y, x);
            c->m_pathfind_info.opaque = false;
            c->m_pathfind_info.value = 100000;
            for (auto item = c->m_items.begin(); item != c->m_items.end(); ++item) {
                if ((*item) == goal) {
                    c->m_pathfind_info.opaque = false;
                    c->m_pathfind_info.value = 0;
                } else {
                    if (((*item) != object) && qualifier((*item))) {
                        c->m_pathfind_info.opaque = true;
                        break;
                    }
                }
            }
        }
    }
}

void Dijkstra_map::calculate_cost_autoexplore(GameMap* map, GameObject* object)
{
    m_map = map;
    const auto qualifier = static_cast<AI_enemy*>(object->m_active_state->m_ai)->m_path_qualifier->predicate;
    for (auto y = 0; y < m_map->m_size.dy; y++) {
        for (auto x = 0; x < m_map->m_size.dx; x++) {
            auto c = &m_map->get(0, y, x);
            if (!c->m_notable) {
                c->m_pathfind_info.opaque = false;
                c->m_pathfind_info.value = 0;
            } else {
                c->m_pathfind_info.opaque = false;
                c->m_pathfind_info.value = 100000;
                for (auto item = c->m_items.begin(); item != c->m_items.end(); ++item) {
                    if (((*item) != object) && qualifier((*item))) {
                        c->m_pathfind_info.opaque = true;
                        break;
                    }
                }
            }
        }
    }
}

void Dijkstra_map::trace()
{
    bool was_change;
    int low;
    int i = 0;
    GameMap& cells = *m_map;
    do {
        was_change = false;
        for (int y = 1; y < m_map->m_size.dy - 1; y++) {
            for (int x = 1; x < m_map->m_size.dx - 1; x++) {
                if (opaque_check(x, y, 2)) {
                    low = cells.get(0, y - 1, x - 1).m_pathfind_info.value;
                    low = (cells.get(0, y - 1, x).m_pathfind_info.value < low) ? cells.get(0, y - 1, x).m_pathfind_info.value : low;
                    low = (cells.get(0, y - 1, x + 1).m_pathfind_info.value < low) ? cells.get(0, y - 1, x + 1).m_pathfind_info.value : low;
                    low = (cells.get(0, y, x - 1).m_pathfind_info.value < low) ? cells.get(0, y, x - 1).m_pathfind_info.value : low;
                    low = (cells.get(0, y, x + 1).m_pathfind_info.value < low) ? cells.get(0, y, x + 1).m_pathfind_info.value : low;
                    low = (cells.get(0, y + 1, x - 1).m_pathfind_info.value < low) ? cells.get(0, y + 1, x - 1).m_pathfind_info.value : low;
                    low = (cells.get(0, y + 1, x).m_pathfind_info.value < low) ? cells.get(0, y + 1, x).m_pathfind_info.value : low;
                    low = (cells.get(0, y + 1, x + 1).m_pathfind_info.value < low) ? cells.get(0, y + 1, x + 1).m_pathfind_info.value : low;
                    if (cells.get(0, y, x).m_pathfind_info.value > low + 1) {
                        was_change = true;
                        cells.get(0, y, x).m_pathfind_info.value = low + 1;
                    }
                }
            }
        }
        i += 1;
    } while (was_change);
}

MapCell* Dijkstra_map::next(GameObject* object)
{
    MapCell* c = nullptr;
    int value = object->cell()->m_pathfind_info.value;
    int x = object->cell()->x;
    int y = object->cell()->y;
    if (m_map->get(0, y - 1, x - 1).m_pathfind_info.value < value) {
        c = &m_map->get(0, y - 1, x - 1);
        value = m_map->get(0, y - 1, x - 1).m_pathfind_info.value;
    }
    if (m_map->get(0, y - 1, x).m_pathfind_info.value < value) {
        c = &m_map->get(0, y - 1, x);
        value = m_map->get(0, y - 1, x).m_pathfind_info.value;
    }
    if (m_map->get(0, y - 1, x + 1).m_pathfind_info.value < value) {
        c = &m_map->get(0, y - 1, x + 1);
        value = m_map->get(0, y - 1, x + 1).m_pathfind_info.value;
    }
    if (m_map->get(0, y, x - 1).m_pathfind_info.value < value) {
        c = &m_map->get(0, y, x - 1);
        value = m_map->get(0, y, x - 1).m_pathfind_info.value;
    }
    if (m_map->get(0, y, x + 1).m_pathfind_info.value < value) {
        c = &m_map->get(0, y, x + 1);
        value = m_map->get(0, y, x + 1).m_pathfind_info.value;
    }
    if (m_map->get(0, y + 1, x - 1).m_pathfind_info.value < value) {
        c = &m_map->get(0, y + 1, x - 1);
        value = m_map->get(0, y + 1, x - 1).m_pathfind_info.value;
    }
    if (m_map->get(0, y + 1, x).m_pathfind_info.value < value) {
        c = &m_map->get(0, y + 1, x);
        value = m_map->get(0, y + 1, x).m_pathfind_info.value;
    }
    if (m_map->get(0, y + 1, x + 1).m_pathfind_info.value < value) {
        c = &m_map->get(0, y + 1, x + 1);
        value = m_map->get(0, y + 1, x + 1).m_pathfind_info.value;
    }
    return c;
}

AI_enemy::AI_enemy()
{
    m_ai_type = ai_type_e::non_humanoid;
    m_fov = new FOV();
    m_goal = nullptr;
    m_memory_goal_cell = nullptr;
    m_action_controller = new Action_wrapper();
}

AI* AI_enemy::clone()
{
    auto c = new AI_enemy();
    c->m_ai_type = m_ai_type;
    //for (auto current = m_FOVs.begin(); current != m_FOVs.end(); current++)
    //{
    //	c->m_FOVs.push_back(AI_FOV(current->radius, current->qualifier, current->start_angle, current->end_angle));
    //}
    c->m_path_qualifier = m_path_qualifier;
    return c;
}

GameObject* AI_enemy::find_goal()
{
    auto vl = static_cast<Vision_list*>(m_object->m_active_state->get_list(interaction_e::vision));
    auto radius = vl->m_max_radius;

    auto xc = m_object->cell()->x;
    auto yc = m_object->cell()->y;
    auto zc = m_object->cell()->z;
    auto dx = m_object->m_active_state->m_size.dx;
    auto dy = m_object->m_active_state->m_size.dy;
    auto dz = m_object->m_active_state->m_size.dz;

    auto xs = dx - 1 >> 1;
    auto ys = dy - 1 >> 1;
    auto zs = dz - 1 >> 1;

    auto x_start = xc - radius + xs;
    x_start = (std::max)(x_start, 0);
    x_start = (std::min)(x_start, m_map->m_size.dx - 1);
    auto x_end = xc + radius + xs;
    x_end = (std::max)(x_end, 0);
    x_end = (std::min)(x_end, m_map->m_size.dx - 1);
    auto y_start = yc - radius - ys;
    y_start = (std::max)(y_start, 0);
    y_start = (std::min)(y_start, m_map->m_size.dy - 1);
    auto y_end = yc + radius - ys;
    y_end = (std::max)(y_end, 0);
    y_end = (std::min)(y_end, m_map->m_size.dy - 1);
    auto z_start = zc - radius + zs;
    z_start = (std::max)(z_start, 0);
    z_start = (std::min)(z_start, m_map->m_size.dz - 1);
    auto z_end = zc + radius + zs;
    z_end = (std::max)(z_end, 0);
    z_end = (std::min)(z_end, m_map->m_size.dz - 1);

    for (int z = z_start; z < z_end + 1; z++) {
        for (int y = y_start; y < y_end + 1; y++) {
            for (int x = x_start; x < x_end + 1; x++) {
                const auto fc = &m_fov->m_map[y - y_start][x - x_start];
                if (fc->visible) {
                    for (auto& m_item : m_map->get(z, y, x).m_items) {
                        if (m_item == Application::instance().m_world->m_player->m_object) {
                            return Application::instance().m_world->m_player->m_object;
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

bool AI_enemy::check_goal()
{
    auto vl = static_cast<Vision_list*>(m_object->m_active_state->get_list(interaction_e::vision));
    auto radius = vl->m_max_radius;

    auto xc = m_object->cell()->x;
    auto yc = m_object->cell()->y;
    auto zc = m_object->cell()->y;
    auto dx = m_object->m_active_state->m_size.dx;
    auto dy = m_object->m_active_state->m_size.dy;
    auto dz = m_object->m_active_state->m_size.dz;

    auto xs = dx - 1 >> 1;
    auto ys = dy - 1 >> 1;
    auto zs = dz - 1 >> 1;

    int x_start = xc - radius + xs;
    x_start = (std::max)(x_start, 0);
    x_start = (std::min)(x_start, m_map->m_size.dx - 1);
    int x_end = xc + radius + xs;
    x_end = (std::max)(x_end, 0);
    x_end = (std::min)(x_end, m_map->m_size.dx - 1);
    int y_start = yc - radius - ys;
    y_start = (std::max)(y_start, 0);
    y_start = (std::min)(y_start, m_map->m_size.dy - 1);
    int y_end = yc + radius - ys;
    y_end = (std::max)(y_end, 0);
    y_end = (std::min)(y_end, m_map->m_size.dy - 1);
    int z_start = zc - radius + zs;
    z_start = (std::max)(z_start, 0);
    z_start = (std::min)(z_start, m_map->m_size.dz - 1);
    int z_end = zc + radius + zs;
    z_end = (std::max)(z_end, 0);
    z_end = (std::min)(z_end, m_map->m_size.dz - 1);

    for (int z = z_start; z < z_end + 1; z++) {
        for (int y = y_start; y < y_end + 1; y++) {
            for (int x = x_start; x < x_end + 1; x++) {
                const auto fc = &m_fov->m_map[y - y_start][x - x_start];
                if (fc->visible) {
                    for (auto& m_item : m_map->get(z, y, x).m_items) {
                        if (m_item == m_goal) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

void AI_enemy::calculate_fov(GameObject* object, GameMap* map)
{
    m_fov->calculate_FOV(object, map);
    update(VoidEventArgs());
}

void AI_enemy::create()
{
    if (m_object->m_active_state->m_ai == nullptr) {
        return;
    }
    if (!m_action_controller->m_action) {
        calculate_fov(m_object, m_map);
        MapCell* c;
        if (m_goal) {
            if (!check_goal()) {
                const auto goal = find_goal();
                if (goal) {
                    c = m_goal->cell();
                    m_memory_goal_cell = c;
                } else
                    c = m_memory_goal_cell;
            } else {
                c = m_goal->cell();
                m_memory_goal_cell = c;
            }
        } else {
            m_goal = find_goal();
            if (m_goal) {
                c = m_goal->cell();
                m_memory_goal_cell = c;
            } else
                return;
        }
        if (Game_algorithm::check_distance(dynamic_cast<MapCell*>(m_object->m_owner), m_object->m_active_state->m_size,
                dynamic_cast<MapCell*>(m_goal->m_owner), m_goal->m_active_state->m_size)) {
            std::list<Action_helper_t> attacks;
            m_object->get_actions_list(attacks);
            for (auto& attack : attacks) {
                if (attack.action->m_kind == action_e::hit_melee) {
                    auto p = attack.parameter;
                    (*p)[1].set(m_goal);
                    auto attack_area = area_t(position_t<int>(m_object->cell()->x - 1, m_object->cell()->y + 1), position_t<int>(m_object->cell()->x - m_object->m_active_state->m_size.dx, m_object->cell()->y - m_object->m_active_state->m_size.dy));
                    attack_area.p1.x = (std::max)(attack_area.p1.x, m_goal->cell()->x);
                    attack_area.p2.x = (std::min)(attack_area.p2.x, m_goal->cell()->x + m_goal->m_active_state->m_size.dx - 1);
                    attack_area.p1.y = (std::min)(attack_area.p1.y, m_goal->cell()->y);
                    attack_area.p2.y = (std::max)(attack_area.p2.y, m_goal->cell()->y - m_goal->m_active_state->m_size.dy + 1);
                    auto map = c->m_map;
                    (*p)[3].set(&map->get(0, attack_area.p1.y, attack_area.p1.x));
                    Logger::instance().info("Удар противника в точку [" + std::to_string(attack_area.p1.x) + "," + std::to_string(attack_area.p1.y) + "]");
                    m_action_controller->set((*p)[0].m_object, attack.action, p);
                    break;
                }
            }
            //Application::instance().m_action_manager->add(new GameTask(this, p));

            //P_unit_interaction* p = new P_unit_interaction();
            //p->m_unit = m_object;
            //p->m_object = m_goal;
            //p->m_unit_body_part = nullptr;
            ////Application::instance().m_action_manager->add(p->m_unit, new GameTask(Application::instance().m_actions[action_e::hit], p));
            //m_action_controller->set(p->m_unit,Application::instance().m_actions[action_e::hit], p);
        } else {
            /*	m_map->m_dijkstra_map->calculate(m_map, m_object, m_goal);
			MapCell* c = nullptr;
			int value = m_map->m_dijkstra_map->m_map[m_object->cell()->y][m_object->cell()->x].value;
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x - 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y - 1][m_object->cell()->x - 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x - 1].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x].value < value)
			{
				c = m_map->m_items[m_object->cell()->y - 1][m_object->cell()->x];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x + 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y - 1][m_object->cell()->x + 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y - 1][m_object->cell()->x + 1].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y][m_object->cell()->x - 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y][m_object->cell()->x - 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y][m_object->cell()->x - 1].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y][m_object->cell()->x + 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y][m_object->cell()->x + 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y][m_object->cell()->x + 1].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x - 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y + 1][m_object->cell()->x - 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x - 1].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x].value < value)
			{
				c = m_map->m_items[m_object->cell()->y + 1][m_object->cell()->x];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x].value;
			}
			if (m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x + 1].value < value)
			{
				c = m_map->m_items[m_object->cell()->y + 1][m_object->cell()->x + 1];
				value = m_map->m_dijkstra_map->m_map[m_object->cell()->y + 1][m_object->cell()->x + 1].value;
			}
			if (c != nullptr)
			{
				Parameter_Position* P;
						P = new Parameter_Position();
						P->m_object = m_object;
						P->m_place = c;
						P->m_map = m_map;
						m_action_controller->set(P->m_object, Application::instance().m_actions[action_e::move], P);
			}*/
            auto radius = 0;
            auto vl = static_cast<Vision_list*>(m_object->m_active_state->get_list(interaction_e::vision));
            for (auto& m_item : vl->m_items) {
                auto current = static_cast<Vision_component*>(m_item)->m_value;
                radius = (std::max)(radius, current.radius);
            }
            Path::instance().calculate(m_map, m_object, c, m_goal, radius);
            const auto path = Path::instance().get_path();
            if (path) {
                if (path->size() >= 2) {
                    const auto p = new Parameter(parameter_type_e::position);
                    (*p)[0].set(m_object);
                    (*p)[1].set((*path)[path->size() - 2]);
                    //Application::instance().m_action_manager->add(m_object, new GameTask(Application::instance().m_actions[action_e::move], P));
                    m_action_controller->set((*p)[0].m_object, Application::instance().m_actions[action_e::move], p);
                } else {
                    m_goal = nullptr;
                    m_memory_goal_cell = nullptr;
                }
            }
            Path::instance().m_heap.m_items.clear();
        }
    }
    m_action_controller->update();
}

void AI_enemy::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_action_controller) {
        if (m_action_controller->m_serialization_index > 0) {
            m_action_controller->reset_serialization_index();
        }
    }
}

AI_trap::AI_trap() {};

GameObject* AI_trap::find_goal() { return nullptr; };

void AI_trap::create()
{
    m_object->set_state(object_state_e::off);
    for (auto& m_item : m_object->cell()->m_items) {
        if (m_object != m_item && m_item->m_name != u"floor") {
            m_object->set_state(object_state_e::on);
        }
    }
    if (m_object->m_active_state->m_state == object_state_e::on) {
        for (auto& m_item : m_object->cell()->m_items) {
            if (m_object != m_item && m_item->m_name != u"floor") {
                /*?1? P_unit_interaction* p = new P_unit_interaction();
				p->m_unit = m_object;
				p->m_object = (*item);
				p->m_unit_body_part = nullptr;
				Application::instance().m_action_manager->add(new GameTask(Application::instance().m_actions[action_e::hit], p));*/
            }
        }
    }
};

iPacker& AI_trap::get_packer()
{
    return Packer<AI_trap>::instance();
}

IVISITABLE_IMPL(AI_trap);

AI_manager::AI_manager()
{
    m_fov_qualifiers.push_back(new predicate_t([](GameObject* object) -> bool { return !object->get_stat(object_tag_e::seethrough_able); }, 0));
    m_fov_qualifiers.push_back(new predicate_t([](GameObject* object) -> bool { return false; }, 1));

    m_path_qualifiers.push_back(new predicate_t([](GameObject* object) -> bool { return !object->get_stat(object_tag_e::pass_able); }, 0));
    m_path_qualifiers.push_back(new predicate_t([](GameObject* object) -> bool { return !object->get_stat(object_tag_e::pass_able) && object->m_name != u"wall"; }, 1));
}

iPacker& AI_enemy::get_packer()
{
    return Packer<AI_enemy>::instance();
}

IVISITABLE_IMPL(AI_enemy);
