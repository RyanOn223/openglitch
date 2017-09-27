#ifndef PLAYER_H
#define PLAYER_H

#include <monster.h>


class player : public monster
{
    public:
        player(const texture_manager&  textures);
        virtual ~player();

    protected:

    private:
};

#endif // PLAYER_H
