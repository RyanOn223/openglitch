#include "projectile.h"
#include "datatables.h"
std::vector<bullet_data> data_table = init_bullet_data();
projectile::projectile(type pptype, const texture_manager& textures, float sp, int dmg) :
    ptype(pptype),
    sprite(textures.get(textures::bullet)),
    entity(1)
{
    sf::FloatRect bounds = 	sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	scale(.75f, .75f);
	speed = sp;
	damage = dmg;
	//std::cout << "projectile created with dmg:" << damage << std::endl;
}
unsigned int projectile::get_category() const
{
    switch (ptype)
    {
        case ally_bullet:
            return cmd_category::ally_projectiles;
        case enemy_bullet:
            return cmd_category::enemy_projectiles;
    }
}
sf::FloatRect projectile::get_bounding_rect() const
{
    return sprite.getGlobalBounds();
}
float projectile::get_max_speed() const
{
    return speed;
}
int projectile::get_damage() const
{
    if (damage > 0) return damage;
    else return 0;
}
void projectile::update_current(sf::Time delta, command_queue& cmds)
{
    //std::cout << getPosition().x << ", " << getPosition().y << std::endl;
    move(get_velocity() * delta.asSeconds());
}
void projectile::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}
projectile::~projectile()
{

}
sf::FloatRect projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(sprite.getGlobalBounds());
}
bool projectile::is_marked_for_removal() const
{
    return is_dead();
}
