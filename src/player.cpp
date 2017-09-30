#include "player.h"
player::player()
{
    player_speed = PSPEED;

}
void player::handle_event(const sf::Event& event, command_queue& cmds)
{
}
player::~player()
{
    //dtor
}
void player::handle_input(command_queue& cmds)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) player_speed = 2 * PSPEED; else player_speed = PSPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        command move_left;
        move_left.ccategory = (cmd_category::the_player);
        move_left.action = monster::monster_mover(-player_speed, 0.f);
        cmds.push(move_left);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        command move_up;
        move_up.ccategory = (cmd_category::the_player);
        move_up.action = monster::monster_mover(0.f, -player_speed);
        cmds.push(move_up);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        command move_right;
        move_right.ccategory = (cmd_category::the_player);
        move_right.action = monster::monster_mover(player_speed, 0.f);
        cmds.push(move_right);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        command move_down;
        move_down.ccategory = (cmd_category::the_player);
        move_down.action = monster::monster_mover(0.f, player_speed);
        cmds.push(move_down);
    }
}
