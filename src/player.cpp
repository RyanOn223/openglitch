#include "player.h"
player::player()
{
    player_speed = 30.f;
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        command move_left;
        move_left.ccategory = (cmd_category::the_player);
        move_left.action = monster::monster_mover(-player_speed, 0.f);
        cmds.push(move_left);
    }
}
