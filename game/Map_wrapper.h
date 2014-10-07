#pragma once
#include <list>
#include <map>

template<typename _key, typename _val>
class Map_wrapper
{
private:
	typedef std::list<_val> list_t;
	typedef std::map<_key, list_t> map_t;

public:
	Map_wrapper();
	~Map_wrapper();

	inline void append(_key key, const _val &val) { m_map[key].push_back(val); }
	inline void remove(_key key, const _val &val) { m_map[key].remove(val); if (m_map[key].empty()) m_map.erase(key); }
	inline const list_t& elements(_key key) { return m_map[key]; }
private:
	map_t m_map;
};