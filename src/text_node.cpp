#include "text_node.h"

text_node::text_node(const resource_manager<sf::Font, fonts::ID>& fonts, const std::string& ttext) : text(new sf::Text())
{
    text->setFont(fonts.get(fonts::pixel));
    text->setCharacterSize(30);
    text->setScale(0.15f, 0.15f);
    set_string(ttext);
}
void text_node::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*text, states);
}
void text_node::set_string(const std::string& s)
{
    text->setString(s);
    //text->setOrigin(text->getLocalBounds().width / 2, text->getLocalBounds().height / 2);
}
text_node::~text_node()
{
    //dtor
}
