#include "pattern.hpp"
#include <cmath>
#include <iostream>

pattern::pattern(pattern_type type, double step_size, double initial_theta, double initial_a, bool osc, double speed, sf::Vector2i center, int start_time)
{
	this->type = type;
	this->bullets = std::vector<sf::Vector2i>();
	this->step_size = step_size;
	this->initial_theta = initial_theta;
	this->initial_a = initial_a;
	this->center = center;
	this->start_time = start_time;
	this->osc = osc;
	this->speed = speed;
}

bool pattern::check(sf::Vector2i player_pos, sf::Vector2i player_dim)
{

	for (sf::Vector2i &bullet : bullets)
	{

		if (abs(bullet.x - player_pos.x) <= player_dim.x / 2 && abs(bullet.y - player_pos.y) <= player_dim.y / 2)
			return true;
	}
	return false;
}

void pattern::calc(int time, double window_w, double window_h)
{
	// We calculate the location of the bullets based on the current time,
	// which should start at 0 when the pattern is initialized.
	// Therefore we subtract the start time from the current time.
	time -= start_time;
	this->bullets.clear();

	if (this->type == pattern_type::ROSE)
	{
		void spawn_pattern(int level);
		// equation: r = a * cos(3theta);
		// a is a function of time
		double a;
		if (osc)
		{
			a = initial_a + initial_a * sin(time * speed);
		}
		else
		{
			a = initial_a + time * speed;
		}

		// theta is a function of time, so we use a theta offset
		double theta_offset = time * speed;

		for (double theta = 0; theta < 2 * M_PI; theta += step_size)
		{
			double r = a * cos(3 * (theta + theta_offset));

			double x = this->center.x + r * cos(theta + initial_theta);
			double y = this->center.y + r * sin(theta + initial_theta);

			this->bullets.push_back(sf::Vector2i(x, y));
		}
	}
	else if (this->type == pattern_type::CIRCLE)
	{
		// equation: r = a
		// we still use a theta offset to rotate the pattern over time.
		double theta_offset = time * speed;

		// a is a function of time (and window size)
		// double a = std::min(window_w, window_h) / 3 * sin(theta_offset);
		double a = initial_a + time * 0.1;
		for (double theta = 0; theta < 2 * M_PI; theta += step_size)
		{
			double r = a;

			double x = this->center.x + r * cos(theta + theta_offset + initial_theta);
			double y = this->center.y + r * sin(theta + theta_offset + initial_theta);

			this->bullets.push_back(sf::Vector2i(x, y));
		}
	}
	else if (this->type == pattern_type::DIAMOND)
	{
		// this is a piecewise function with 4 parts:
		// r_1 = a/[sin(theta)+cos(theta)]
		// r_2 = a/[sin(theta)-cos(theta)]
		// r_3 = -a/[sin(theta)+cos(theta)]
		// r_4 = -a/[sin(theta)-cos(theta)]

		// we don't use a theta offset to rotate the pattern over time. because it looks bad.
		double theta_offset = 0;

		double a = initial_a + time * 0.1;
		for (double theta = 0; theta < 2 * M_PI; theta += step_size)
		{
			double r;

			if (theta < M_PI / 2)
				r = a / (sin(theta + theta_offset) + cos(theta + theta_offset));
			else if (theta < M_PI)
				r = a / (sin(theta + theta_offset) - cos(theta + theta_offset));
			else if (theta < 3 * M_PI / 2)
				r = -a / (sin(theta + theta_offset) + cos(theta + theta_offset));
			else
				r = -a / (sin(theta + theta_offset) - cos(theta + theta_offset));

			// todo: extract into function (code reusal)
			double x = this->center.x + r * cos(theta + theta_offset + initial_theta);
			double y = this->center.y + r * sin(theta + theta_offset + initial_theta);

			this->bullets.push_back(sf::Vector2i(x, y));
		}
	}
}

void pattern::draw(sf::RenderWindow &window)
{
	for (sf::Vector2i &bullet : this->bullets)
	{
		sf::CircleShape bullet_shape(3);
		bullet_shape.setPosition(bullet.x, bullet.y);
		window.draw(bullet_shape);
	}
}