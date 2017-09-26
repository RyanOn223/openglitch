#ifndef GAME_H
#define GAME_H
#include "include.h"
#include "texture_manager.h"
#include "resource_manager.h"
#include "scene_node.h"
#include "entity.h"
class game
{
	public:
		//antialaising setting is passed here from main.cpp, other settings
		//should be as well TODO
		game(sf::ContextSettings settings);

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
		sf::RenderWindow window;
		//debug player
		sf::CircleShape player;
		//bool values used for input
		bool move_up, move_down, move_left, move_right;
		//our global resource managers
		texture_manager textures;
		//resource THEN identifier
		resource_manager<sf::Sound, sounds::ID> sounds;
		resource_manager<sf::Font, fonts::ID> fonts;
		resource_manager<sf::Image, images::ID> images;

		//fps text
		sf::Text fps_text;
		//global frame time
		sf::Time time_per_frame;
		//global turn count
		long turn_no;
};
#endif
