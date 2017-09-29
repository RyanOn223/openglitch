#include "../include/game.h"
game::game(sf::ContextSettings settings) :
                gwindow(sf::VideoMode(800, 600), "openglitch", sf::Style::Default, settings),
                game_world(gwindow)
{
    tick_clock.restart();
    dbg_clock.restart();
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
    //fps_text->setString("60.00");
	//print some console information
	std::cout << "Successfully initialized game\n" << "FPS limited(soft): " <<
		LIMIT_FPS << std::endl << "FPS limit: " << FPS << std::endl;
	std::cout << "Vsync = " << VSYNC << std::endl;
	std::cout << "Anti-aliasing level: " << AA_LEVEL << "x" << std::endl;
	std::cout << "Time spent initializing = " << dbg_clock.restart().asMicroseconds() << " µs\n";
}
void game::run()
{
	sf::Time delta;
	while (gwindow.isOpen())
	{
        //main loop
		delta = tick_clock.restart();
		process_events();
		//once every half second, and only if it's value makes sense, update FPS text
		if (turn_no % static_cast<int>(FPS)/2 == 0 && static_cast<int>(delta.asMilliseconds()) > 6)
            fps_text->setString(std::to_string(1000000.f/delta.asMicroseconds()).substr(0, 5));
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
    command_queue& cmds = game_world.get_cmd_queue();

	sf::Event event;
	while (gwindow.pollEvent(event))
	{
        the_player.handle_event(event, cmds);
        if (event.type == sf::Event::Closed)
            gwindow.close();
	}
	the_player.handle_input(cmds);
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
game::~game()
{
    delete fps_text;
}
