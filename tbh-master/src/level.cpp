#include "level.hpp"
#include <cmath>
#include <iostream>
using namespace std;

level::level(int difficulty, int window_width, int window_height, int start_frame)
{
	this->patterns = std::vector<pattern>();
	this->level_num = difficulty;
	this->window_width = window_width;
	this->window_height = window_height;
	this->start_frame = start_frame;
	this->checkpoints = std::vector<sf::Vector2i>();

	for (int i = 0; i <= level_num; i++)
	{
		checkpoints.push_back(this->get_random_checkpoint_coord());
	}
}

level_state level::check(sf::Vector2i player_pos, sf::Vector2i player_dim)
{
	for (pattern &pat : patterns)
	{
		bool collides = pat.check(player_pos, player_dim);
		if (collides)
			return level_state::LOSE;
	}

	if (checkpoints.size() > 0 &&
		abs(checkpoints.at(0).x - player_pos.x) <= player_dim.x / 2 &&
		abs(checkpoints.at(0).y - player_pos.y) <= player_dim.y / 2)
	{
		checkpoints.erase(checkpoints.begin());
	}

	if (checkpoints.size() == 0)
		return level_state::WIN;

	return level_state::NONE;
}

sf::Vector2i level::get_random_checkpoint_coord()
{
	double theta = (rand() % 360) * (2 * M_PI / 360);

	double r = this->window_width / (2 * (cos(theta) + 1) / 2);
	r = sqrt(r);

	int x = r * cos(theta);
	int y = r * sin(theta);

	x = x + this->window_width / 2;
	y = y + this->window_height / 3;

	return sf::Vector2i{x, y};
}

pattern level::get_random_oscillating_pattern(int difficulty, sf::Clock &clock)
{
	const bool OSC = true;

	// pick a random pattern type and assign the parameters based on the difficulty.
	// Higher difficulty -> smaller step size, higher speed
	// we take the difficulty as a fraction of the max difficulty and use it to generate the pattern's parameters
	int pat_type = rand() % pattern_type::COUNT;
	double difficulty_fraction = (double)difficulty / (double)MAX_DIFFICULTY;
	double step_size = M_PI / 16 / difficulty_fraction;
	double initial_theta = static_cast<double>(rand() / RAND_MAX) * 2 * M_PI; // random initial theta offset
	double speed = 0.001 * difficulty_fraction;
	sf::Vector2i center = sf::Vector2i{window_width / 2, window_height / 3};
	int current_time = clock.getElapsedTime().asMilliseconds();

	return pattern(static_cast<pattern_type>(pat_type), step_size, initial_theta, 0, OSC, speed, center, current_time);
}
/*pattern level::get_random_expanding_pattern(int difficulty, sf::Clock& clock)
{
	const bool OSC = false;

	// pick a random pattern type and assign the parameters based on the difficulty.
	// Higher difficulty -> smaller step size,
}*/
void level::draw(sf::RenderWindow &window, int frame, sf::Clock &clock)
{
	int current_time = clock.getElapsedTime().asMilliseconds();

	frame -= start_frame + 1;

	if (frame == 0)
	{
		// patterns.push_back(get_random_oscillating_pattern(level_num, clock));
	}

	if (level_num == 0 && frame == 0)
	{
		patterns.push_back(pattern(pattern_type::ROSE, M_PI / 20., 0., 100., true, 0.001, sf::Vector2i{window_width / 2, window_height / 3}, current_time));
	}

	if (level_num == 1)
	{
		if (frame % 30 == 0)
		{
			patterns.push_back(pattern(pattern_type::CIRCLE, M_PI / 6., 0., 50., false, 0.001, sf::Vector2i{window_width / 2, window_height / 3}, current_time));
		}
	}

	if (level_num == 2)
	{
		if (frame % 30 == 0)
		{
			patterns.push_back(pattern(pattern_type::DIAMOND, M_PI / 12., 0., 50., true, 0.001, sf::Vector2i{window_width / 2, window_height / 3}, current_time));
		}
	}

	// We need to reference each pattern in the vector using &
	// so that calculations in the pattern can affect the patterns vector in main instead of being copied
	// that way on the next iteration, the pat gets the updated bullet information to check against the player for collision
	for (pattern &pat : patterns)
	{
		pat.calc(current_time, window_width, window_height);
		pat.draw(window);
	}

	if (checkpoints.size() >= 1)
	{
		sf::CircleShape checkpoint_shape(4);
		checkpoint_shape.setFillColor(sf::Color::Yellow);
		checkpoint_shape.setPosition(checkpoints.at(0).x, checkpoints.at(0).y);
		window.draw(checkpoint_shape);
	}
}
