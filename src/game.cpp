#include "../include/game.h"

game::game(sf::ContextSettings settings) :
                gwindow(sf::VideoMode(1366, 768), "openglitch", sf::Style::None, settings),
                game_world(gwindow),
                the_player()
{
    ispaused = false;
    tick_clock.restart();
    dbg_clock.restart();
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
	//print some console information
	std::cout << "Time spent initializing = " << dbg_clock.restart().asMicroseconds() << " µs\n";
	std::cout << "Successfully initialized game\n" << "FPS limited(soft): " <<
		LIMIT_FPS << std::endl << "FPS limit: " << FPS << std::endl;
	std::cout << "Vsync = " << VSYNC << std::endl;
	std::cout << "Anti-aliasing level: " << AA_LEVEL << "x\n\nRunning game...\n\n";
}
void game::run()
{
	sf::Time delta = sf::seconds(1.f/FPS);
	sf::Time accumulator = sf::Time::Zero;
	tick_clock.restart();
	fps_clock.restart();
	ups_clock.restart();
	while (gwindow.isOpen())
	{
        sf::Time fps_time = fps_clock.restart();
        if (turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
            fps_text->setString("fps:\t" + std::to_string(1000000.f/fps_time.asMicroseconds()).substr(0, 5));

        dbg_clock.restart();
        process_events();
        sf::Time ev_time = dbg_clock.restart();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
        {
            fps_text->setString(fps_text->getString() + "\nevent:\t" + (std::to_string(ev_time.asMicroseconds()/1000.f).substr(0,5)));
        }

		sf::Vector2f pos;
        sf::Clock update_clock;
        sf::Time up_time = sf::Time::Zero;
        //dont ever take two steps in a row. this leads to visual lag that our render method cant fix
        //this may have unknown reprecussions down the line
        if (accumulator > sf::milliseconds(32)) accumulator = sf::milliseconds(32);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
        {
            fps_text->setString(fps_text->getString() + "\naccum:\t" + (std::to_string(accumulator.asMicroseconds()/1000.f).substr(0, 5)));
        }
		if (!ispaused)
		{
            while (accumulator > delta)
            {
                accumulator -= delta;
                update(delta);
                up_time = update_clock.restart();
                sf::Time ups_time = ups_clock.restart();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
                {
                    fps_text->setString(fps_text->getString() + "\nupdate:\t" + (std::to_string(up_time.asMicroseconds()/1000.f).substr(0, 5)));
                    fps_text->setString(fps_text->getString() + "\nups:\t" + (std::to_string(1000000.f/ups_time.asMicroseconds()).substr(0, 5)));
                }
            }
        }
        sf::Clock render_clock;
        render_clock.restart();
		render();
        sf::Time rnd_time = render_clock.restart();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
        {
            fps_text->setString(fps_text->getString() +
                                "\nrender:\t" +
                                (std::to_string(rnd_time.asMicroseconds()/1000.f).substr(0, 5)));
        }


        accumulator += tick_clock.restart();


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
        {
            fps_text->setString(fps_text->getString() + "\n(" + std::to_string(game_world.get_cursor()->getPosition().x).substr(0,6)+
               ", " + std::to_string(game_world.get_cursor()->getPosition().y).substr(0,6) + ")\n");
        }
		turn_no++;
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
            if (event.mouseWheelScroll.delta > 0.f)
                game_world.set_zoom(.9f);
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) gwindow.draw(*fps_text);
	gwindow.display();
}
game::~game()
{
    delete fps_text;
}
