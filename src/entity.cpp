#include "../include/entity.h"
void entity::set_velocity(sf::Vector2f v)
{
	velocity = v;
}
void entity::set_velocity(float vx, float vy)
{
	velocity.x = vx;
	velocity.y = vy;
}
sf::Vector2f entity::get_velocity() const
{
	return velocity;
}
