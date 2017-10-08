#include "../include/monster.h"

namespace
{
    const std::vector<monster_data> mon_data = init_monster_data();
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
    fire_command.ccategory = cmd_category::air_layer;
    fire_command.action = [this, &textures] (scene_node& node, sf::Time)
        {
            create_bullets(node, textures);
        };
	//set the sprites origin to the center of the local bounds of its bounding rectangle.
	//aka move its reference point from the top left corner to the center of the sprite
	fire_cooldown = sf::Time::Zero;
	//scale(2.f, 2.f);
	sf::FloatRect bounds = 	sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	//std::cout << "new monster, type: " << mtype << std::endl;
	std::unique_ptr<text_node> tx(new text_node(fonts, ""));
	health_text = tx.get();
	attach_child(std::move(tx));
	is_firing = false;
	removal_mark = false;
	hit_wall = false;
}
void monster::draw_current(sf::RenderTarget& target,
								  sf::RenderStates  states) const
{
	//the result of all this inheritence, oop, recursion, and sfml usage is that this is a super simple call
	//if (this->get_category() == cmd_category::the_player) std::cout << "drawing player\n";
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
void monster::update_current(sf::Time delta, command_queue& cmds)
{
    health_text->set_string(std::to_string(get_hp()) + " hp");
    health_text->setPosition(-4.f, -7.f);
    health_text->setRotation(-getRotation());
    if ((get_velocity().x != 0.f && get_velocity().y == 0.f) ||
        (get_velocity().x == 0.f && get_velocity().y != 0.f) ||
        (get_velocity().x != 0.f && get_velocity().y != 0.f))
        {
            last_velocity = get_velocity();
        }
    if (hit_wall)
    {
        if (get_velocity().x != 0.f && get_velocity().y != 0.f) move(-get_velocity() * delta.asSeconds() / 2.5f);
        else move(-last_velocity * delta.asSeconds() / 2.5f);
        hit_wall = false;
    }
    else move(get_velocity() * delta.asSeconds());
    check_launch(delta, cmds);
    if (get_hp() <= 0) removal_mark = true;
}
void monster::fire_weapon()
{
    //if (get_category() == cmd_category::the_player) std::cout << "fire player weapon\n";
    is_firing = true;
}
void monster::check_launch(sf::Time delta, command_queue& cmds)
{
    if (is_firing && (fire_cooldown <= sf::Time::Zero))
    {
        //std::cout << "pushed fire command\n";
        cmds.push(fire_command);
        fire_cooldown += sf::milliseconds(200);
        is_firing = false;
    }
    else if (fire_cooldown > sf::Time::Zero)
    {
        fire_cooldown -= delta;
    }
    //reset to false, otherwise old firing commands can persist to the next reload and you'll fire twice
    is_firing = false;
}
void monster::create_bullets(scene_node& node, const texture_manager& textures) const
{
    projectile::type btype;
    if (is_ally()) btype = projectile::type::ally_bullet;
    else btype = projectile::type::enemy_bullet;
    create_projectile(node, btype, .0f, .0f, textures);
    if (btype == projectile::type::enemy_bullet) std::cout << "created enemy bullet\n";
}
bool monster::is_ally() const
{
    switch (monster_type)
    {
        case monster::type::large_mutant:
        case monster::type::small_mutant:
        return false;
        case monster::type::player:
        return true;
    }
}
void monster::create_projectile(scene_node& node, projectile::type ptype, float xoff, float yoff, const texture_manager& textures) const
{
    //create the new projectile and calculate its offset
    std::unique_ptr<projectile> proj(new projectile(ptype, textures, 300.f, 5));
    sf::Vector2f offset(xoff * sprite.getGlobalBounds().width, yoff * sprite.getGlobalBounds().height);
    //determine the velocity vector of the projectile based upon the rotation of this monster. i.e shoot this out the front
    float hyp = proj->get_max_speed();
    float theta = sprite.getRotation() * PI/180.f;
    sf::Vector2f v(hyp * cos(theta), hyp * sin(theta));
    proj->setPosition(getPosition());
    proj->setRotation(sprite.getRotation());
    proj->set_velocity(v);
    //std::cout << proj->getPosition().x << ", " << proj->getPosition().y << std::endl;
    //node needs to be the air scene layer here
    node.attach_child(std::move(proj));
}
sf::FloatRect monster::getBoundingRect() const
{
    sf::FloatRect to_return;//(getWorldTransform().transformRect(sprite.getGlobalBounds()));
    to_return.left = getPosition().x;
    to_return.top = getPosition().y;
    to_return.width = sprite.getTexture()->getSize().x;
    to_return.height = sprite.getTexture()->getSize().y;
    return to_return;
}
bool monster::is_marked_for_removal() const
{
    return removal_mark;
}
