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
	enum ID { player, small_mutant, large_mutant, floor, wall, cursor, bullet, sm_ammo, sm_health_pack };
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
        none =               0,
        scene =              1 << 0,
        the_player =         1 << 1,
        enemies =            1 << 2,
        ally_projectiles =   1 << 3,
        enemy_projectiles =  1 << 4,
        //the air scene layer has to take commands itself
        air_layer =          1 << 5,
        pickups =            1 << 6,

    };
}
//define some critical game values. later, these should be loaded from a file
const float PI = 3.141592654f;
const float FPS = 90.f;
const bool VSYNC = true;
const bool LIMIT_FPS = true;
const short int AA_LEVEL = 4;
const float PSPEED = 30.f;
const float SENSITIVITY  = 2; //higher numbers are less sensitive
const float UPPER_ZOOM_LIMIT = 0.3f;
const float LOWER_ZOOM_LIMIT = 0.1f;

//sf::Vector2f rtheta_to_xy(float R, float theta)
//{
//    theta *= PI/180.f;
//    return sf::Vector2f(R * cos(theta), R * sin(theta));
//}
#endif
