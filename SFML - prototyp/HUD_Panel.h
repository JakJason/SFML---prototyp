#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class HUD_Panel
{
private:
	sf::RectangleShape left_display;
	sf::RectangleShape mid_display;
	sf::RectangleShape right_display;

	GameObject** active_list = new GameObject*[20];

public:
	HUD_Panel();

	HUD_Panel(sf::Vector2u res);

	void drawTo(sf::RenderWindow &window);

	void update();

};