#include "../pch.h"
#include "aStar.h"

#include "../logic/game.h"
#include "../world/map.h"

Pathfinder::Pathfinder(unsigned int startnode_x, unsigned int startnode_y, unsigned int endnode_x, unsigned int endnode_y, unsigned int grid_scale) {
	path_found = false;
	failed = false;

	//Setup nodemap
	for (unsigned int x = min(startnode_x, endnode_x) - grid_scale; x < max(startnode_x, endnode_x) + grid_scale; x++)
	{
		std::vector<Node*> column;
		for (unsigned int y = min(startnode_y, endnode_y) - grid_scale; y < max(startnode_y, endnode_y) + grid_scale; y++)
		{
			Node *newNode = new Node(x, y, !Database::objects[Game::getMap()->getTile(x, y)->m_object].wall);

			//Add node to list
			column.push_back(newNode);

			//Startnode
			if (x == startnode_x && y == startnode_y) p_startnode = newNode;
			
			//Endnode
			if (x == endnode_x && y == endnode_y) p_endnode = newNode;
		}
		p_nodemap.push_back(std::move(column));
	}

	for (int x = 0; x < p_nodemap.size(); x++)
	{
		for (int y = 0; y < p_nodemap[x].size(); y++)
		{
			//right
			if (x + 1 < p_nodemap.size() && x + 1 >= 0) {
				p_nodemap[x][y]->neighbours.push_back(p_nodemap[x + 1][y]);

				//up
				if (y + 1 < p_nodemap[x].size() && y + 1 >= 0) {
					p_nodemap[x][y]->neighbours.push_back(p_nodemap[x][y + 1]);

					if (p_nodemap[x][y + 1]->walkable) {
						p_nodemap[x][y]->neighbours.push_back(p_nodemap[x + 1][y + 1]);
					}
				}

				//down
				if (y - 1 < p_nodemap[x].size() && y - 1 >= 0) {
					p_nodemap[x][y]->neighbours.push_back(p_nodemap[x][y - 1]);

					if (p_nodemap[x][y - 1]->walkable) {
						p_nodemap[x][y]->neighbours.push_back(p_nodemap[x + 1][y - 1]);
					}
				}
			}
			
			//left
			if (x - 1 < p_nodemap.size() && x - 1 >= 0) {
				p_nodemap[x][y]->neighbours.push_back(p_nodemap[x - 1][y]);

				//up
				if (y + 1 < p_nodemap[x].size() && y + 1 >= 0) {
					if (p_nodemap[x][y + 1]->walkable) {
						p_nodemap[x][y]->neighbours.push_back(p_nodemap[x - 1][y + 1]);
					}
				}

				//down
				if (y - 1 < p_nodemap[x].size() && y - 1 >= 0) {
					if (p_nodemap[x][y - 1]->walkable) {
						p_nodemap[x][y]->neighbours.push_back(p_nodemap[x - 1][y - 1]);
					}
				}
			}
		}
	}

	if (!p_startnode || !p_endnode) {
		oe::Logger::out(oe::warning, "Startnode or endnode lost");
		failed = true;
	}

	//Sets
	p_openSet.push_back(p_startnode);

}

int Pathfinder::step() {
	if (p_openSet.size() <= 0) return -1;

	int curnode = 0;
	p_curnode = p_openSet[0];
	for (int i = 1; i < p_openSet.size(); i++)
	{
		if (p_openSet[i]->f_cost() < p_curnode->f_cost() || (p_openSet[i]->f_cost() == p_curnode->f_cost() && p_openSet[i]->h_cost < p_curnode->h_cost)) {
			curnode = i;
			p_curnode = p_openSet[i];
		}
	}

	p_curnode->visited = true;
	p_openSet.erase(p_openSet.begin() + curnode);

	if (p_curnode == p_endnode) {
		path_found = true;
		return 1;
	}

	for (int i = 0; i < p_curnode->neighbours.size(); i++)
	{
		if (!p_curnode->neighbours[i]->walkable || p_curnode->neighbours[i]->visited) {
			continue;
		}

		float neighbourNewCost = p_curnode->g_cost + distance(p_curnode, p_curnode->neighbours[i]);
		if (neighbourNewCost < p_curnode->neighbours[i]->g_cost || !p_curnode->neighbours[i]->open) {
			p_curnode->neighbours[i]->g_cost = neighbourNewCost;
			p_curnode->neighbours[i]->h_cost = distance(p_curnode->neighbours[i], p_endnode);
			p_curnode->neighbours[i]->parent = p_curnode;

			if (!p_curnode->neighbours[i]->open) {
				p_curnode->neighbours[i]->open = true;
				p_openSet.push_back(p_curnode->neighbours[i]);
			}
		}
	}
	return 0;
}

int Pathfinder::runNSteps(unsigned int n) {
	for (int i = 0; i < n; i++)
	{
		int result = step();

		if (result == 1) return 1;
		if (result == 0) continue;
		if (result == -1) return -1;
	}
	return 0;
}

bool Pathfinder::runUnitlEnd() {
	int result = 0;
	while (p_openSet.size() > 0 && path_found == false)
	{
		result = step();
	}
	return result;
}

void Pathfinder::debugRender(oe::Renderer *renderer, float offx, float offy) {
	Node *curNode = p_curnode;
	bool first = true;
	while (curNode != p_startnode) {
		glm::vec3 pos = glm::vec3((curNode->x + offx) * TILE_SIZE, (curNode->y + offy) * TILE_SIZE, 0.0f);

		if (first) {
			renderer->lineRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, glm::vec4(1.0, 0.5, 0.0, 0.2)));
		}
		else {
			renderer->lineRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, glm::vec4(1.0, 0.5, 0.0, 0.2)));
			renderer->lineRenderer->submitVertex(oe::VertexData(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(0.0f, 0.0f), 20, glm::vec4(1.0, 0.5, 0.0, 0.2)));
		}

		curNode = curNode->parent;

		first = false;
	}
}

std::vector<glm::vec2> *Pathfinder::retrace() {
	std::vector<glm::vec2> *path = new std::vector<glm::vec2>;

	Node *curNode = p_curnode;
	while (curNode != p_startnode) {
		path->push_back(glm::vec2(curNode->x, curNode->y));
		curNode = curNode->parent;
	}
	return path;
}