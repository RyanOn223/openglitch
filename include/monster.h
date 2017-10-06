#ifndef MONSTER_H
#define MONSTER_H
#include "include.h"
#include "entity.h"
#include "resource_manager.h"
#include "texture_manager.h"
#include "datatables.h"
#include "text_node.h"
#include "projectile.h"
class monster : public entity
{
	public:
	enum type
	{
		small_mutant,
		large_mutant,
		player,
		type_count,
	};
	private:
		typedef resource_manager<sf::Texture, textures::ID> texture_holder;
	public:
		explicit monster(type _type, const texture_manager& textures, const resource_manager<sf::Font, fonts::ID>& fonts, int hp);
		virtual void draw_current(sf::RenderTarget& target,
								  sf::RenderStates  states) const;
        virtual unsigned int get_category() const;
        virtual void update_current(sf::Time delta, command_queue& cmds);
        struct monster_mover
        {
            monster_mover(float vx, float vy) : velocity(vx, vy) {}
            void operator()(scene_node& node, sf::Time delta) const
            {
                //assume the node we've been passed represents a monster reference
                //this is safe only here, even then watch out later for this
                monster& mon = static_cast<monster&>(node);
                if (!mon.hit_wall) mon.accelerate(velocity);
                else mon.hit_wall = false;
            }
            sf::Vector2f velocity;
        };
        struct monster_rotator
        {
            monster_rotator(float angle) : theta(angle) {}
            void operator()(scene_node& node, sf::Time delta) const
            {
                monster& mon = static_cast<monster&>(node);
                //mon.setRotation(theta);
                mon.sprite.setRotation(theta);
            }
            float theta;
        };
        struct weapon_firer
        {
            weapon_firer() {}
            void operator()(scene_node& node, sf::Time delta) const
            {
                monster& mon = static_cast<monster&>(node);
                mon.fire_weapon();
            }
        };
        struct monster_healer
        {
            monster_healer(int hp) : healthpoints(hp) {}
            void operator()(scene_node& node) const
            {
                monster& mon = static_cast<monster&>(node);
                mon.repair(healthpoints);
            }
            int healthpoints;
        };
        void fire_weapon();
        void create_bullets(scene_node& node, const texture_manager& textures) const;
        void create_projectile(scene_node& node, projectile::type ptype, float xoff, float yoff, const texture_manager& textures) const;
        bool is_ally() const;
        virtual sf::FloatRect getBoundingRect() const;
        virtual bool is_marked_for_removal() const;
        bool hit_wall;
	private:

        bool removal_mark;
		type monster_type;
		sf::Sprite sprite;
		text_node* health_text;
		bool is_firing;
		void check_launch(sf::Time delta, command_queue& cmds);
		sf::Time fire_cooldown;
		command fire_command;
};
#endif
