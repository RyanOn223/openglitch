#include "cursor.h"

cursor::cursor(texture_manager& textures)
    : sprite(textures.get(textures::cursor)), entity(1)
{
    scale(0.5f, 0.5f);
    sf::FloatRect bounds = 	sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}
void cursor::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}
cursor::~cursor()
{
    //dtor
}
sf::FloatRect cursor::getBoundingRect() const
{
    sf::FloatRect to_return;
    to_return.top = getPosition().y + 1;
    to_return.left = getPosition().x + 1;
    to_return.width = 1;
    to_return.height = 1;
    return to_return;
}
