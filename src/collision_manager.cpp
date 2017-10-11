#include "collision_manager.h"

collision_manager::collision_manager(monster* Player) : the_player(Player), shadow_manager()
{
    //ctor
}
collision_manager::collision_manager() : the_player(nullptr)
{

}
void collision_manager::add_entity(entity* to_add, cmd_category::ID type)
{
    std::vector<projectile*>::iterator bul_itr = std::find(bullets.begin(), bullets.end(), to_add);
    std::vector<monster*>::iterator mon_itr = std::find(monsters.begin(), monsters.end(), to_add);
    std::vector<pickup*>::iterator pik_itr = std::find(pickups.begin(), pickups.end(), to_add);
    std::vector<wall*>::iterator wal_itr = std::find(walls.begin(), walls.end(), to_add);
    switch (type)
    {
        //I get that this isn't the most elegant, but it works
        case cmd_category::enemies:
            if (mon_itr == monsters.end())
            {
                std::cout << "info: added monster to collision manager\n";
                monsters.push_back(static_cast<monster*>(to_add));
            }
            else std::cout << "error: attempted to add repeat monster to collision manager\n";
            break;
        case cmd_category::pickups:
            if (pik_itr == pickups.end())
            {
                std::cout << "info: added pickup to collision manager\n";
                pickups.push_back(static_cast<pickup*>(to_add));
            }
            else std::cout << "error: attempted to add repeat pickup to collision manager\n";
            break;
        case cmd_category::ally_projectiles:
        case cmd_category::enemy_projectiles:
            if (bul_itr == bullets.end())
            {
                std::cout << "info: added bullet to collision manager\n";
                bullets.push_back(static_cast<projectile*>(to_add));
            }
            else std::cout << "error: attempted to add repeat bullet to collision manager\n";
            break;
        case cmd_category::walls:
            if (wal_itr == walls.end())
            {
                std::cout << "info: added wall to collision manager\n";
                walls.push_back(static_cast<wall*>(to_add));
            }
            else std::cout << "error: attempted to add repeat wall to collision manager\n";
            break;
        case cmd_category::the_player:
            std::cout << "info: added player to collision manager\n";
            the_player = static_cast<monster*>(to_add);
            break;
        default:
            //no other categories should be passed here (yet)
            assert(false);
    }
}
void collision_manager::rmv_entity(entity& to_remove, cmd_category::ID type)
{
    std::vector<projectile*>::iterator bul_itr = std::find(bullets.begin(), bullets.end(), &to_remove);
    std::vector<monster*>::iterator mon_itr = std::find(monsters.begin(), monsters.end(), &to_remove);
    std::vector<pickup*>::iterator pik_itr = std::find(pickups.begin(), pickups.end(), &to_remove);
    switch (type)
    {
        case cmd_category::ally_projectiles:
        case cmd_category::enemy_projectiles:
            if (bul_itr != bullets.end())
            {
                bullets.erase(bul_itr);
                std::cout << "info: removed bullet from collision manager\n";
            }
            else std::cout << "error: attempted to remove bullet not in collision manager\n";
            break;
        case cmd_category::enemies:
            if (mon_itr != monsters.end())
            {
                monsters.erase(mon_itr);
                std::cout << "info: removed monster from collision manager\n";
            }
            break;
        case cmd_category::walls:
            std::cout << "error: attempted to delete a wall from collision manager\n";
            break;
        case cmd_category::pickups:
            if (pik_itr != pickups.end())
            {
                pickups.erase(pik_itr);
                std::cout << "info: removed pickup from collision manager\n";
            }
            break;
        case cmd_category::the_player:
            std::cout << "error: attempted to delete the_player from collision manager\n";
            break;
        default:
            std::cout << "error: attempted to delete un-handled entity from collision manager\n";
            break;
    }
}
collision_manager::~collision_manager()
{
    //dtor
}
void collision_manager::check_collisions(command_queue& cmds)
{
    wall_monster_collisions(cmds);
    wall_bullet_collisions(cmds);
    monster_bullet_collisions(cmds);
    monster_pickup_collisions(cmds);
}
void collision_manager::wall_monster_collisions(command_queue& cmds)
{
    for (wall* each_wall : walls)
    {
        for (monster* each_monster : monsters)
        {
            if (each_monster->getBoundingRect().intersects(each_wall->getBoundingRect()))
            {
                each_monster->hit_wall = true;
            }
        }
        if (the_player->getBoundingRect().intersects(each_wall->getBoundingRect()))
        {
            the_player->hit_wall = true;
        }
    }
}
void collision_manager::wall_bullet_collisions(command_queue& cmds)
{
    for (wall* each_wall : walls)
    {
        for (projectile* blt : bullets)
        {
            if (blt->getBoundingRect().intersects(each_wall->getBoundingRect()))
            {
                blt->destroy();
                //note that the cases for enemy and ally projectiles are the same
                rmv_entity(*blt, cmd_category::ally_projectiles);
            }
        }
    }
}
void collision_manager::monster_bullet_collisions(command_queue& cmds)
{
    for (projectile* each_bullet : bullets)
    {
        for (monster* each_mstr : monsters)
        {
            if (each_bullet->getBoundingRect().intersects(each_mstr->getBoundingRect()))
            {
                each_mstr->damage(each_bullet->get_damage());
                each_bullet->destroy();
                rmv_entity(*each_bullet, cmd_category::ally_projectiles);
                if (each_mstr->is_dead())
                {
                    rmv_entity(*each_mstr, cmd_category::enemies);
                }
            }
        }
    }
}
void collision_manager::monster_pickup_collisions(command_queue& cmds)
{
    for (pickup* each_pickup: pickups)
    {
        if (the_player->getBoundingRect().intersects(each_pickup->getBoundingRect()))
        {
            each_pickup->apply(*the_player);
            each_pickup->destroy();
            rmv_entity(*each_pickup, cmd_category::pickups);
        }
    }
}
