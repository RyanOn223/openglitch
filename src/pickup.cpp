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
    sprite.scale(0.6f, 0.6f);
}
unsigned int pickup::get_category() const
{
    return cmd_category::pickups;
}
sf::FloatRect pickup::getBoundingRect() const
{
    return getWorldTransform().transformRect(sprite.getGlobalBounds());
}
void pickup::apply(monster& the_player) const
{
    table[mtype].action(the_player);
}
void pickup::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}
pickup::~pickup()
{
    //dtor
}
