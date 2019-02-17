#pragma once

#include "../overpeek-engine/engine.h"

class Pathfinder {
private:
	struct Node {
		bool visited = false;
		bool open = false;
		bool walkable;

		float g_cost;
		float h_cost;
		
		unsigned int x;
		unsigned int y;

		std::vector<Node*> neighbours;
		Node* parent;

		float f_cost() { 
			return g_cost + h_cost; 
		}

		Node(unsigned int _x, unsigned int _y, bool _walkable) {
			x = _x;
			y = _y;
			walkable = _walkable;
		}
	};
	std::vector<std::vector<Node*>> p_nodemap;

	std::vector<Node*> p_openSet;
	std::vector<Node*> p_closedSet;
	
	Node* p_startnode;
	Node* p_endnode;
	Node* p_curnode;
	int p_currentnode_x;
	int p_currentnode_y;

	int step();

	float distance(Node *nodeA, Node *nodeB) {
		float dx = abs((int)nodeA->x - (int)nodeB->x);
		float dy = abs((int)nodeA->y - (int)nodeB->y);

		if (dx > dy) return 1.4*dy + dx - dy;
		return 1.4*dx + dy - dx;

		//return abs((int)nodeA->x - (int)nodeB->x) + abs((int)nodeA->y - (int)nodeB->y);
	}

public:
	bool path_found;

	//Beginds A* algorithm from startnode on map to find path to endnode on map, but doesn't run any steps
	//grid_scale is scalar of the node map to search from, if map edges are startnode and endnode grid_scale is 1, else gridscale is distance from startnode to endnode plus grid_scale
	Pathfinder(unsigned int startnode_x, unsigned int startnode_y, unsigned int endnode_x, unsigned int endnode_y, unsigned int grid_scale);
	
	//Run n number of steps to find path to endnode from startnode
	//Returns 1 if path is found, -1 if no path is available, 0 if it can continue
	int runNSteps(unsigned int n);

	//Runs steps until path from startnode to endnode have been found or if it finds out that there is no path
	bool runUnitlEnd();

	//
	void debugRender(oe::Renderer *renderer, float offx, float offy);
	
	//Gets the found path
	std::vector<glm::vec2> *retrace();

};