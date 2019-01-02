#include "Map.h"
#include "Chunk.h"
#include <iostream>
#include <list>


typedef struct node {
	Chunk* ch;
	node* parent = NULL;
	node* adjacent[4] = { NULL, NULL, NULL, NULL };

	bool checked = false;
	double g = 0;
	double f = 0;

	bool isValid(Map map) {
		if (ch->get_position_x() > map.get_width() || ch->get_position_x() < 0) {
			return false;
		}
		if (ch->get_position_y() > map.get_height() || ch->get_position_y() < 0) {
			return false;
		}
		return true;
	}

	bool isDest(int x, int y) {
		if (ch->get_position_x() == x && ch->get_position_y() == y) {
			return true;
		}
		return false;
	}

	bool inLists(std::list<node*>l) {
		for (std::list<node*>::iterator iter = l.begin(); iter != l.end(); iter++) {
			if (*iter == this) {
				return true;
			}
		}
		return false;
	}

	double hCost(int x, int y) {
		return sqrt((x - ch->get_position_x())*(x - ch->get_position_x()) + ((y - ch->get_position_y())*(y - ch->get_position_y())));
	}

	void set(node* p, float n_g, float n_f) {
		parent = p;
		g = n_g;
		f = n_f;
	}

};
typedef bool(*checkpath)(node* node);



Map::Map() {

}

Map::Map(int width, int height, sf::Texture& t) {
	map_width = width;
	map_height = height;

	chunks = new Chunk*[map_width];
	for (int i = 0; i < map_width; i++) {
		chunks[i] = new Chunk[map_height];
		for (int j = 0; j < map_height; j++) {
			chunks[i][j].set_position_x(i);
			chunks[i][j].set_position_y(j);
			chunks[i][j].set_sprite(t, i, j);
		}
	}
	std::cout << map_width*t.getSize().x << " " << map_height*t.getSize().y << "\n";
}

void Map::drawTo(sf::RenderWindow& window) {
	for (int i = 0; i < map_width; i++) {
		for (int j = 0; j < map_height; j++) {
			chunks[i][j].drawTo(window);
		}
	}
}

Chunk** Map::get_chunks() {
	return chunks;
}

Chunk* Map::get_pointer_to_chunk(int i, int j) {

	Chunk* c = &chunks[i][j];

	return c;
}

int Map::get_width() {
	return map_width;
}

int Map::get_height() {
	return map_height;
}

Chunk* Map::get_pointer_to_chunk_with_mouse_on_it(sf::RenderWindow &window,  sf::Vector2i mouse_possition, sf::View v) {
	for (int i = 0; i < map_width; i++) {
		for (int j = 0; j < map_height; j++) {
			if (chunks[i][j].get_sprite().getGlobalBounds().contains(window.mapPixelToCoords(mouse_possition, v))) {
				return &chunks[i][j];
			}
		}
	}
	Chunk c = Chunk(-1, -1);
	return &c;
}

Chunk* Map::find_path(int x0, int y0, int x1, int y1) {

	std::list<node*> path;
	node **nodes;
	nodes = new node*[map_width];
	double new_g = 0;
	//initialize node map
	for (int i = 0; i < map_width; i++) {
		nodes[i] = new node[map_height];
		for (int j = 0; j < map_height; j++) {
			nodes[i][j].ch = &(chunks[i][j]);
		}
	}

	for (int i = 0; i < map_width; i++) {
		for (int j = 0; j < map_height; j++) {
			if (nodes[i][j].isValid(*this)) {
				if (i > 0) {
					if(nodes[i - 1][j].isValid(*this)) {
						nodes[i][j].adjacent[0] = &nodes[i - 1][j];
					}
				}
				if (i < map_width - 1) {
					if (nodes[i + 1][j].isValid(*this)) {
						nodes[i][j].adjacent[2] = &nodes[i + 1][j];
					}
				}
				if (j > 0) {
					if (nodes[i][j - 1].isValid(*this)) {
						nodes[i][j].adjacent[1] = &nodes[i][j - 1];
					}
				}
				if (j < map_height - 1) {
					if (nodes[i][j + 1].isValid(*this)) {
						nodes[i][j].adjacent[3] = &nodes[i][j + 1];
					}
				}
			}
		}
	}

	std::list<node*> open;

	open.push_front(&nodes[x0][y0]);

	node *Q = new node();

	//std::cout << x0 <<" \n";
	//std::cout << y0 <<" \n";
	//std::cout << x1 <<" \n";
	//std::cout << y1 <<" \n";

	while (!open.empty()) {
		open.sort([](node* first, node* second) { return first->f < second->f; });
		Q = open.front();
		Q->checked = true;

		if (Q->isDest(x1, y1)) { break; }

		open.pop_front();
		for (int a = 0; a < 4; a++) {
			if (Q->adjacent[a] != NULL) {
				if (Q->adjacent[a]->checked == true){ //|| checkpath == true)) {
					continue;
				}
				else if (Q->adjacent[a]->inLists(open) == false) {
					open.push_front(Q->adjacent[a]);
					Q->adjacent[a]->set(Q, Q->adjacent[a]->g + 1.0, Q->adjacent[a]->g + Q->adjacent[a]->hCost(x1, y1));
				}
				else {
					new_g = Q->adjacent[a]->g + 1.0;
					if (new_g < Q->adjacent[a]->g) {
						Q->adjacent[a]->set(Q, new_g, Q->adjacent[a]->g + Q->adjacent[a]->hCost(x1, y1));
					}
				}
			}
			else {
				continue;
			}
		}
	}

	while (true) {
		if (Q->parent == NULL) break;
		path.push_front(Q);
		Q = Q->parent;
	}
	
	Chunk* c = path.front()->ch;

	//std::cout << " szukam \n";

	return c;

}