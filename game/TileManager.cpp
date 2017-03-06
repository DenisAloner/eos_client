#include "game/TileManager.h"
#include "game/GameObject.h"


TileManager::TileManager():m_animation(animation_e::idle)
{
}


TileManager::~TileManager()
{
}


bool TileManager::init(const std::string& filename, object_direction_e direction, int frame)
{
	m_tiles.resize(1);
	m_tiles[0] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
	return true;
}

void TileManager::set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction)
{
	tile.unit = m_tiles[get_tile_index(direction, frame)];
	double sx = 1.0 / obj->m_active_state->m_tile_size.w;
	tile.coordinat[0] = 0.0;
	tile.coordinat[1] = 1.0;
	tile.coordinat[2] = 1.0;
	tile.coordinat[3] = 0.0;
}

TileManager_Single::TileManager_Single()
{
	
}

int TileManager_Single::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;
}


TileManager_Single_animate::TileManager_Single_animate()
{
}

bool TileManager_Single_animate::init(const std::string& filename, object_direction_e direction, int frame)
{
	m_tiles.resize(frame);
	m_tiles[frame] = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
	return true;
}

int TileManager_Single_animate::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return frame * (m_frame/24.0);
}

TileManager_Single_png::TileManager_Single_png()
{
}

int TileManager_Single_png::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;

}

bool TileManager_Single_png::init(const std::string& filename, object_direction_e direction, int frame)
{
	m_tiles.resize(1);
	m_tiles[0] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
	return true;
}

TileManager_rotate8_animate::TileManager_rotate8_animate()
{
}

int TileManager_rotate8_animate::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;

}

bool TileManager_rotate8_animate::init(const std::string& filename, object_direction_e direction, int frame)
{
	m_frame = frame;
	m_tiles.resize(2);
	for(int i=0;i<2;++i)
	{
		m_tiles[i] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + std::to_string(i + 1) + ".png");
	}
	return true;
}

void TileManager_rotate8_animate::set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction)
{
	tile.unit = m_tiles[static_cast<int>(m_animation)];
	int ix = frame * (m_frame / 24.0);
	int iy = int(direction);
	double dx = 1.0 / m_frame;
	double dy = 1.0 / 8.0;
	tile.coordinat[0] = ix*dx;
	tile.coordinat[1] = 1.0 - ((iy)*dy);
	tile.coordinat[2] = (ix + 1)*dx;
	tile.coordinat[3] = 1.0 - ((iy + 1)*dy);
}

TileManager_equilateral_animate::TileManager_equilateral_animate()
{
}

int TileManager_equilateral_animate::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;

}

bool TileManager_equilateral_animate::init(const std::string& filename, object_direction_e direction, int frame)
{
	m_frame = frame;
	m_tiles.resize(1);
	m_tiles[0] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
	return true;
}

void TileManager_equilateral_animate::set_tile(tile_t& tile, GameObject* obj, int frame, const object_direction_e& direction)
{
	tile.unit = m_tiles[static_cast<int>(m_animation)];
	int ix = frame * (m_frame / 24.0);
	double dx = 1.0 / m_frame;
	tile.coordinat[0] = ix*dx;
	tile.coordinat[1] = 1.0;
	tile.coordinat[2] = (ix + 1)*dx;
	tile.coordinat[3] = 0.0;
}