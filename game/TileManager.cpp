#include "game/TileManager.h"
#include "game/GameObject.h"


TileManager::TileManager():m_animation(animation_e::idle)
{
}


TileManager::~TileManager()
{
}


bool TileManager::load_from_file(const std::string& filename, object_direction_e direction, int frame)
{
	m_tiles[get_tile_index(direction,frame)] = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".bmp");
	return true;
}


bool TileManager::load_from_file(const std::string& filename, object_direction_e direction, int frame, std::string& ext)
{
	LOG(FATAL) << "Метод не описан";
	return false;
}

void TileManager::set_tile(tile_t& tile, GameObject* obj, int frame, int shift)
{
	tile.unit = m_tiles[get_tile_index(obj->m_direction, frame)];
	double sx = 1.0 / obj->m_active_state->m_tile_size.w;
	tile.coordinat[0] = 0.0;
	tile.coordinat[1] = 1.0;
	tile.coordinat[2] = 1.0;
	tile.coordinat[3] = 0.0;
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
	m_tiles = new GLuint[1];
}

int TileManager_Single::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;
}


TileManager_Single_animate::TileManager_Single_animate(int frame):m_frame(frame)
{
	m_tiles = new GLuint[frame];
}

bool TileManager_Single_animate::load_from_file(const std::string& filename, object_direction_e direction, int frame)
{
	m_tiles[frame] = Application::instance().m_graph->load_texture(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".bmp");
	return true;
}

bool TileManager_Single_animate::load_from_file(const std::string& filename, object_direction_e direction, int frame, std::string& ext)
{
	m_tiles[frame] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
	return true;
}

int TileManager_Single_animate::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return frame * (m_frame/24.0);
}


TileManager_Map::TileManager_Map()
{
	m_tiles = new GLuint[32];
}

int TileManager_Map::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return static_cast<int>(direction) * 4 + frame / 7.0*3.0;
}


TileManager_rotating::TileManager_rotating()
{
	m_tiles = new GLuint[4];
}

int TileManager_rotating::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return static_cast<int>(direction)/2;
}

TileManager_rotating8::TileManager_rotating8()
{
	m_tiles = new GLuint[8];
}

int TileManager_rotating8::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return static_cast<int>(direction);
}

TileManager_Single_png::TileManager_Single_png()
{
	m_tiles = new GLuint[1];
}

int TileManager_Single_png::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;

}

bool TileManager_Single_png::load_from_file(const std::string& filename, object_direction_e direction, int frame)
{
	m_tiles[frame] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + ".png");
	return true;
}

TileManager_rotate8_animate::TileManager_rotate8_animate(int frame) :m_frame(frame)
{
	m_tiles = new GLuint[2];
}

int TileManager_rotate8_animate::get_tile_index(const object_direction_e& direction, const int& frame)
{
	return 0;

}

bool TileManager_rotate8_animate::load_from_file(const std::string& filename, object_direction_e direction, int frame)
{
	for(int i=0;i<2;++i)
	{
		m_tiles[i] = Application::instance().m_graph->png_texture_load(FileSystem::instance().m_resource_path + "Tiles\\" + filename + std::to_string(i + 1) + ".png");
	}
	return true;
}

void TileManager_rotate8_animate::set_tile(tile_t& tile, GameObject* obj, int frame, int shift)
{
	tile.unit = m_tiles[static_cast<int>(m_animation)];
	int ix = frame * (m_frame / 24.0);
	int iy = int(obj->m_direction);
	double dx = 1.0 / m_frame;
	double dy = 1.0 / 8.0;
	tile.coordinat[0] = ix*dx;
	tile.coordinat[1] = 1.0 - ((iy)*dy);
	tile.coordinat[2] = (ix+1)*dx;
	tile.coordinat[3] = 1.0 - ((iy + 1)*dy);
}