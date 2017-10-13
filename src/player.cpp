#include "player.h"
player::player(monster* Player)
{
    player_speed = PSPEED;
    the_player = Player;
}
void player::handle_event(const sf::Event& event, command_queue& cmds)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                //if we don't hand this as well here, the fire command might not get passed
                //if the mouse button was down for less than a tick
                command fire;
                fire.ccategory = cmd_category::the_player;
                fire.action = monster::weapon_firer();
                cmds.push(fire);
            }
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
        move_dir.x -= player_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        move_dir.y -= player_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        move_dir.x += player_speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        move_dir.y += player_speed;
    }
    move_command.action = monster::monster_mover(move_dir.x, move_dir.y);
    cmds.push(move_command);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        command aim_command;
        aim_command.ccategory = cmd_category::the_player;
        aim_command.action = monster::monster_aimer(true);
        cmds.push(aim_command);
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
        sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        //automatic weapons get a command push here too
        command fire;
        fire.ccategory = cmd_category::the_player;
        fire.action = monster::weapon_firer();
        if (the_player->has_auto_weapon()) cmds.push(fire);
    }
}
