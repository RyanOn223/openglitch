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
    std::vector<bullet_data> data(projectile::type::type_count);
    data[projectile::type::ally_bullet].damage = 10;
    data[projectile::type::ally_bullet].speed = 100.f;
    return data;
};
std::vector<pickup_data> init_pickup_data()
{
    std::vector<pickup_data> data(pickup::type::type_count);
    data[pickup::type::sm_ammo].texture = textures::sm_ammo;
    data[pickup::type::sm_health_pack].texture = textures::sm_health_pack;
    data[pickup::type::sm_health_pack].action = monster::monster_healer(25);
    return data;
}
