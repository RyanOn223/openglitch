#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "animation.h"
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
        projectile(type ptype, const texture_manager& textures, float speed, int damage);
        virtual unsigned int get_category() const;
        virtual sf::FloatRect get_bounding_rect() const;
        float get_max_speed() const;
        int get_damage() const;
        virtual ~projectile();
        virtual sf::FloatRect getBoundingRect() const;
        virtual bool is_marked_for_removal() const;
        projectile::type get_type();
        bool show_explosion;
    protected:

    private:
        virtual void update_current(sf::Time delta, command_queue& cmds);
        virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
        sf::Vector2f last_position;
        type ptype;
        sf::Sprite sprite;
        sf::Vector2f target_direction;
        float speed;
        int damage;
        int turns_alive;
        animation explosion;
};

#endif // PROJECTILE_H
