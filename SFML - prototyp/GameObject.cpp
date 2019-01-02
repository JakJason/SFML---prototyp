#include "GameObject.h"


void GameObject::set_sprite(sf::Sprite s) {
	sprite = s;
}

void GameObject::set_sprite(sf::Texture& t, double x_s, double y_s) {
	sprite.setTexture(t);
	sprite.setPosition(x_s, y_s);
}

void GameObject::set_sprite_position(double x_s, double y_s) {
	sprite.setPosition(x_s, y_s);
}

void GameObject::set_position_x(int x) {
	pos_x = x;
}

void GameObject::set_position_y(int y) {
	pos_y = y;
}

int  GameObject::get_position_x() {
	return pos_x;
}

int GameObject::get_position_y() {
	return pos_y;
}

sf::Sprite GameObject::get_sprite() {
	return sprite;
}

