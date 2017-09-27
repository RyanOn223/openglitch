#ifndef ENTITY_H
#define ENTITY_H
#include "include.h"
#include "scene_node.h"
class entity : public scene_node
{
	public:
		void set_velocity(sf::Vector2f velocity);
		void set_velocity(float vx, float vy);
		sf::Vector2f get_velocity() const;
	private:
		sf::Vector2f velocity;
		virtual void update_current(sf::Time delta);
};
#endif
