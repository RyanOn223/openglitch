#ifndef DATATABLES_H
#define DATATABLES_H
#include "include.h"
#include "monster.h"
struct monster_data
{
    int healthpoints;
    float speed;
    textures::ID texture;
};
std::vector<monster_data> init_data();
#endif // DATATABLES_H
