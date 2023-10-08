#ifndef _LEVEL_HPP
#define _LEVEL_HPP

#include <vector>
#include "SFML/Graphics.hpp"
#include "pattern.hpp"

enum level_state
{
	NONE,
	WIN,
	LOSE
};

const int MAX_DIFFICULTY = 5;

class level
{
private:
	std::vector<pattern> patterns;
	std::vector<sf::Vector2i> checkpoints;
	int window_width;
	int window_height;
	int start_frame;
	pattern get_random_oscillating_pattern(int difficulty, sf::Clock &clock);
	pattern get_random_expanding_pattern(int difficulty, sf::Clock &clock);
	sf::Vector2i get_random_checkpoint_coord();

public:
	int level_num;

	level(int difficulty, int window_width, int window_height, int start_frame);
	level_state check(sf::Vector2i player_pos, sf::Vector2i player_dim);
	void draw(sf::RenderWindow &window, int frame, sf::Clock& clock);
};

#endif