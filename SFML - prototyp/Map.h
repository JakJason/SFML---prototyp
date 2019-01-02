#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.h"
#include <list>

class Map {

private:
	
	int map_width;
	int map_height;
	Chunk **chunks;

	sf::FloatRect whole;

public:

	Map();

	Map(int width, int height, sf::Texture& t);

	void drawTo(sf::RenderWindow& window);

	Chunk** get_chunks();

	Chunk* get_pointer_to_chunk(int i, int j);

	int get_width();

	int get_height();

	Chunk* get_pointer_to_chunk_with_mouse_on_it(sf::RenderWindow &window, sf::Vector2i mouse_possition, sf::View v);

	Chunk* find_path(int x0, int y0, int x1, int y1);
};
