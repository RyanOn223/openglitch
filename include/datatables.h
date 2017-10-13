#ifndef DATATABLES_H
#define DATATABLES_H
#include "include.h"
#include "monster.h"
#include "projectile.h"
#include "pickup.h"
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
struct pickup_data
{
    textures::ID texture;
    std::function<void(scene_node&)> action;
};
struct weapon_data
{
    bullet_type ammo_type;
    //higher numbers are more accurate
    float spread;
    //in milliseconds
    float reload_speed;
    bool automatic;
    textures::ID texture;
};
std::vector<monster_data> init_monster_data();
std::vector<bullet_data> init_bullet_data();
std::vector<pickup_data> init_pickup_data();
std::vector<weapon_data> init_weapon_data();
#endif // DATATABLES_H
