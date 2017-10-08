#include "player.h"
player::player()
{
    player_speed = PSPEED;

}
void player::handle_event(const sf::Event& event, command_queue& cmds)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            //if we don't hand this as well here, the fire command might not get passed
            //if the mouse button was down for less than a tick
            //stupid af
            command fire;
            fire.ccategory = cmd_category::the_player;
            fire.action = monster::weapon_firer();
            cmds.push(fire);
        }
    }
}
player::~player()
{
    //dtor
}
void player::handle_input(command_queue& cmds)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) player_speed = 2 * PSPEED ; else player_speed = PSPEED;
    command move_command;
    move_command.ccategory = (cmd_category::the_player);
    sf::Vector2f move_dir(0.f,0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        //move_left.action = monster::monster_mover(-player_speed, 0.f);
        move_dir.x -= player_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        //move_up.action = monster::monster_mover(0.f, -player_speed);
        move_dir.y -= player_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        //move_right.action = monster::monster_mover(player_speed, 0.f);
        move_dir.x += player_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        //move_down.action = monster::monster_mover(0.f, player_speed);
        move_dir.y += player_speed;
    }
    move_command.action = monster::monster_mover(move_dir.x, move_dir.y);
    cmds.push(move_command);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        //check here too, for automatic weapons
        command fire;
        fire.ccategory = cmd_category::the_player;
        fire.action = monster::weapon_firer();
        cmds.push(fire);
    }
}
