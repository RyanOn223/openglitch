#include "player.h"
player::player(const texture_manager& textures) : monster(monster::type::player, textures)
{

}

player::~player()
{
    //dtor
}
