#include "../include/game.h"
game::game(sf::ContextSettings settings):window(sf::VideoMode(800, 600), "openglitch", sf::Style::Default, settings)
{
	//set some window preferences
	window.setVerticalSyncEnabled(VSYNC);
	if (LIMIT_FPS == true) window.setFramerateLimit(60);

	//some debug player settings
	player.setRadius(12.f);
	player.setPosition(100.f, 100.f);
	player.setFillColor(sf::Color::Cyan);

	//debug calls
	scene_node node();
	sf::Vector2f v(0.f, 0.f);
	entity entity(v);

	//set the fps
	time_per_frame = sf::seconds(1.f/FPS);
	//load our one font
	//ps this call wouldn't work without compiling sfml ourselves
	fonts.load(fonts::pixel, "src/pixel.ttf");
	//create and adjust our fps text
	fps_text.setFont(fonts.get(fonts::pixel));
	fps_text.setPosition(0.f, 0.f);
	fps_text.setColor(sf::Color::Yellow);
	//initialize some values
	move_down = false;
	move_left = false;
	move_right = false;
	move_up = false;
	turn_no = 0;
	//print some console information
	std::cout << "Successfully initialized game\n" << "FPS limited(soft): " <<
		LIMIT_FPS << std::endl << "FPS limit: " << FPS << std::endl;
	std::cout << "Vsync = " << VSYNC << std::endl;
	std::cout << "Anti-aliasing level: " << AA_LEVEL << "x" << std::endl;
}
void game::run()
{
	sf::Clock tick_clock;
	//sf::Time last_update = sf::Time::Zero;
	while (window.isOpen())
	{
		sf::Time delta = tick_clock.restart();
		if (turn_no % 5 == 0)
		{
			fps_text.setString(std::to_string(1000000.f/delta.asMicroseconds()).substr(0, 5));
		}
		process_events();
		update(delta);
		render();

		turn_no++;

		/*
		 * this is the other update method the book described but it
		 * didnt work very well
		 *
		process_events();
		last_update += tick_clock.restart();
		while (last_update > time_per_frame)
		{
			printf("%f\n", 1000.f/last_update.asMilliseconds());
			last_update -= time_per_frame;
			process_events();
			update(time_per_frame);
		}
		render();*/
	}
}
void game::process_events()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				handle_input(event.key.code, true);
				break;
			case sf::Event::KeyReleased:
				handle_input(event.key.code, false);
				break;
			case sf::Event::Closed:
				std::cout << "Window Close Event Polled. Goodbye.\n";
				window.close();
				break;
		}
	}
}
void game::update(sf::Time delta)
{
	sf::Vector2f 	movement(0.f, 0.f);
	if (move_up) 	movement.y -= max_linear_speed;
	if (move_down) 	movement.y += max_linear_speed;
	if (move_left) 	movement.x -= max_linear_speed;
	if (move_right) movement.x += max_linear_speed;
	player.move(movement * delta.asSeconds());
}
void game::render()
{
	window.clear();
	window.draw(player);
	window.draw(fps_text);
	window.display();
}
void game::handle_input(sf::Keyboard::Key key, bool is_pressed)
{
	if (key == sf::Keyboard::W) move_up = is_pressed;
	else if (key == sf::Keyboard::A) move_left = is_pressed;
	else if (key == sf::Keyboard::D) move_right = is_pressed;
	else if (key == sf::Keyboard::S) move_down = is_pressed;
}
