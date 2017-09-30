#include "cursor.h"

cursor::cursor(texture_manager& textures) : sprite(textures.get(textures::cursor))
{
    scale(0.5f, 0.5f);
    sf::FloatRect bounds = 	sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}
void cursor::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    //std::cout << getPosition().x << ", " << getPosition().y << std::endl;
    target.draw(sprite, states);
}
cursor::~cursor()
{
    //dtor
}
