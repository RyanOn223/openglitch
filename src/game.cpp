#include "../include/game.h"
game::game(sf::ContextSettings settings) :
                gwindow(sf::VideoMode(800, 600), "openglitch", sf::Style::Default, settings),
                game_world(gwindow)
{
	//set some window preferences
	gwindow.setVerticalSyncEnabled(VSYNC);
	if (LIMIT_FPS == true) gwindow.setFramerateLimit(FPS);

	//set some values
	time_per_frame = sf::seconds(1.f/FPS);
	turn_no = 0;
    sf::Text* t(new sf::Text);
    fps_text = t;
	fps_font.loadFromFile("src/pixel.ttf");
    fps_text->setFont(fps_font);
    fps_text->setCharacterSize(25);
    fps_text->setColor(sf::Color::Yellow);
    fps_text->setPosition(0.f, 0.f);
    fps_text->setString("60.00");
	//print some console information
	std::cout << "Successfully initialized game\n" << "FPS limited(soft): " <<
		LIMIT_FPS << std::endl << "FPS limit: " << FPS << std::endl;
	std::cout << "Vsync = " << VSYNC << std::endl;
	std::cout << "Anti-aliasing level: " << AA_LEVEL << "x" << std::endl;
}
void game::run()
{
	sf::Clock tick_clock;
	sf::Time delta;
	while (gwindow.isOpen())
	{
        //main loop
		delta = tick_clock.restart();
		process_events();
		if (turn_no % 30 == 0 ) fps_text->setString(std::to_string(1000000.f/delta.asMicroseconds()).substr(0, 5));
		sf::Vector2f pos;
		update(delta);
		render();

		turn_no++;

		/*
		 * this is the other update method the book described but it
		 * didnt work very well. it may be worth revisiting though.
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
	while (gwindow.pollEvent(event))
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
				gwindow.close();
				break;
		}
	}
}
void game::update(sf::Time delta)
{
    game_world.update(delta);
}
void game::render()
{
	gwindow.clear();
	game_world.draw();
	gwindow.setView(gwindow.getDefaultView());
	gwindow.draw(*fps_text);
	gwindow.display();
}
void game::handle_input(sf::Keyboard::Key key, bool is_pressed)
{

}
game::~game()
{
    delete fps_text;
}
