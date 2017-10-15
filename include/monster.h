#ifndef MONSTER_H
#define MONSTER_H
#include "include.h"
#include "entity.h"
#include "resource_manager.h"
#include "texture_manager.h"
#include "datatables.h"
#include "text_node.h"
#include "projectile.h"
#include "collision_manager.h"
#include "emitter_node.h"
class collision_manager;
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
		explicit monster(type _type,
                         const texture_manager& textures,
                         const resource_manager<sf::Font,
                         fonts::ID>& fonts,
                         collision_manager& cmanager);
		virtual void draw_current(sf::RenderTarget& target,
								  sf::RenderStates  states) const;
        ~monster();
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
            }
            sf::Vector2f velocity;
        };
        struct monster_rotator
        {
            monster_rotator(float angle) : theta(angle) {}
            void operator()(scene_node& node, sf::Time delta) const
            {
                monster& mon = static_cast<monster&>(node);
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
        struct monster_aimer
        {
            monster_aimer(bool aim) : to_aim(aim) {}
            void operator()(scene_node& node, sf::Time delta) const
            {
                monster& mon = static_cast<monster&>(node);
                mon.is_aiming = to_aim;
            }
            bool to_aim;
        };
        struct monster_weapon_pickup
        {
            monster_weapon_pickup(weapon_type type) : wtype(type) {}
            void operator()(scene_node& node) const
            {
                monster& mon = static_cast<monster&>(node);
                mon.other_weapon = mon.weapon;
                mon.weapon = wtype;
            }
            weapon_type wtype;
        };
        struct monster_ammo_pickup
        {
            monster_ammo_pickup(bullet_type Btype, int amt) : btype(Btype), amount(amt) {}
            void operator()(scene_node& node) const
            {
                monster& mon = static_cast<monster&>(node);
                mon.ammo_held[btype] += amount;
            }
            bullet_type btype;
            int amount;
        };
        struct monster_weapon_swapper
        {
            monster_weapon_swapper() {}
            void operator()(scene_node& node, sf::Time delta) const
            {
                monster& mon = static_cast<monster&>(node);
                std::swap(mon.weapon, mon.other_weapon);
            }
        };
        void fire_weapon();
        void create_bullets(scene_node& node, const texture_manager& textures) const;
        void create_projectile(scene_node& node, projectile::type ptype, float xoff, float yoff, const texture_manager& textures) const;
        bool is_ally() const;
        virtual sf::FloatRect getBoundingRect() const;
        virtual bool is_marked_for_removal() const;
        bool hit_wall;
        void enable_outline();
        void disable_outline();
        bool is_aiming;
        weapon_type weapon;
        weapon_type other_weapon;
        bool has_auto_weapon();
        bool has_ammo();
        std::array<int, bullet_type::bullet_type_count> ammo_held{{0}};
	private:
        collision_manager& cmanager;
        bool removal_mark;
		type monster_type;
		sf::Sprite sprite;
		text_node* health_text;
		bool is_firing;
		void check_launch(sf::Time delta, command_queue& cmds);
		sf::Time fire_cooldown;
		command fire_command;
		bool draw_outline;

};
#endif
