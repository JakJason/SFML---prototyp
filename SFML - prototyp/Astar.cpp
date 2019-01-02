#include "Chunk.h"
#include "Map.h"

typedef struct node {
	Chunk* ch;
	node* parent = NULL;
	node* adjacent[4] = { NULL, NULL, NULL, NULL };


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

	double hCost(int x, int y) {
		double h = sqrt( (ch->get_position_x() - x)*(ch->get_position_x() - x) + ((ch->get_position_y() - y)*(ch->get_position_y() - y) ));
		return h;
	}


};

bool 