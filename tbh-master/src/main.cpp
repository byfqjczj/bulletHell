#include <iostream>
#include <cmath>
#include "SFML/Graphics.hpp"
#include "pattern.hpp"
#include "level.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	constexpr int WINDOW_WIDTH = 600;
	constexpr int WINDOW_HEIGHT = 800;
	constexpr int FPS = 60;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "tbh");
	// window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(FPS);

	const int PLAYER_SIZE = 10;
	sf::Vector2f player_pos = sf::Vector2f{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3 * 2};
	const float MOVEMENT_STEP = 5;

	sf::Clock clock;

	double current_time = clock.getElapsedTime().asMilliseconds();

	// A monotonic frame counter
	uint32_t frames = 0;

	level current_level = level(0, WINDOW_WIDTH, WINDOW_HEIGHT, frames);

	while (window.isOpen())
	{
		frames++;

		double current_time = clock.getElapsedTime().asMilliseconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
				break;
			case sf::Event::MouseMoved:
				// player_pos = sf::Vector2f(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			player_pos.y -= MOVEMENT_STEP;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			player_pos.y += MOVEMENT_STEP;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			player_pos.x -= MOVEMENT_STEP;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			player_pos.x += MOVEMENT_STEP;

		if (player_pos.x < 0)
			player_pos.x = 0;
		if (player_pos.x > WINDOW_WIDTH - PLAYER_SIZE)
			player_pos.x = WINDOW_WIDTH - PLAYER_SIZE;

		if (player_pos.y < 0)
			player_pos.y = 0;
		if (player_pos.y > WINDOW_HEIGHT - PLAYER_SIZE)
			player_pos.y = WINDOW_HEIGHT - PLAYER_SIZE;

		// Clear screen
		window.clear();

		// Draw the player
		sf::RectangleShape player(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
		player.setPosition(player_pos.x, player_pos.y);
		window.draw(player);

		switch (current_level.check(static_cast<sf::Vector2i>(player.getPosition()), static_cast<sf::Vector2i>(player.getSize())))
		{
		case level_state::LOSE:
			window.close();
			break;
		case level_state::WIN:
			player_pos = sf::Vector2f{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 3 * 2};
			current_level = level(current_level.level_num + 1, WINDOW_WIDTH, WINDOW_HEIGHT, frames);
			break;
		}

		current_level.draw(window, frames, clock);

		// Update the window on the screen
		window.display();
	}

	return EXIT_SUCCESS;
}
