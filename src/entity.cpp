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
void entity::update_current(sf::Time delta, command_queue& cmds)
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
    //std::cout << "took damage: " << hp << std::endl;
    if (hitpoints > 0) hitpoints -= hp;
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
    return hitpoints <= 0;
}
int entity::get_hp() const
{
    return hitpoints;
}
bool entity::is_marked_for_removal() const
{
    return (hitpoints <= 0);
}
float entity::angle_to(entity& other)
{
    sf::Vector2f diff = other.getPosition() - getPosition();
    return atan2(diff.y, diff.x) * 180/PI;
}
