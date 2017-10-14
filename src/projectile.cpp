#include "projectile.h"
#include "datatables.h"
std::vector<bullet_data> data_table = init_bullet_data();
projectile::projectile(type pptype, const texture_manager& textures, float sp, int dmg) :
    ptype(pptype),
    sprite(textures.get(textures::entities), sf::IntRect(0,0,10,1)),
    entity(1),
    turns_alive(0)
{;
	sprite.setScale(0.005f, 0.1f);
	speed = sp;
	damage = dmg;
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
    last_position = getPosition();
    move(get_velocity() * delta.asSeconds());

    turns_alive++;
    if (sprite.getScale().x < 1.f) sprite.setScale(static_cast<float>(turns_alive) / 4.f, 1.f);
    draw_this = true;
}
void projectile::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (draw_this)
    {
        sf::Vector2f v(getBoundingRect().width, getBoundingRect().height);
        sf::RectangleShape collide_rect(v);
        sf::FloatRect bounds = 	collide_rect.getLocalBounds();
        collide_rect.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        collide_rect.setPosition(getPosition());
        collide_rect.setOutlineColor(sf::Color::Black);
        collide_rect.setFillColor(sf::Color(0,0,0,0));
        collide_rect.setOutlineThickness(.5f);
        target.draw(sprite, states);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) target.draw(collide_rect);
    }
}
projectile::~projectile()
{

}
sf::FloatRect projectile::getBoundingRect() const
{
    sf::FloatRect to_return;
    to_return.top = getPosition().y;
    to_return.left = getPosition().x;
    to_return.width = 1.f;
    to_return.height = 1.f;
    return to_return;
}
bool projectile::is_marked_for_removal() const
{
    return is_dead();
}
projectile::type projectile::get_type()
{
    return ptype;
}
