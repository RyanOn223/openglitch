#include "include/game.h"
int main(int argc, char **argv)
{
	//have to pass in settings here to allow anti-alaising
	sf::ContextSettings settings;
	settings.antialiasingLevel = AA_LEVEL;
    sf::Clock main_clock;
    main_clock.restart();
	try
	{
        game game_1(settings, main_clock);
        game_1.run();
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
	return 0;
}
