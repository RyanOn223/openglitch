#ifndef INCLUDE_H
#define INCLUDE_H
//#define _GLIBCXX_USE_CXX11_ABI 1
#include "/usr/local/./include/SFML/Window.hpp"
#include "/usr/local/./include/SFML/Graphics.hpp"
#include "/usr/local/./include/SFML/System.hpp"
#include "/usr/local/./include/SFML/Audio.hpp"
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <queue>
#include <math.h>
//I'm not sure if I like this namespace::enum thing the book is using
//but we're going to do it for practice

namespace textures
{
	enum ID { player, small_mutant, large_mutant, floor, wall, cursor };
}
namespace sounds
{
	enum ID { fire, walk, interact };
}
namespace fonts
{
	enum ID { pixel };
}
namespace images
{
	enum ID {};
}
namespace cmd_category
{
    enum ID
    {
        none =          0,
        scene =         1 << 0,
        the_player =    1 << 1,
        enemies =       1 << 2
    };
}
//define some critical game values. later, these should be loaded from a file in game() and main() respectively
//const float max_linear_speed = 300.f;
const float FPS = 60.f;
const bool VSYNC = true;
const bool LIMIT_FPS = false;
const short int AA_LEVEL = 2;
const float decel_speed = 35.f/60.f;
const float PSPEED = 30.f;
const float SENSITIVITY  = 2; //higher numbers are less sensitive
const float UPPER_ZOOM_LIMIT = 0.5f;
const float LOWER_ZOOM_LIMIT = 0.1f;
#endif
