#ifndef SPRITE_NODE_H
#define SPRITE_NODE_H

#include <scene_node.h>


class sprite_node : public scene_node
{
    public:
        explicit sprite_node(const sf::Texture& tex);
        sprite_node(const sf::Texture& text, const sf::IntRect& rect);
        virtual ~sprite_node();

    protected:

    private:
    virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Sprite msprite;
};

#endif // SPRITE_NODE_H
