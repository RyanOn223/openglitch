#include "cursor.h"

cursor::cursor(texture_manager& textures)//), resource_manager<sf::Font, fonts::ID>& fonts)
    : sprite(textures.get(textures::cursor)), entity(1)
{
    scale(0.5f, 0.5f);
    sf::FloatRect bounds = 	sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	//std::unique_ptr<text_node> d(new text_node(fonts, ""));
	//display = d.get();
	//attach_child(std::move(d));
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
