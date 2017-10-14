#ifndef INCLUDE_H
#define INCLUDE_H
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
#include <fstream>
#include <sstream>
#include <algorithm>

namespace textures
{
	enum ID { entities,
              walls,
              explosions,
              particles,
              floors,
              };
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
        air_layer =          1 << 5,
        pickups =            1 << 6,
        walls =              1 << 7,
        mouse =              1 << 8,
    };
}
enum bullet_type
{
    pistol_bullet,
    shotgun_bullet,
    bullet_type_count,
};
enum weapon_type
{
    none,
    small_pistol,
    basic_shotgun,
    weapon_type_count,
};
//define some critical game values. later, these should be loaded from a file
const float PI = 3.14159265359f;
const float FPS = 60.f;
const bool VSYNC = true;
const bool LIMIT_FPS = true;
const short int AA_LEVEL = 4;
const float PSPEED = 45.f;
const float SENSITIVITY  = 2; //higher numbers are less sensitive
const float UPPER_ZOOM_LIMIT = 0.3f;
const float LOWER_ZOOM_LIMIT = 0.1f;
//something to do with how the cursor resizes with zooming
const float CURS_SCALE = 2.4f;
//debug text is updated DEBUG_DRAW_UPS times per second
const int DEBUG_DRAW_UPS = 6;
//0 for couple update-draw logic
//1 for decoupled update-draw logic, which should be better but isn't at anything other than 60fps
const int UPDATE_METHOD = 0;
#endif
