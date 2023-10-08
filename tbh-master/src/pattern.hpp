#ifndef _PATTERN_HPP
#define _PATTERN_HPP

#include <vector>
#include "SFML/Graphics.hpp"

enum pattern_type
{
	CIRCLE = 0,
	ROSE,
	DIAMOND,
	COUNT
};

class pattern
{
private:
	std::vector<sf::Vector2i> bullets;
	double initial_theta;
	double initial_a;
	sf::Vector2i center;
	int start_time;
	bool osc;
	double speed;

public:
	// higher patternType = more difficulty (enum?, arranged by difficulty)
	pattern_type type;

	// angle step size while drawing (lower = harder)
	double step_size;

	pattern(pattern_type type, double step_size, double initial_theta, double initial_a, bool osc, double speed, sf::Vector2i center, int start_time);

	/** Checks if bullets and player collide.
	@param player_pos The position of the player.
	@param player_dim The dimensions of the player.
	*/
	bool check(sf::Vector2i player_pos, sf::Vector2i player_dim);

	/** Calculates the position of every bullet for a specific pattern and draws
	them on the screen.
	@param time The current time, starts very close to 0
	@param window_w The width of the window
	@param window_h The height of the window
	*/
	void calc(int time, double window_w, double window_h);

	/** Draws the bullets on the screen, for use after running calc().
	@param window The window to draw the bullets on.
	*/
	void draw(sf::RenderWindow &window);
};

#endif