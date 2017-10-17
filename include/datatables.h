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
    sf::IntRect texture_rect;
    animation::type walk_animation;
};
struct bullet_data
{
    int damage;
    float speed;
    bool has_emitter;
    particle::type ptype;
};
struct pickup_data
{
    textures::ID texture;
    sf::IntRect texture_rect;
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
    sf::IntRect texture_rect;
    //for shotguns etc
    int num_bullets;
};
struct particle_data
{
    sf::Color color;
    sf::Time lifetime;
};
struct animation_data
{
    textures::ID texture;
    sf::IntRect texture_rect;
    sf::Vector2i frame_size;
    std::size_t num_frames;
    sf::Time duration;
    bool repeat;
};
std::vector<monster_data> init_monster_data();
std::vector<bullet_data> init_bullet_data();
std::vector<pickup_data> init_pickup_data();
std::vector<weapon_data> init_weapon_data();
std::vector<particle_data> init_particle_data();
std::vector<animation_data> init_animation_data();
#endif // DATATABLES_H
