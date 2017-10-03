#ifndef ENTITY_H
#define ENTITY_H
#include "include.h"
#include "scene_node.h"
class entity : public scene_node
{
	public:
        explicit entity(int hitpoints);
        void repair(int hp);
        void damage(int hp);
        void destroy();

        int get_hp() const;
        bool is_dead() const;

		void set_velocity(sf::Vector2f velocity);
		void set_velocity(float vx, float vy);
		sf::Vector2f get_velocity() const;
		void accelerate(sf::Vector2f v);
	private:
        int hitpoints;
		sf::Vector2f velocity;
		virtual void update_current(sf::Time delta, command_queue& cmds);
};
#endif
