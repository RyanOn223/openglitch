#include "../include/entity.h"
entity::entity(int hp) : hitpoints(hp)
{

}
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
void entity::update_current(sf::Time delta)
{
    move(velocity * delta.asSeconds());
        //debug and 'physics'
    //velocity.x = decel_speed * (velocity.x);
    //velocity.y = decel_speed * (velocity.y);
    //if (velocity.x < 1.f && velocity.x > -1.f) velocity.x = 0.f;
    //if (velocity.y < 1.f && velocity.y > -1.f) velocity.y = 0.f;

}
void entity::accelerate(sf::Vector2f v)
{
    velocity += v;
}
void entity::damage(int hp)
{
    if (hitpoints > 0) hitpoints -= hp;
    if (hitpoints < 0) hitpoints = 0;
}
void entity::repair(int hp)
{
    hitpoints += hp;
}
void entity::destroy()
{
    hitpoints = 0;
}
bool entity::is_dead() const
{
    return hitpoints;
}
int entity::get_hp() const
{
    return hitpoints;
}
