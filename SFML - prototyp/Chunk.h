#pragma once
#include <SFML/Graphics.hpp>

class Chunk
{
private:
	int pos_x;
	int pos_y;

	sf::Sprite sprite;

public:
	Chunk();
	
	Chunk(int x, int y);

	void set_position_x(sf::Uint8 x);

	void set_position_y(sf::Uint8 y);
	
	int get_position_x();

	int get_position_y();

	void set_sprite(sf::Texture& t, int i, int j);

	sf::Sprite get_sprite();

	void drawTo(sf::RenderWindow &window);
};