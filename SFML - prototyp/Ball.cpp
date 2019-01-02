#include "Ball.h"
#include <iostream>
#define PI 3.14159265
#define GLOBAL_FRAME_RATE 60


Ball::Ball() {
	pos_x = 0;
	pos_y = 0;
}

Ball::Ball(sf::Texture t, int x, int y, double x_s, double y_s) {
	
	pos_x = x;	
	pos_y = y;
	sprite.setTexture(t);
	sprite.setPosition(x_s, y_s);
}

int Ball::get_position_x() {
	return pos_x;
}

int Ball::get_position_y() {
	return pos_y;
}

sf::Sprite Ball::get_sprite() {
	return sprite;
}

void Ball::set_sprite(sf::Texture& t, int i, int j) {
	sprite.setTexture(t);
	sprite.setPosition(i, j);
}

MoveOrder* Ball::get_current_order() {
	return order;
}

void Ball::set_current_order(MoveOrder *o) {
	order = o;
}

void Ball::drawTo(sf::RenderWindow &window) {
	window.draw(sprite);
}

void Ball::drawToActive(sf::RenderWindow &window, sf::Sprite &active) {
	active.setPosition(get_sprite().getPosition().x, get_sprite().getPosition().y);
	window.draw(active);
	window.draw(sprite);
}

void Ball::place_on_chunk(sf::RenderWindow &window, sf::Sprite active, Chunk c, bool b) {

	sprite.setPosition(c.get_sprite().getPosition().x - sprite.getTexture()->getSize().x / 2, c.get_sprite().getPosition().y);
	pos_x = c.get_position_x();
	pos_y = c.get_position_y();

	if (b == true) {
		drawToActive(window, active);
	}
	else {
		drawTo(window);
	}
}

void Ball::check_status(Map map, sf::RenderWindow &window, bool b) {
	if (order != nullptr) {
		apply(*order, map, window, b);
	}
	else {
	}
}

void Ball::destroy() {

}

void Ball::apply(MoveOrder& o, Map map, sf::RenderWindow &window, bool b) {
	if (order != nullptr) {
		if (o.get_start_x() == o.get_final_x() && o.get_start_y() == o.get_final_y()) {
			cancel(o);
			//std::cout << " 1 \n";
		}
		else if (o.get_start_x() == o.get_mid_x() && o.get_start_y() == o.get_mid_y()) {
			give(o, map);
			//std::cout << " 2 \n";
		}
		else {
			place_on_chunk(window, get_sprite(), *o.get_next(), b);
			o.set_start_x(o.get_mid_x());
			o.set_start_y(o.get_mid_y());
			//std::cout << " 4 \n";
		}
	}

}

void Ball::cancel(MoveOrder& o) {
	/*
	if (o.get_final_x() != o.get_next()->get_position_x() || o.get_final_y() != o.get_next()->get_position_y()) {
		o.set_final_x(o.get_next()->get_position_x());
		o.set_final_x(o.get_next()->get_position_x());
	}
	else if (o.get_final_x() == o.get_next()->get_position_x() && o.get_final_y() == o.get_next()->get_position_y()) {
		o.set_next(nullptr);
	}
	*/
	order = nullptr;
}

void Ball::give(MoveOrder& o, Map map) {
	cancel(*get_current_order());
	set_current_order(&o);
	o.set_next(map.find_path(pos_x, pos_y, o.get_final_x(), o.get_final_y()));
}

void Ball::remove(MoveOrder& o) {
	order = nullptr;
	delete &o;
}