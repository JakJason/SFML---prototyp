#include "MoveOrder.h"
#include "GameObject.h"
#include "Map.h"

#define GLOBAL_FRAME_RATE 60

MoveOrder::MoveOrder() {

}

MoveOrder::MoveOrder(int i0, int j0, int i1, int j1, Map map) {

	start_x = i0;
	start_y = j0;

	final_x = i1;
	final_y = j1;

	next = &map.get_chunks()[i0][j0];
}

int MoveOrder::get_final_x() {
	return final_x;
}

int MoveOrder::get_final_y() {
	return final_y;
}

void MoveOrder::set_final_x(int x) {
	final_x = x;
}

void MoveOrder::set_final_y(int y) {
	final_y = y;
}


int MoveOrder::get_start_x() {
	return start_x;
}

int MoveOrder::get_start_y() {
	return start_y;
}

void MoveOrder::set_start_x(int x) {
	start_x = x;
}

void MoveOrder::set_start_y(int y) {
	start_y = y;
}

int MoveOrder::get_mid_x() {
	return mid_x;
}

int MoveOrder::get_mid_y() {
	return mid_y;
}

float MoveOrder::get_speed() {
	return speed_per_second;
}

void MoveOrder::set_next(Chunk *c) {
	next = c;
	mid_x = c->get_position_x();
	mid_y = c->get_position_y();
}

Chunk* MoveOrder::get_next() {
	return next;
}
