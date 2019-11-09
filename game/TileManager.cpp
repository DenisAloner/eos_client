#include "game/TileManager.h"
#include "Application.h"

void TileManager::vector_gl_uint_from_json(const std::u16string& value, std::vector<GLuint>& prop, SerializationContext& context)
{
    auto temp = value;
    const auto s = read_array(temp);
    if (s) {
        prop.resize(s->size());
        auto i = 0;
        for (const auto& element : *s) {
            auto&& name = utf16_to_cp1251(get_value(element));
            prop[i] = Application::instance().m_graph->png_texture_load(
                FileSystem::instance().m_resource_path + "Tiles\\" + name + ".png");
            i += 1;
        }
        delete s;
    }
}

void TileManager::vector_gl_uint_from_binary(const std::string& value, std::vector<GLuint>& prop, std::size_t& pos, SerializationContext& context)
{
}

TileManager_Single_Atlas::TileManager_Single_Atlas()
{
    m_tiles.resize(1);
}

int TileManager_Single_Atlas::get_tile_index(const object_direction_e& direction, const int& frame)
{
    return 0;
}

void TileManager_Single_Atlas::set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction)
{
    /*tile.unit = m_tiles[0];
	tile.coordinates[0] = m_frame.x / 38.0;
	tile.coordinates[1] = m_frame.y / 264.0;
	tile.coordinates[2] = (m_frame.x + m_frame.w) / 38.0;
	tile.coordinates[3] = (m_frame.y + m_frame.h) / 264.0;*/
}

void TileManager_Single_Atlas::set_tile(quad_t& quad, GameObject* obj, int pos, const object_direction_e& direction)
{
    const auto x1 = m_frame.x / 950.0;
    const auto y1 = m_frame.y / 352.0;
    const auto x2 = (m_frame.x + 38) / 950.0;
    const auto y2 = (m_frame.y + 44) / 352.0;
    quad.vertex[0].texture_coordinates[0] = x1;
    quad.vertex[0].texture_coordinates[1] = y2;
    quad.vertex[0].texture_coordinates[2] = static_cast<double>(m_layer);

    quad.vertex[1].texture_coordinates[0] = x1;
    quad.vertex[1].texture_coordinates[1] = y1;
    quad.vertex[1].texture_coordinates[2] = static_cast<double>(m_layer);

    quad.vertex[2].texture_coordinates[0] = x2;
    quad.vertex[2].texture_coordinates[1] = y1;
    quad.vertex[2].texture_coordinates[2] = static_cast<double>(m_layer);

    quad.vertex[3].texture_coordinates[0] = x2;
    quad.vertex[3].texture_coordinates[1] = y2;
    quad.vertex[3].texture_coordinates[2] = static_cast<double>(m_layer);
}

iPacker& TileManager_Single_Atlas::get_packer()
{
    return Packer<TileManager_Single_Atlas>::instance();
}

TileManager_Atlas_Rotatable::TileManager_Atlas_Rotatable()
{
    m_tiles.resize(1);
}

int TileManager_Atlas_Rotatable::get_tile_index(const object_direction_e& direction, const int& frame)
{
    return 0;
}

void TileManager_Atlas_Rotatable::set_tile(tile_t& tile, GameObject* obj, int frame,
    const object_direction_e& direction)
{
}

void TileManager_Atlas_Rotatable::set_tile(quad_t& quad, GameObject* obj, int pos,
    const object_direction_e& direction)
{
    const GLdouble x1 = (m_frame.x + pos * 38) / 950.0;
    const GLdouble y1 = (m_frame.y + static_cast<int>(direction) * 44) / 352.0;
    const GLdouble x2 = (m_frame.x + (pos + 1) * 38) / 950.0;
    const GLdouble y2 = (m_frame.y + (static_cast<int>(direction) + 1) * 44) / 352.0;
    quad.vertex[0].texture_coordinates[0] = x1;
    quad.vertex[0].texture_coordinates[1] = y2;
    quad.vertex[0].texture_coordinates[2] = static_cast<double>(m_layer);

    quad.vertex[1].texture_coordinates[0] = x1;
    quad.vertex[1].texture_coordinates[1] = y1;
    quad.vertex[1].texture_coordinates[2] = static_cast<double>(m_layer);

    quad.vertex[2].texture_coordinates[0] = x2;
    quad.vertex[2].texture_coordinates[1] = y1;
    quad.vertex[2].texture_coordinates[2] = static_cast<double>(m_layer);

    quad.vertex[3].texture_coordinates[0] = x2;
    quad.vertex[3].texture_coordinates[1] = y2;
    quad.vertex[3].texture_coordinates[2] = static_cast<double>(m_layer);
}

iPacker& TileManager_Atlas_Rotatable::get_packer()
{
    return Packer<TileManager_Atlas_Rotatable>::instance();
}

TileManager::TileManager()
    : m_animation(animation_e::idle)
{
}

TileManager::~TileManager()
{
}

//bool TileManager::init(const std::string& filename, object_direction_e direction, int frame)
//{
//	m_tiles.resize(1);
//	m_tiles[0] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
//	return true;
//}

void TileManager::set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction)
{
    tile.unit = m_tiles[get_tile_index(direction, frame)];
    double sx = 1.0 / obj->m_active_state->m_tile_size.w;
    tile.coordinates[0] = 0.0;
    tile.coordinates[1] = 1.0;
    tile.coordinates[2] = 1.0;
    tile.coordinates[3] = 0.0;
}

void TileManager::set_tile(quad_t& quad, GameObject* obj, int pos, const object_direction_e& direction)
{
}

TileManager_Single::TileManager_Single()
{
}

int TileManager_Single::get_tile_index(const object_direction_e& direction, const int& frame)
{
    return 0;
}

iPacker& TileManager_Single::get_packer()
{
    return Packer<TileManager_Single>::instance();
}

//bool TileManager_Single::init(const std::string& filename, object_direction_e direction, int frame)
//{
//	m_tiles.resize(1);
//	m_tiles[0] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
//	return true;
//}

TileManager_rotate8_animate::TileManager_rotate8_animate()
{
}

int TileManager_rotate8_animate::get_tile_index(const object_direction_e& direction, const int& frame)
{
    return 0;
}

//bool TileManager_rotate8_animate::init(const std::string& filename, object_direction_e direction, int frame)
//{
//	m_frame = frame;
//	m_tiles.resize(2);
//	for(int i=0;i<2;++i)
//	{
//		m_tiles[i] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + std::to_string(i + 1) + ".png");
//	}
//	return true;
//}

void TileManager_rotate8_animate::set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction)
{
    tile.unit = m_tiles[static_cast<int>(m_animation)];
    int ix = frame * (m_frame / 24.0);
    int iy = int(direction);
    double dx = 1.0 / m_frame;
    double dy = 1.0 / 8.0;
    tile.coordinates[0] = ix * dx;
    tile.coordinates[1] = 1.0 - ((iy)*dy);
    tile.coordinates[2] = (ix + 1) * dx;
    tile.coordinates[3] = 1.0 - ((iy + 1) * dy);
}

iPacker& TileManager_rotate8_animate::get_packer()
{
    return Packer<TileManager_rotate8_animate>::instance();
}

TileManager_equilateral_animate::TileManager_equilateral_animate()
{
}

int TileManager_equilateral_animate::get_tile_index(const object_direction_e& direction, const int& frame)
{
    return 0;
}

//bool TileManager_equilateral_animate::init(const std::string& filename, object_direction_e direction, int frame)
//{
//	m_frame = frame;
//	m_tiles.resize(1);
//	m_tiles[0] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
//	return true;
//}

void TileManager_equilateral_animate::set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction)
{
    tile.unit = m_tiles[static_cast<int>(m_animation)];
    int ix = frame * (m_frame / 24.0);
    double dx = 1.0 / m_frame;
    tile.coordinates[0] = ix * dx;
    tile.coordinates[1] = 1.0;
    tile.coordinates[2] = (ix + 1) * dx;
    tile.coordinates[3] = 0.0;
}

iPacker& TileManager_equilateral_animate::get_packer()
{
    return Packer<TileManager_equilateral_animate>::instance();
}
