#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "MoveOrder.h"
#include "Map.h"

class Ball : public GameObject{

private:
	int pos_x;
	int pos_y;

	MoveOrder* order = nullptr;

	sf::Sprite sprite;
public:

	Ball();

	Ball(sf::Texture t, int pos_x, int pos_y, double x_s, double y_s);

	int get_position_x();

	int get_position_y();

	sf::Sprite get_sprite();

	MoveOrder* get_current_order();

	void set_current_order(MoveOrder *o);

	void set_sprite(sf::Texture& t, int i, int j);

	void drawTo(sf::RenderWindow &window);

	void drawToActive(sf::RenderWindow &window, sf::Sprite &active);

	void check_status(Map map, sf::RenderWindow &window, bool b);

	void destroy();

	void place_on_chunk(sf::RenderWindow &window, sf::Sprite active, Chunk c, bool b);

	void cancel(MoveOrder& o);

	void give(MoveOrder& o, Map map);

	void apply(MoveOrder& o, Map map, sf::RenderWindow &window, bool b);

	void remove(MoveOrder& o);

};
