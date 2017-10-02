#include "datatables.h"

std::vector<monster_data> init_data()
{
    std::vector<monster_data> data(monster::type::type_count);
    data[monster::type::player].healthpoints = 100;
    data[monster::type::player].speed = PSPEED;
    data[monster::type::player].texture = textures::player;
    return data;
}
