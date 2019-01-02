#pragma once
#include <SFML/Graphics.hpp>
#include "GenericOrder.h"
#include "Chunk.h"
#include "Map.h"

class MoveOrder : public GenericOrder {

private:
	float speed_per_second = 5;

	int start_x = -1;
	int start_y = -1;

	int mid_x = 0;
	int mid_y = 0;

	int final_x = -1;
	int final_y = -1;

	Chunk* next = nullptr;

public:

	MoveOrder();

	MoveOrder(int i0, int j0, int i1, int j1, Map map);

	int get_final_x();

	int get_final_y();

	void set_final_x(int x);

	void set_final_y(int y);

	int get_start_x();

	int get_start_y();

	void set_start_x(int x);

	void set_start_y(int y);

	int get_mid_x();

	int get_mid_y();

	float get_speed();

	void set_next(Chunk *c);

	Chunk* get_next();

};