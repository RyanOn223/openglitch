#include "collision_manager.h"

//black box routine, inaccurate but fast
double fastpow(double a, double b) {
    struct {
        double d;
        int x[2];
    } u = { a };
    u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;
    return u.d;
}

collision_manager::collision_manager() : the_player(nullptr), the_cursor(nullptr)
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
                monsters.push_back(static_cast<monster*>(to_add));
            }
            else std::cout << "error: attempted to add repeat monster to collision manager\n";
            break;
        case cmd_category::pickups:
            if (pik_itr == pickups.end())
            {
                pickups.push_back(static_cast<pickup*>(to_add));
            }
            else std::cout << "error: attempted to add repeat pickup to collision manager\n";
            break;
        case cmd_category::ally_projectiles:
        case cmd_category::enemy_projectiles:
            if (bul_itr == bullets.end())
            {
                //std::cout << "info: added bullet id: " << to_add << std::endl;
                bullets.push_back(static_cast<projectile*>(to_add));
            }
            else std::cout << "error: attempted to add repeat bullet to collision manager\n";
            break;
        case cmd_category::walls:
            if (wal_itr == walls.end())
            {
                walls.push_back(static_cast<wall*>(to_add));
            }
            else std::cout << "error: attempted to add repeat wall to collision manager\n";
            break;
        case cmd_category::the_player:
            the_player = static_cast<monster*>(to_add);
            break;
        case cmd_category::mouse:
            the_cursor = static_cast<cursor*>(to_add);
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
                //std::cout << "info: erased bullet id: " << *bul_itr << std::endl;
                bullets.erase(bul_itr);
            }
            else std::cout << "error: attempted to remove bullet not in collision manager with id:" << *bul_itr <<std::endl;
            break;
        case cmd_category::enemies:
            if (mon_itr != monsters.end())
            {
                monsters.erase(mon_itr);
            }
            break;
        case cmd_category::walls:
            std::cout << "error: attempted to delete a wall from collision manager\n";
            break;
        case cmd_category::pickups:
            if (pik_itr != pickups.end())
            {
                pickups.erase(pik_itr);
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
    //hey, don't delete anything from the vectors in the inner loops in the functions
    wall_monster_collisions(cmds);
    wall_bullet_collisions(cmds);
    monster_bullet_collisions(cmds);
    monster_pickup_collisions(cmds);
    monster_monster_collisions(cmds);
    cursor_collisions(cmds);
    //std::cout << bullets.size() + monsters.size() + pickups.size() + walls.size() << std::endl;
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
    std::vector<projectile*> remove_bullet = {0};
    for (wall* each_wall : walls)
    {
        for (projectile* blt : bullets)
        {
            if (blt->getBoundingRect().intersects(each_wall->getBoundingRect()))
            {
                blt->destroy();
                //note that the cases for enemy and ally projectiles are the same
                remove_bullet.push_back(blt);
            }
        }
    }
    for (projectile* each_remove : remove_bullet)
    {
        for (projectile* each_bullet : bullets)
        {
            if (each_remove == each_bullet)
            {
                rmv_entity(*each_remove, cmd_category::ally_projectiles);
            }
        }
    }
}
void collision_manager::monster_bullet_collisions(command_queue& cmds)
{
    std::vector<projectile*> remove_bullet = {0};
    for (monster* each_mstr : monsters)
    {
        for (projectile* each_bullet : bullets)
        {
            //std::cout << "attempting to collide bullet: " << each_bullet << std::endl;
            if (each_bullet->getBoundingRect().intersects(each_mstr->getBoundingRect()))
            {
                //std::cout << "current bullets:\n";
                //for (projectile* p_each : bullets) std::cout << p_each << std::endl;
                //std::cout << std::endl;
                each_mstr->damage(each_bullet->get_damage());
                each_bullet->destroy();
                remove_bullet.push_back(each_bullet);
                if (each_mstr->is_dead())
                {
                    rmv_entity(*each_mstr, cmd_category::enemies);
                }
            }
        }
    }
    for (projectile* each_remove : remove_bullet)
    {
        for (projectile* each_bullet : bullets)
        {
            if (each_remove == each_bullet)
            {
                rmv_entity(*each_remove, cmd_category::ally_projectiles);
            }
        }
    }
}
void collision_manager::monster_pickup_collisions(command_queue& cmds)
{
    std::vector<pickup*> remove_pickup = {0};
    for (pickup* each_pickup: pickups)
    {
        if (the_player->getBoundingRect().intersects(each_pickup->getBoundingRect()))
        {
            each_pickup->enable_outline();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                each_pickup->apply(*the_player);
                each_pickup->destroy();
                remove_pickup.push_back(each_pickup);
            }
        }
        //if its stupid and it works..
        else (each_pickup->disable_outline());
    }
    for (pickup* each_remove : remove_pickup)
    {
        for (pickup* each_pickup : pickups)
        {
            if (each_remove == each_pickup)
            {
                rmv_entity(*each_remove, cmd_category::pickups);
            }
        }
    }
}
void collision_manager::monster_monster_collisions(command_queue& cmds)
{
    for (monster* each_lhs : monsters)
    {
        if (the_player->getBoundingRect().intersects(each_lhs->getBoundingRect()))
        {
            the_player->hit_wall = true;
            each_lhs->hit_wall = true;
        }
        for (monster* each_rhs : monsters)
        {
            if (each_lhs != each_rhs)
            {
                if (each_lhs->getBoundingRect().intersects(each_rhs->getBoundingRect()))
                {
                    each_lhs->hit_wall = true;
                    each_rhs->hit_wall = true;
                }
            }
        }
    }
}
void collision_manager::cursor_collisions(command_queue& cmds)
{
    for (monster* each_monster : monsters)
    {
        if (each_monster->getBoundingRect().intersects(the_cursor->getBoundingRect()))
        {
            each_monster->enable_outline();
        }
        else each_monster->disable_outline();
    }
    for (pickup* each_pickup : pickups)
    {
        if (each_pickup->getBoundingRect().intersects(the_cursor->getBoundingRect()))
        {
            each_pickup->enable_outline();
        }
    }
    if (the_cursor->getBoundingRect().intersects(the_player->getBoundingRect()))
    {
        the_player->enable_outline();
    }
    else the_player->disable_outline();
}
void collision_manager::init_shadows(int screenX, int screenY)
{
    shadow_manager.setObstacles(get_obstacles(), 5);
    shadow_manager.setScreenDiagonal(screenX, screenY);
}
std::vector<sf::Vector2f> collision_manager::get_obstacles()
{
    std::vector<sf::Vector2f> obstacles;
    for (wall* each_wall : walls)
    {
        obstacles.push_back(sf::Vector2f(each_wall->getBoundingRect().left, each_wall->getBoundingRect().top));
    }
    return obstacles;
}
void collision_manager::update_shadows(sf::Vector2f view_center)
{
    shadow_manager.update(the_player->getPosition(), view_center);
}
void collision_manager::draw_shadows(sf::RenderWindow* window)
{
    shadow_manager.draw(window);
}
void collision_manager::do_culling(sf::View& current_view)
{
    //O(n)
    sf::Vector2f center = current_view.getCenter();
    sf::Vector2f size = current_view.getSize() / 2.f;
    for (projectile* bullet : bullets)
    {
        //bullets aren't culled till they are 20 pixels off screen
        sf::FloatRect bounds = bullet->getBoundingRect();
        if ((bounds.top > center.y + size.y + 20) ||
           (bounds.top + bounds.height < center.y - size.y - 20))
        {
            bullet->draw_this = false;
        }
        else if ((bounds.left > center.x + size.x +20) ||
                 (bounds.left + bounds.width < center.x - size.x - 20))
        {
            bullet->draw_this = false;
        }
    }
    for (wall* each_wall : walls)
    {
        sf::FloatRect bounds = each_wall->getBoundingRect();
        if ((bounds.top > center.y + size.y) ||
           (bounds.top + bounds.height < center.y - size.y))
        {
            each_wall->draw_this = false;
        }
        else if ((bounds.left > center.x + size.x) ||
                 (bounds.left + bounds.width < center.x - size.x))
        {
            each_wall->draw_this = false;
        }
    }
    for (pickup* each_pickup : pickups)
    {
        sf::FloatRect bounds = each_pickup->getBoundingRect();
        if ((bounds.top > center.y + size.y) ||
           (bounds.top + bounds.height < center.y - size.y))
        {
            each_pickup->draw_this = false;
        }
        else if ((bounds.left > center.x + size.x) ||
                 (bounds.left + bounds.width < center.x - size.x))
        {
            each_pickup->draw_this = false;
        }
    }
    for (monster* each_monster : monsters)
    {
        sf::FloatRect bounds = each_monster->getBoundingRect();
        if ((bounds.top > center.y + size.y) ||
           (bounds.top + bounds.height < center.y - size.y))
        {
            each_monster->draw_this = false;
        }
        else if ((bounds.left > center.x + size.x) ||
                 (bounds.left + bounds.width < center.x - size.x))
        {
            each_monster->draw_this = false;
        }
    }
}
