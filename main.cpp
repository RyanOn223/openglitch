#include "include/game.h"
int main(int argc, char **argv)
{
	//have to pass in settings here to allow anti-alaising
	sf::ContextSettings settings;
	settings.antialiasingLevel = AA_LEVEL;

	game game_1(settings);
	game_1.run();

	//keeps the console open after window is closed
	char pause;
	std::cin >> pause;

	return 0;
}
