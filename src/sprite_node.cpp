#include "sprite_node.h"

sprite_node::sprite_node(const sf::Texture& tex)
{
    msprite.setTexture(tex);
}
sprite_node::sprite_node(const sf::Texture& tex, const sf::IntRect& rect)
{
    msprite.setTexture(tex);
    msprite.setTextureRect(rect);
}
sprite_node::~sprite_node()
{

}
void sprite_node::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(msprite);
}
