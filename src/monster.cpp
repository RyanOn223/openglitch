#include "../include/monster.h"
namespace
{
    const std::vector<weapon_data> weapons = init_weapon_data();
    const std::vector<bullet_data> bullets = init_bullet_data();
    const std::vector<monster_data> monsters = init_monster_data();
    const std::vector<animation_data> animations = init_animation_data();
}
//this function is declared in the global scope of monster.cpp and
//not shown in the class interface
monster::monster(monster::type mtype, const texture_manager& textures, const resource_manager<sf::Font, fonts::ID>& fonts, collision_manager& manager) :
				 monster_type(mtype),
				 sprite(textures.get(monsters[mtype].texture), monsters[mtype].texture_rect),
				 entity(monsters[mtype].healthpoints),
				 cmanager(manager),
				 walk_animation(textures, monsters[mtype].walk_animation),
				 current_ai_state(ai_state::sleep_state)

{
    fire_command.ccategory = cmd_category::air_layer;
    fire_command.action = [this, &textures] (scene_node& node, sf::Time)
        {
            create_bullets(node, textures);
        };
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
	generator = new(std::mt19937_64);
}
void monster::draw_current(sf::RenderTarget& target,
								  sf::RenderStates  states) const
{
    if (draw_this)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
        {
            //draw debug rect
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
        //if we're sitting still or aiming, use the standard sprite
        if (walk_animation.get_type() == animation::type::none || is_aiming || get_velocity() == sf::Vector2f(0.f,0.f)) target.draw(sprite, states);
        //otherwise show the walking animation
        else target.draw(walk_animation, states);
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



    if (get_velocity() != sf::Vector2f(0.f, 0.f));
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
        //calculate velocity vector and point towards it
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
        walk_animation.setRotation(point_towards);
        sprite.setRotation(point_towards);
        //update walking animation
        if (get_velocity() != sf::Vector2f(0,0)) walk_animation.update(delta);
    }
    //sets flag for object culler to use
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
        default:
            assert(false);
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
    std::normal_distribution<float> dis(0.f, 1.f);
    float spread = dis(*generator) / 4;
    theta += (spread / weapons[weapon].spread);
    sf::Vector2f v(hyp * cos(theta), hyp * sin(theta));
    proj->setPosition(getPosition() + (v * 0.02f));
    proj->setRotation(sprite.getRotation() - 180.f);
    proj->set_velocity(v);

    //hardcode rocket flare for now
    if (weapons[weapon].ammo_type == rocket_shell)
    {
        std::unique_ptr<emitter_node> smoke(new emitter_node(particle::type::smoke));
        std::unique_ptr<emitter_node> flame(new emitter_node(particle::type::propellant));
        smoke->setPosition(0,0);
        smoke->setRotation(proj->getRotation());
        flame->setPosition(0,0);
        flame->setRotation(proj->getRotation());
        proj->attach_child(std::move(smoke));
        proj->attach_child(std::move(flame));
        proj->show_explosion = true;
    }
    //node needs to be the air scene layer her
    cmanager.add_entity(proj.get(), static_cast<cmd_category::ID>(proj->get_category()));
    node.attach_child(std::move(proj));


}
sf::FloatRect monster::getBoundingRect() const
{
    sf::FloatRect to_return;

    //TODO figure out why these adjustments are needed here, because they shouldnt be
    to_return.left = getPosition().x - 1;
    to_return.top = getPosition().y - 1;
    //this will break if you scale anything
    sf::IntRect rect(monsters[monster_type].texture_rect);
    to_return.width = rect.width;
    to_return.height = rect.height;
    return to_return;
}
bool monster::is_marked_for_removal() const
{
    return removal_mark;
}
monster::~monster()
{
    delete generator;
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
    return ammo_held[weapons[weapon].ammo_type] > 0;

}
monster::type monster::get_type() const
{
    return monster_type;
}
