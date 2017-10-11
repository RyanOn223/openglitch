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
	sprite.setScale(0.75f, 0.75f);
	//scale(.75f, .75f);
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
    last_position = getPosition();
    //std::cout << getPosition().x << ", " << getPosition().y << std::endl;
    move(get_velocity() * delta.asSeconds());
}
void projectile::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Vector2f v(getBoundingRect().width, getBoundingRect().height);
	//std::cout << v.x << ", " << v.y << std::endl;
	sf::RectangleShape collide_rect(v);
	sf::FloatRect bounds = 	collide_rect.getLocalBounds();
	collide_rect.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	//collide_rect.setRotation(sprite.getRotation());
	collide_rect.setPosition(getPosition());
	collide_rect.setOutlineColor(sf::Color::Black);
	collide_rect.setFillColor(sf::Color(0,0,0,0));
	collide_rect.setOutlineThickness(.5f);
	target.draw(sprite, states);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) target.draw(collide_rect);
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
void projectile::check_node_collision(scene_node& node, std::set<scn_pair>& collision_pairs)
{
    if (this != &node &&
        collision(*this, node) &&
        !is_dead() &&
        !node.is_dead())
    {
        //using minmax ensures that a-b and b-a will always be in the same order
        collision_pairs.insert(std::minmax(static_cast<scene_node*>(this), &node));
    }
    //projectiles should not have children
    assert(children.empty());
}
bool projectile::collision(const scene_node& lhs, const scene_node& rhs)
{
    //sf::Vector2f difference(getPosition() - last_position);
    //sf::FloatRect tween;
    //if (last_position.x == getPosition().x
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}
projectile::type projectile::get_type()
{
    return ptype;
}
