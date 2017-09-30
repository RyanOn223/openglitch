#include "../include/game.h"
game::game(sf::ContextSettings settings) :
                gwindow(sf::VideoMode(1024, 768), "openglitch", sf::Style::Default  , settings),
                game_world(gwindow),
                the_player()
{
    ispaused = false;
    tick_clock.restart();
    dbg_clock.restart();
    //global_clock.restart();
	//set some window preferences
	gwindow.setVerticalSyncEnabled(VSYNC);
	if (LIMIT_FPS == true) gwindow.setFramerateLimit(static_cast<unsigned int>(FPS));

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
   //sf::Time t = sf::Time::Zero;
	sf::Time delta;
	//sf::Time current_time = global_clock.getElapsedTime();
	//float accumulator = 0.f;
	while (gwindow.isOpen())
	{

        //main loop
		delta = tick_clock.restart();

		//once every half second, and only if it's value makes sense, update FPS text
		if (turn_no % static_cast<int>(FPS)/2 == 0)
            fps_text->setString(std::to_string(1000000.f/delta.asMicroseconds()).substr(0, 5));
		sf::Vector2f pos;
		if (!ispaused) update(delta);
		render();
        process_events();
		turn_no++;

		/*
		 * this is the other update method the book described but it
		 * didnt work very well. it may be worth revisiting though.
		 *
        sf::Time new_time = global_clock.getElapsedTime();
		sf::Time frame_time = new_time - current_time;
		current_time = new_time;
		accumulator += frame_time.asMilliseconds();
		while (accumulator >= 1.f/FPS)
		{
			update(sf::microseconds(1666));
			process_events();
			accumulator -= 1.f/FPS;

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
        if (event.type == sf::Event::GainedFocus)
            ispaused = false;
        if (event.type == sf::Event::LostFocus)
            ispaused = true;
        if (event.type == sf::Event::MouseWheelScrolled)
        {
            //scroll in or out
            if (event.mouseWheelScroll.delta > 0.f) game_world.set_zoom(.9f);
            else game_world.set_zoom(1.1f);
        }
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
