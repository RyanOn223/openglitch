#include "datatables.h"
std::vector<monster_data> init_monster_data()
{
    std::vector<monster_data> data(monster::type::type_count);
    data[monster::type::player].healthpoints = 100;
    data[monster::type::player].speed = PSPEED;
    data[monster::type::player].texture = textures::player;
    return data;
}
std::vector<bullet_data> init_bullet_data()
{
    std::vector<bullet_data> data(bullet_type::bullet_type_count);
    data[bullet_type::pistol_bullet].damage = 10;
    data[bullet_type::pistol_bullet].speed = 400.f;
    data[bullet_type::shotgun_bullet].damage = 3;
    data[bullet_type::shotgun_bullet].speed = 300.f;
    return data;
};
std::vector<pickup_data> init_pickup_data()
{
    std::vector<pickup_data> data(pickup::type::type_count);
    data[pickup::type::sm_ammo].texture = textures::sm_ammo;
    data[pickup::type::sm_ammo].action = monster::monster_ammo_pickup(pistol_bullet, 32);
    data[pickup::type::sh_ammo].texture = textures::sh_ammo;
    data[pickup::type::sh_ammo].action = monster::monster_ammo_pickup(shotgun_bullet, 16);
    data[pickup::type::sm_health_pack].texture = textures::sm_health_pack;
    data[pickup::type::sm_health_pack].action = monster::monster_healer(25);
    data[pickup::type::small_pistol].texture = textures::small_pistol;
    data[pickup::type::small_pistol].action = monster::monster_weapon_pickup(weapon_type::small_pistol);
    data[pickup::type::basic_shotgun].texture = textures::basic_shotgun;
    data[pickup::type::basic_shotgun].action = monster::monster_weapon_pickup(weapon_type::basic_shotgun);
    return data;
}
std::vector<weapon_data> init_weapon_data()
{
    std::vector<weapon_data> data(weapon_type::weapon_type_count);
    data[weapon_type::small_pistol].ammo_type = bullet_type::pistol_bullet;
    data[weapon_type::small_pistol].automatic = true;
    data[weapon_type::small_pistol].spread = 10;
    data[weapon_type::small_pistol].reload_speed = 60;
    data[weapon_type::small_pistol].num_bullets = 1;
    data[weapon_type::small_pistol].texture = textures::small_pistol;
    data[weapon_type::basic_shotgun].ammo_type = bullet_type::shotgun_bullet;
    data[weapon_type::basic_shotgun].automatic = false;
    data[weapon_type::basic_shotgun].spread = 9;
    data[weapon_type::basic_shotgun].reload_speed = 300;
    data[weapon_type::basic_shotgun].num_bullets = 8;
    data[weapon_type::basic_shotgun].texture = textures::basic_shotgun;
    return data;
}
