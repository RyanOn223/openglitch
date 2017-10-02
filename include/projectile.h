#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entity.h"
#include "command_queue.h"
#include "texture_manager.h"

class projectile : public entity
{
    public:
        enum type
        {
            ally_bullet,
            enemy_bullet,
            type_count,
        };
        projectile(type ptype, const texture_manager& textures);
        virtual unsigned int get_category() const;
        virtual sf::FloatRect get_bounding_rect() const;
        float get_max_speed() const;
        int get_damage() const;
        virtual ~projectile();

    protected:

    private:
        virtual void update_current(sf::Time delta);//, command_queue& cmds);
        virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
        type ptype;
        sf::Sprite sprite;
        sf::Vector2f target_direction;
};

#endif // PROJECTILE_H
