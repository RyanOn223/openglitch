#ifndef GAME_H
#define GAME_H
#include "include.h"
#include "world.h"
class game
{
	public:

		game(sf::ContextSettings settings, sf::Clock& init_clock);
        ~game();
		void run();
		bool ispaused;
	private:
		//processes window events
		void process_events();
		//updates entities
		void update(sf::Time delta);
		//draws things
		void render();
		void draw_health();
		void draw_ammo();
	private:
		//the one and only window
		sf::RenderWindow gwindow;
        sf::Font fps_font;
        sf::Text* fps_text;
        sf::Text* hp_text;
        sf::Text* ammo_text;
		world game_world;
		sf::Time time_per_frame;
		sf::Clock tick_clock;
		sf::Clock dbg_clock;
		sf::Clock fps_clock;
		sf::Clock ups_clock;
		long turn_no;
		player the_player;
};
#endif
