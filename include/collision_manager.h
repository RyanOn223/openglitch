#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H
#include "include.h"
#include "wall.h"
#include "monster.h"
#include "projectile.h"
#include "pickup.h"
#include "shadow.h"
#include "command_queue.h"
class pickup;
class collision_manager
{
    public:
        collision_manager(monster* Player);
        collision_manager();
        virtual ~collision_manager();
        void check_collisions(command_queue& cmds);
        void add_entity(entity*, cmd_category::ID type);
        void rmv_entity(entity&, cmd_category::ID type);
        void draw_shadows(sf::RenderTarget& target, sf::RenderStates states);
    protected:

    private:
        std::vector<wall*> walls;
        std::vector<monster*> monsters;
        monster* the_player;
        std::vector<projectile*> bullets;
        std::vector<pickup*> pickups;
    private:
        void wall_monster_collisions(command_queue& cmds);
        void wall_bullet_collisions(command_queue& cmds);
        void monster_bullet_collisions(command_queue& cmds);
        void monster_pickup_collisions(command_queue& cmds);
        ShadowHandler shadow_manager;
};
#endif // COLLISION_MANAGER_H
