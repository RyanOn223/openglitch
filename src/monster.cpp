#include "../include/monster.h"
namespace
{
    const std::vector<weapon_data> weapons = init_weapon_data();
    const std::vector<bullet_data> bullets = init_bullet_data();
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
monster::monster(monster::type mtype, const texture_manager& textures, const resource_manager<sf::Font, fonts::ID>& fonts, int hp, collision_manager& manager) :
				 monster_type(mtype), sprite(textures.get(to_texture_ID(monster_type))), entity(hp), cmanager(manager)
{
    fire_command.ccategory = cmd_category::air_layer;
    fire_command.action = [this, &textures] (scene_node& node, sf::Time)
        {
            create_bullets(node, textures);
        };
	//set the sprites origin to the center of the local bounds of its bounding rectangle.
	//aka move its reference point from the top left corner to the center of the sprite
	sf::FloatRect bounds = 	sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	fire_cooldown = sf::Time::Zero;
	std::unique_ptr<text_node> tx(new text_node(fonts, ""));
	health_text = tx.get();
	attach_child(std::move(tx));
	is_firing = false;
	removal_mark = false;
	hit_wall = false;
	draw_outline = false;
	is_aiming = false;
	weapon = weapon_type::none;
}
void monster::draw_current(sf::RenderTarget& target,
								  sf::RenderStates  states) const
{
    if (draw_this)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
        {
            sf::Vector2f v(getBoundingRect().width, getBoundingRect().height);
            sf::RectangleShape collide_rect(v);
            sf::FloatRect bounds = 	collide_rect.getLocalBounds();
            collide_rect.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            collide_rect.setPosition(getPosition());
            collide_rect.setOutlineColor(sf::Color::Black);
            collide_rect.setFillColor(sf::Color(0,0,0,0));
            collide_rect.setOutlineThickness(.5f);
            target.draw(collide_rect);
        }
        if (draw_outline)
        {
            sf::Vector2f v(getBoundingRect().width, getBoundingRect().height);
            sf::CircleShape outline_circle(std::max((v.x / sqrt(2)), (v.y / sqrt(2))));
            outline_circle.setPosition(getPosition());
            outline_circle.setFillColor(sf::Color(0,0,0,0));
            outline_circle.setOutlineColor(sf::Color::White);
            outline_circle.setOutlineThickness(0.25f);
            outline_circle.setOrigin(outline_circle.getRadius(), outline_circle.getRadius());
            target.draw(outline_circle);
        }
        target.draw(sprite, states);
    }
}
unsigned int monster::get_category() const
{
    switch(monster_type)
    {
        case small_mutant:
        case large_mutant:;
            return cmd_category::enemies;
        case player:
            return cmd_category::the_player;
        default:
            assert(false);
    }
}
void monster::update_current(sf::Time delta, command_queue& cmds)
{
    if (draw_outline) health_text->set_string(std::to_string(get_hp()) + " hp");
    else health_text->set_string("");
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
        setPosition(last_position);
    }
    last_position = getPosition();
    if (is_aiming)
    {
        sf::Vector2f v(get_velocity());
        v = v * 0.5f;
        set_velocity(v);
    }
    if (!hit_wall) move(get_velocity() * delta.asSeconds());
    else hit_wall = false;

    check_launch(delta, cmds);

    if (get_hp() <= 0) removal_mark = true;
    if (!is_aiming)
    {
        sf::Vector2f point;
        float point_towards;
        if (get_velocity() != sf::Vector2f(0.f, 0.f))
        {
            point = (getPosition() + get_velocity());
        }
        else
        {
            point = (getPosition() + last_velocity);
        }
        sf::Vector2f diff(point - getPosition());
        point_towards = (atan2(diff.y, diff.x) * 180/PI);
        sprite.setRotation(point_towards);
    }
    draw_this = true;
}
void monster::fire_weapon()
{
    is_firing = true;
}
void monster::check_launch(sf::Time delta, command_queue& cmds)
{
    if (is_firing && (fire_cooldown <= sf::Time::Zero))
    {
        if (has_ammo())
        {
            cmds.push(fire_command);
            ammo_held[weapons[weapon].ammo_type] -= 1;
        }
        fire_cooldown += sf::milliseconds(weapons[weapon].reload_speed);
        is_firing = false;
        //use the bullet

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
    if (weapon != weapon_type::none)
    {
        for (int x = 0; x < weapons[weapon].num_bullets; x++)
        {
            create_projectile(node, btype, .0f, .0f, textures);
        }
    }
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
    std::unique_ptr<projectile> proj(new projectile(ptype, textures, bullets[weapons[weapon].ammo_type].speed + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 10,
                                                                     bullets[weapons[weapon].ammo_type].damage));
    sf::Vector2f offset(xoff * sprite.getGlobalBounds().width, yoff * sprite.getGlobalBounds().height);
    //determine the velocity vector of the projectile based upon the rotation of this monster. i.e shoot this out the front
    float hyp = proj->get_max_speed();
    float theta = sprite.getRotation() * PI/180.f;
    float spread = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    spread -= 0.5f;
    theta += spread / weapons[weapon].spread;
    sf::Vector2f v(hyp * cos(theta), hyp * sin(theta));
    proj->setPosition(getPosition());
    proj->setRotation(sprite.getRotation() - 180.f);
    proj->set_velocity(v);


    //node needs to be the air scene layer here
    //std::cout << "info: created bullet with id: " << proj.get() << std::endl;
    cmanager.add_entity(proj.get(), static_cast<cmd_category::ID>(proj->get_category()));
    node.attach_child(std::move(proj));


}
sf::FloatRect monster::getBoundingRect() const
{
    sf::FloatRect to_return;

    //TODO figure out why these adjustments are needed here, because they shouldnt be
    to_return.left = getPosition().x - 1;
    to_return.top = getPosition().y - 1;
    to_return.width = sprite.getTexture()->getSize().x - 1;
    to_return.height = sprite.getTexture()->getSize().y - 1;
    return to_return;
}
bool monster::is_marked_for_removal() const
{
    return removal_mark;
}
monster::~monster()
{

}
void monster::enable_outline()
{
    draw_outline = true;
}
void monster::disable_outline()
{
    draw_outline = false;
}
bool monster::has_auto_weapon()
{
    return weapons[weapon].automatic;
}
bool monster::has_ammo()
{
    //std::cout << ammo_held[weapons[weapon].ammo_type] << std::endl;
    return ammo_held[weapons[weapon].ammo_type] > 0;

}
