#include "../include/game.h"

namespace
{
    const std::vector<weapon_data> weapons = init_weapon_data();
    const std::vector<bullet_data> bullets = init_bullet_data();
}

game::game(sf::ContextSettings settings, sf::Clock& init_clock) :
                gwindow(sf::VideoMode(1024 , 768), "openglitch", sf::Style::Default, settings),
                game_world(gwindow),
                the_player(game_world.get_player())
{
    ispaused = false;
    tick_clock.restart();
	//set some window preferences
	gwindow.setVerticalSyncEnabled(VSYNC);
	if (LIMIT_FPS == true) gwindow.setFramerateLimit(static_cast<unsigned int>(FPS));

	time_per_frame = sf::seconds(1.f/FPS);
	turn_no = 0;

	//initialize resources
    sf::Text* t(new sf::Text);
    fps_text = t;
	fps_font.loadFromFile("src/pixel.ttf");
    fps_text->setFont(fps_font);
    fps_text->setCharacterSize(25);
    fps_text->setColor(sf::Color::Yellow);
    fps_text->setPosition(0.f, 0.f);

    sf::Text* t2(new sf::Text);
    hp_text = t2;
    hp_text->setFont(fps_font);
    hp_text->setCharacterSize(45);
    hp_text->setColor(sf::Color::White);
    hp_text->setOrigin(hp_text->getLocalBounds().width / 2.f, hp_text->getLocalBounds().height);
    hp_text->setPosition(gwindow.getDefaultView().getSize().x - 75, -15);

    sf::Text* t3(new sf::Text);
    ammo_text = t3;
    ammo_text->setFont(fps_font);
    ammo_text->setCharacterSize(45);
    ammo_text->setColor(sf::Color::Green);
    ammo_text->setOrigin(hp_text->getLocalBounds().width / 2.f, hp_text->getLocalBounds().height);
    ammo_text->setPosition(gwindow.getDefaultView().getSize().x - 150, -15);

	std::cout << "Time spent initializing = " << init_clock.restart().asMilliseconds() << " ms\n";
	std::cout << "Successfully initialized game\n" << "FPS limited(soft): " <<
		LIMIT_FPS << std::endl << "FPS limit: " << FPS << std::endl;
	std::cout << "Vsync = " << VSYNC << std::endl;
	if (UPDATE_METHOD == 0) std::cout << "Update method: non-deterministic\n";
	else std::cout << "Update method: deterministic\n";
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
        if (UPDATE_METHOD == 1)
        {
            sf::Time fps_time = fps_clock.restart();
            if (turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
                fps_text->setString("fps:\t" + std::to_string(1000000.f/fps_time.asMicroseconds()).substr(0, 5));

            dbg_clock.restart();
            process_events();
            accumulator += tick_clock.restart();

            sf::Time ev_time = dbg_clock.restart();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
            {
                fps_text->setString(fps_text->getString() + "\nevent:\t" + (std::to_string(ev_time.asMicroseconds()/1000.f).substr(0,5)));
            }

            sf::Vector2f pos;
            sf::Clock update_clock;
            sf::Time up_time = sf::Time::Zero;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
            {
                fps_text->setString(fps_text->getString() + "\naccum:\t" + (std::to_string(accumulator.asMicroseconds()/1000.f).substr(0, 5)));
            }
            if (!ispaused)
            {
                bool first_update = true;
                while (accumulator > delta)
                {
                    accumulator -= delta;
                    process_events();
                    update(delta);

                    up_time = update_clock.restart();
                    sf::Time ups_time = ups_clock.restart();
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0 && first_update)
                    {
                        fps_text->setString(fps_text->getString() + "\nupdate:\t" + (std::to_string((up_time.asMicroseconds()/1000.f)).substr(0, 5)));
                        fps_text->setString(fps_text->getString() + "\nups:\t" + (std::to_string(1000000.f/ups_time.asMicroseconds() * static_cast<int>(1 + accumulator / delta)).substr(0, 5)));
                    }
                    first_update = false;
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





            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
            {
                fps_text->setString(fps_text->getString() + "\n(" + std::to_string(game_world.get_cursor()->getPosition().x).substr(0,6)+
                   ", " + std::to_string(game_world.get_cursor()->getPosition().y).substr(0,6) + ")\n");
            }
            turn_no++;
        }
        else if (UPDATE_METHOD == 0)
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
            if (!ispaused)
            {
                update(tick_clock.restart());
                up_time = update_clock.restart();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
                {
                    fps_text->setString(fps_text->getString() + "\nupdate:\t" + (std::to_string(up_time.asMicroseconds()/1000.f).substr(0, 5)));
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
                fps_text->setString(fps_text->getString() +
                                    "\ntotal:\t" +
                                    (std::to_string((rnd_time + ev_time + up_time).asMicroseconds()/1000.f).substr(0, 5)));
            }




            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && turn_no % (static_cast<int>(FPS)/DEBUG_DRAW_UPS) == 0)
            {
                fps_text->setString(fps_text->getString() + "\n(" + std::to_string(game_world.get_cursor()->getPosition().x).substr(0,6)+
                   ", " + std::to_string(game_world.get_cursor()->getPosition().y).substr(0,6) + ")\n");
            }
            turn_no++;
        }
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
        else if (event.type == sf::Event::GainedFocus)
            ispaused = false;
        else if (event.type == sf::Event::LostFocus)
            ispaused = true;
        else if (event.type == sf::Event::MouseWheelScrolled)
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
	draw_health();
	if (game_world.get_player()->weapon != weapon_type::none) draw_ammo();
	gwindow.display();
}
void game::draw_health()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) gwindow.draw(*fps_text);
    sf::Sprite phealth(game_world.textures.get(textures::entities), sf::IntRect(0,10,9,8));
    phealth.setOrigin(phealth.getLocalBounds().width / 2.f, phealth.getLocalBounds().height / 2.f);
    phealth.setPosition(gwindow.getDefaultView().getSize().x - 100, 20);
    phealth.setScale(3.f,3.f);
    gwindow.draw(phealth);
    hp_text->setString(std::to_string(game_world.get_player_hp()));

    gwindow.draw(*hp_text);
}
void game::draw_ammo()
{
    sf::Sprite pweapon(game_world.textures.get(weapons[game_world.get_player()->weapon].texture), weapons[game_world.get_player()->weapon].texture_rect);
    pweapon.setOrigin(pweapon.getLocalBounds().width / 2.f, pweapon.getLocalBounds().height / 2.f);
    pweapon.setPosition(gwindow.getDefaultView().getSize().x - 200, 20);
    pweapon.setScale(6.f, 6.f);
    gwindow.draw(pweapon);
    sf::Sprite opweapon(game_world.textures.get(weapons[game_world.get_player()->other_weapon].texture), weapons[game_world.get_player()->other_weapon].texture_rect);
    opweapon.setOrigin(pweapon.getLocalBounds().width / 2.f, pweapon.getLocalBounds().height / 2.f);
    opweapon.setPosition(gwindow.getDefaultView().getSize().x - 275, 20);
    opweapon.setScale(6.f, 6.f);
    //if ((weapons[game_world.get_player()->other_weapon]) != weapon_data())
    {
        gwindow.draw(opweapon);
    }
    ammo_text->setString(std::to_string(game_world.get_player()->ammo_held[weapons[game_world.get_player()->weapon].ammo_type]));
    gwindow.draw(*ammo_text);
}
game::~game()
{
    delete fps_text;
    delete hp_text;
    delete ammo_text;
}
