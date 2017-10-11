#include "wall.h"
textures::ID to_texture_ID(wall::type ttype)
{
    switch (ttype)
    {
        default: return textures::wall_tile;
    }
}
wall::wall(type wtype, const texture_manager& textures, float rot) : wall_type(wtype),
            sprite(textures.get(to_texture_ID(wtype))),
            entity(1),
            rotation(rot)
{
    sf::FloatRect bounds = 	sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	sprite.setScale(0.5f, 0.5f);
	sprite.setRotation(rotation);
}
void wall::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
    {
        sf::Vector2f v(getBoundingRect().width, getBoundingRect().height);
        sf::RectangleShape collide_rect(v);
        sf::FloatRect bounds = 	collide_rect.getLocalBounds();
        collide_rect.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        collide_rect.setPosition(getPosition());
        collide_rect.setOutlineColor(sf::Color::Black);
        collide_rect.setFillColor(sf::Color(0,0,0,0));
        collide_rect.setOutlineThickness(.5f);
        target.draw(collide_rect);
    }
    target.draw(sprite, states);
}
unsigned int wall::get_category() const
{
    switch (wall_type)
    {
        case wall1:
            return cmd_category::walls;
        default:
            //lmao
            assert(false);
    }
}
void wall::update_current(sf::Time delta, command_queue& cmds)
{
    set_velocity(0.f, 0.f);
}
sf::FloatRect wall::getBoundingRect() const
{
    sf::FloatRect to_return(getWorldTransform().transformRect(sprite.getGlobalBounds()));
    //to_return.left = getPosition().x;
    //to_return.top = getPosition().y;
    //to_return.width = sprite.getTexture()->getSize().x;
    //to_return.height = sprite.getTexture()->getSize().y;
    //to_return.left -= 1;
    //to_return.top -= 1;
    //to_return.width += 2;
    //to_return.height += 2;
    return to_return;
}
wall::~wall()
{
    //dtor
}
