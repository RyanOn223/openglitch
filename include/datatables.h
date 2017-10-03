#ifndef DATATABLES_H
#define DATATABLES_H
#include "include.h"
#include "monster.h"
#include "projectile.h"
struct monster_data
{
    int healthpoints;
    float speed;
    textures::ID texture;
};
struct bullet_data
{
    int damage;
    float speed;
};
std::vector<monster_data> init_monster_data();
std::vector<bullet_data> init_bullet_data();
#endif // DATATABLES_H
