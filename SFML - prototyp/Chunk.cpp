#include <iostream>
#include "Chunk.h"
#define PI 3.14159265

using namespace std;

Chunk::Chunk()
{

}

Chunk::Chunk( int x, int y) {

	set_position_x(x);
	set_position_y(y);
}

void Chunk::set_position_x(sf::Uint8 x) {
	pos_x = x;
}

void Chunk::set_position_y(sf::Uint8 y) {
	pos_y = y;
}

int  Chunk::get_position_x() {
	return pos_x;
}

int Chunk::get_position_y() {
	return pos_y;
}

void Chunk::set_sprite(sf::Texture& t, int i, int j) {
	sprite.setTexture(t);
	sprite.setRotation(45);
	sprite.setScale(1, 1);

	float x = i * t.getSize().x * sprite.getScale().x;
	float y = j * t.getSize().y * sprite.getScale().y;

	sprite.setPosition(x * cos(PI / 4) - y * sin(PI / 4), x * sin(PI / 4) + y * cos(PI / 4));
}

sf::Sprite Chunk::get_sprite() {
	return sprite;
}

void Chunk::drawTo(sf::RenderWindow &window) {
	window.draw(sprite);
}
