#include "../include/monster.h"

namespace
{
    const std::vector<monster_data> mon_data = init_data();
}
//this function is declared in the global scope of monster.cpp and
//not shown in the class interface
textures::ID to_texture_ID(monster::type Type)
{
    //to do: abstract this to allow loading from a file
	switch (Type)
	{
		case monster::small_mutant:
			return textures::small_mutant;
		case monster::large_mutant:
			return textures::large_mutant;
        case monster::player:
            return textures::player;
	}
}
//this is a little confusing. the first 'type' refers to the type of variable
//to pass in to the constructor, while the second 'type' refers to the member variable
//to initialize, in this case it get initialized to the same type as the
//monster type passed into the constructor.
monster::monster(monster::type mtype, const texture_manager& textures, const resource_manager<sf::Font, fonts::ID>& fonts, int hp) :
				 monster_type(mtype), sprite(textures.get(to_texture_ID(monster_type))), entity(hp)
{
	//set the sprites origin to the center of the local bounds of its bounding rectangle.
	//aka move its reference point from the top left corner to the center of the sprite
	sf::FloatRect bounds = 	sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	//std::cout << "new monster, type: " << mtype << std::endl;
	std::unique_ptr<text_node> tx(new text_node(fonts, ""));
	health_text = tx.get();
	attach_child(std::move(tx));
}
void monster::draw_current(sf::RenderTarget& target,
								  sf::RenderStates  states) const
{
	//the result of all this inheritence, oop, recursion, and sfml usage is that this is a super simple call
	//if (this->get_category() == cmd_category::the_player) std::cout << "drawing player\n";
	target.draw(sprite, states);
}
unsigned int monster::get_category() const
{
    //std::cout << "get category called with result: ";
    switch(monster_type)
    {
        case small_mutant:
        case large_mutant:
            //std::cout << "enemy\n";
            return cmd_category::enemies;
            //break;
        case player:
            //std::cout << "player\n";
            return cmd_category::the_player;
            //break;
    }
}
void monster::update_current(sf::Time delta)
{
    health_text->set_string(std::to_string(get_hp()) + "hp");
    health_text->setPosition(4.f, 4.f);
    health_text->setRotation(-getRotation());
    move(get_velocity() * delta.asSeconds());
}
