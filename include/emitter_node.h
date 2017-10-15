#ifndef EMITTER_NODE_H
#define EMITTER_NODE_H
#include "include.h"
//#include "scene_node.h"
#include "particle_node.h"
class particle_node;
class emitter_node : public scene_node
{
    public:
        explicit emitter_node(particle::type type);
        virtual ~emitter_node();
        virtual void update_current(sf::Time delta, command_queue& cmds);
    protected:

    private:
        sf::Time accumulator;
        particle::type etype;
        particle_node* particle_system;
        void emit_particles(sf::Time);
};

#endif // EMITTER_NODE_H
