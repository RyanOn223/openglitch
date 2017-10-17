#include "datatables.h"
std::vector<monster_data> init_monster_data()
{
    std::vector<monster_data> data(monster::type::type_count);
    data[monster::type::player].healthpoints = 100;
    data[monster::type::player].speed = PSPEED;
    data[monster::type::player].texture = textures::entities;
    data[monster::type::player].texture_rect = sf::IntRect(32, 0, 4, 4);
    data[monster::type::player].walk_animation = animation::type::player_walk;
    data[monster::type::small_mutant].healthpoints = 65;
    data[monster::type::small_mutant].speed = 30;
    data[monster::type::small_mutant].texture = textures::entities;
    data[monster::type::small_mutant].texture_rect = sf::IntRect(26,0,5,7);
    data[monster::type::small_mutant].walk_animation = animation::type::none;
    return data;
}
std::vector<bullet_data> init_bullet_data()
{
    std::vector<bullet_data> data(bullet_type::bullet_type_count);
    data[bullet_type::pistol_bullet].damage = 10;
    data[bullet_type::pistol_bullet].speed = 500.f;
    data[bullet_type::pistol_bullet].has_emitter = false;
    data[bullet_type::pistol_bullet].ptype = particle::type::none;
    data[bullet_type::shotgun_bullet].damage = 3;
    data[bullet_type::shotgun_bullet].speed = 350.f;
    data[bullet_type::shotgun_bullet].has_emitter = false;
    data[bullet_type::shotgun_bullet].ptype = particle::type::none;
    data[bullet_type::rocket_shell].damage = 100;
    data[bullet_type::rocket_shell].speed = 200.f;
    data[bullet_type::rocket_shell].has_emitter = true;
    data[bullet_type::rocket_shell].ptype = particle::type::propellant;
    return data;
}
std::vector<pickup_data> init_pickup_data()
{
    std::vector<pickup_data> data(pickup::type::type_count);
    data[pickup::type::sm_ammo].texture = textures::entities;
    data[pickup::type::sm_ammo].texture_rect = sf::IntRect(10,10,5,5);
    data[pickup::type::sm_ammo].action = monster::monster_ammo_pickup(pistol_bullet, 320);
    data[pickup::type::sh_ammo].texture = textures::entities;
    data[pickup::type::sh_ammo].texture_rect = sf::IntRect(10,5,5,5);
    data[pickup::type::sh_ammo].action = monster::monster_ammo_pickup(shotgun_bullet, 16);
    data[pickup::type::rk_ammo].texture = textures::entities;
    data[pickup::type::rk_ammo].texture_rect = sf::IntRect(10,15,5,5);
    data[pickup::type::rk_ammo].action = monster::monster_ammo_pickup(rocket_shell, 40);
    data[pickup::type::sm_health_pack].texture = textures::entities;
    data[pickup::type::sm_health_pack].texture_rect = sf::IntRect(10,0,5,5);
    data[pickup::type::sm_health_pack].action = monster::monster_healer(25);
    data[pickup::type::small_pistol].texture = textures::entities;
    data[pickup::type::small_pistol].texture_rect = sf::IntRect(15, 0, 10, 4);
    data[pickup::type::small_pistol].action = monster::monster_weapon_pickup(weapon_type::small_pistol);
    data[pickup::type::basic_shotgun].texture = textures::entities;
    data[pickup::type::basic_shotgun].texture_rect = sf::IntRect(15, 4, 10, 4);
    data[pickup::type::basic_shotgun].action = monster::monster_weapon_pickup(weapon_type::basic_shotgun);
    data[pickup::type::rpg1].texture = textures::entities;
    data[pickup::type::rpg1].texture_rect = sf::IntRect(15, 8, 11, 4);
    data[pickup::type::rpg1].action = monster::monster_weapon_pickup(weapon_type::rpg1);
    return data;
}
std::vector<weapon_data> init_weapon_data()
{
    std::vector<weapon_data> data(weapon_type::weapon_type_count);
    data[weapon_type::small_pistol].ammo_type = bullet_type::pistol_bullet;
    data[weapon_type::small_pistol].automatic = true;
    data[weapon_type::small_pistol].spread = 9;
    data[weapon_type::small_pistol].reload_speed = 60;
    data[weapon_type::small_pistol].num_bullets = 1;
    data[weapon_type::small_pistol].texture = textures::entities;
    data[weapon_type::small_pistol].texture_rect = sf::IntRect(15, 0, 10, 4);
    data[weapon_type::basic_shotgun].ammo_type = bullet_type::shotgun_bullet;
    data[weapon_type::basic_shotgun].automatic = false;
    data[weapon_type::basic_shotgun].spread = 6;
    data[weapon_type::basic_shotgun].reload_speed = 100;
    data[weapon_type::basic_shotgun].num_bullets = 6;
    data[weapon_type::basic_shotgun].texture = textures::entities;
    data[weapon_type::basic_shotgun].texture_rect = sf::IntRect(15, 4, 10, 4);
    data[weapon_type::rpg1].ammo_type = bullet_type::rocket_shell;
    data[weapon_type::rpg1].automatic = false;
    data[weapon_type::rpg1].spread = 8.5;
    data[weapon_type::rpg1].reload_speed = 500;
    data[weapon_type::rpg1].num_bullets = 1;
    data[weapon_type::rpg1].texture = textures::entities;
    data[weapon_type::rpg1].texture_rect = sf::IntRect(15, 8, 11, 4);
    return data;
}
std::vector<particle_data> init_particle_data()
{
    std::vector<particle_data> data(particle::particle_type_count);
    data[particle::type::smoke].color = sf::Color::White;
    data[particle::type::smoke].lifetime = sf::milliseconds(400);
    data[particle::type::fire].lifetime = sf::milliseconds(400);
    data[particle::type::fire].color = sf::Color::Red;
    data[particle::type::propellant].color = sf::Color(255, 255, 50);
    data[particle::type::propellant].lifetime = sf::milliseconds(180);
    return data;
}
std::vector<animation_data> init_animation_data()
{
    std::vector<animation_data> data(animation::type::animation_type_count);
    data[animation::type::explosion].duration = sf::seconds(1);
    data[animation::type::explosion].frame_size = sf::Vector2i(256, 256);
    data[animation::type::explosion].num_frames = 16;
    data[animation::type::explosion].repeat = false;
    data[animation::type::explosion].texture = textures::explosions;
    data[animation::type::explosion].texture_rect = sf::IntRect(0,0, 256 * 4, 256 * 4);
    data[animation::type::player_walk].texture = textures::player_walk;
    data[animation::type::player_walk].texture_rect = sf::IntRect(0, 0, 64, 16);
    data[animation::type::player_walk].duration = sf::milliseconds(800);
    data[animation::type::player_walk].frame_size = sf::Vector2i(16, 16);
    data[animation::type::player_walk].num_frames = 4;
    data[animation::type::player_walk].repeat = true;
    return data;
}
