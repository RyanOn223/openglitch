#ifndef PLAYER_H
#define PLAYER_H

#include "monster.h"
#include "command_queue.h"

class player
{
    public:
        player();
        virtual ~player();
        void handle_event(const sf::Event& event, command_queue& cmds);
        void handle_input(command_queue& cmds);
    protected:

    private:
        float player_speed;
};

#endif // PLAYER_H
