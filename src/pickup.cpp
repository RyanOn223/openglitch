#include "pickup.h"

std::vector<pickup_data> table = init_pickup_data();

textures::ID to_texture_ID(pickup::type Type)
{
    switch (Type)
    {
        case pickup::type::sm_ammo:
            return textures::sm_ammo;
        case pickup::type::sm_health_pack:
            return textures::sm_health_pack;
    }
}

pickup::pickup(pickup::type Type, const texture_manager& textures) : mtype(Type), sprite(textures.get(to_texture_ID(mtype))), entity(1)
{
    sf::FloatRect bounds = 	sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.scale(0.6f, 0.6f);
}
unsigned int pickup::get_category() const
{
    return cmd_category::pickups;
}
sf::FloatRect pickup::getBoundingRect() const
{
    sf::FloatRect to_return;//(getWorldTransform().transformRect(sprite.getGlobalBounds()));
    to_return.left = getPosition().x;
    to_return.top = getPosition().y;
    to_return.width = sprite.getTexture()->getSize().x;
    to_return.height = sprite.getTexture()->getSize().y;
    return to_return;
}
void pickup::apply(monster& the_player) const
{
    table[mtype].action(the_player);
}
void pickup::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
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
pickup::~pickup()
{
    //dtor
}
