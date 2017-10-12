#ifndef COMMAND_H
#define COMMAND_H
#include "include.h"
class scene_node;
class monster;
class command
{
    public:
        command();
        std::function<void(scene_node&, sf::Time)> action;
        unsigned int ccategory;


    protected:

    private:
};

#endif // COMMAND_H
