# include "HUD_Panel.h"

using namespace std;

HUD_Panel::HUD_Panel() {


}

HUD_Panel::HUD_Panel(sf::Vector2u res) {
	left_display.setSize(sf::Vector2f(res.x / 4, res.y / 3));
	left_display.setPosition(sf::Vector2f(0, 2 * res.y / 3));
	left_display.setFillColor(sf::Color(102, 34, 0));
	left_display.setOutlineThickness(15);
	left_display.setOutlineColor(sf::Color(51, 0, 0));

	mid_display.setSize(sf::Vector2f(res.x / 2 + 1, res.y / 5));
	mid_display.setPosition(sf::Vector2f(res.x / 4, 4 * res.y / 5));
	mid_display.setFillColor(sf::Color(102, 34, 0));
	mid_display.setOutlineThickness(15);
	mid_display.setOutlineColor(sf::Color(51, 0, 0));

	right_display.setSize(sf::Vector2f(res.x / 4, res.y / 4));
	right_display.setPosition(sf::Vector2f(3 * res.x / 4, 3 * res.y / 4));
	right_display.setFillColor(sf::Color(102, 34, 0));
	right_display.setOutlineThickness(15);
	right_display.setOutlineColor(sf::Color(51, 0, 0));
}

void HUD_Panel::drawTo(sf::RenderWindow &window) {
	window.draw(mid_display);
	window.draw(left_display);
	window.draw(right_display);

}

void HUD_Panel::update() {

}