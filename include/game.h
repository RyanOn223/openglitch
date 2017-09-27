#ifndef GAME_H
#define GAME_H
#include "include.h"
#include "world.h"
class game
{
	public:
		//antialaising setting is passed here from main.cpp, other settings
		//should be as well TODO
		game(sf::ContextSettings settings);
        ~game();
		void run();
	private:
		//processes window events
		void process_events();
		//updates entities
		void update(sf::Time delta);
		//draws things
		void render();
		//handles player input
		void handle_input(sf::Keyboard::Key key, bool is_pressed);
	private:
		//the one and only window
		sf::RenderWindow gwindow;
        sf::Font fps_font;
        sf::Text* fps_text;
		world game_world;
		//global frame time
		sf::Time time_per_frame;
		//global turn count
		long turn_no;
};
#endif
