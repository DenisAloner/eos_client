#pragma once
#include<iostream>
#include<vector>
#include "GameMap.h"
#include "GameObject.h"
#include "Application.h"
#include "ApplicationGUI.h"
#include "ActionManager.h"

class GameMap;
class GameObject;

class Node
{
public:
	Node(void);
	~Node(void);
};

struct NODE
{
	int TotalCost;
	int X, Y;
	int StartCost;
	int ParentX, ParentY;

};

class Path
{
private:
	std::vector<NODE> OpenList, ClosedList;
	
	

public:
	int start_x, start_y;
	int goal_x, goal_y;
	GameMap* m_map;

	Path();
	virtual ~Path();

	void InsertIntoClosedList(int total_cost, int start_cost, int x, int y);
	void InsertIntoOpenList(int x, int y);
	std::vector<NODE> DeleteElement(std::vector<NODE> nodesList, int x, int y);
	bool IsElementExits(int x, int y);
	NODE MinNode(std::vector<NODE>);
	void Create();
	void ClearAll();
	std::vector<NODE> BackTrack();
	NODE GetNodeAt(int x, int y);

};

class AI
{
public:

	std::vector<NODE> m_path;
	Path m_path_creator;
	GameObject* m_object;

	void create();

	GameMap* m_map;

	AI();
	~AI();
};

