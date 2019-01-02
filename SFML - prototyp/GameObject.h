#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.h"
#include "Map.h"

class GameObject {

private:
	int pos_x;
	int pos_y;

	sf::Sprite sprite;

public:

	void set_sprite(sf::Sprite s);

	void set_sprite(sf::Texture& t, double x_s, double y_s);

	void set_sprite_position(double x_s, double y_s);

	void set_position_x(int x);

	void set_position_y(int y);

	int get_position_x();

	int get_position_y();

	sf::Sprite get_sprite();

	virtual void drawTo(sf::RenderWindow &window) = 0;

	virtual void place_on_chunk(sf::RenderWindow &window, sf::Sprite active, Chunk c, bool b) = 0;

	virtual void check_status(Map map, sf::RenderWindow &windowv, bool b) = 0;

	virtual void destroy() = 0;

};