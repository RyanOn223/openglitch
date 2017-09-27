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

//I'm not sure if I like this namespace::enum thing the book is using
//but we're going to do it for practice

namespace textures
{
	enum ID { player, small_mutant, large_mutant, floor, wall };
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

//define some critical game values. later, these should be loaded from a file in game() and main() respectively
const float max_linear_speed = 300.f;
const float FPS = 60.f;
const bool VSYNC = true;
const bool LIMIT_FPS = false;
const short int AA_LEVEL = 8;
#endif
