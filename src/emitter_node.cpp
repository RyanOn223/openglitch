#include "emitter_node.h"

emitter_node::emitter_node(particle::type ptype) : etype(ptype), accumulator(sf::Time::Zero), removal_mark(false)
{
    particle_system = nullptr;
}

emitter_node::~emitter_node()
{
    //dtor
}
void emitter_node::update_current(sf::Time delta, command_queue& cmds)
{
    if (parent->is_dead()) removal_mark = true;
    if (particle_system)
    {
        emit_particles(delta);
    }
    else
    {
        auto finder = [this] (particle_node& container, sf::Time)
        {
            if (container.get_particle_type() == etype)
            {
                particle_system = &container;
            }
        };
        command c;
        c.ccategory = cmd_category::particle_sys;
        c.action = derived_action<particle_node>(finder);
        cmds.push(c);
    }
}
void emitter_node::emit_particles(sf::Time delta)
{
    const float emission_rate = 30.f;
    const sf::Time interval = sf::seconds(1.f) / emission_rate;
    accumulator += delta;

    while (accumulator > interval)
    {
        accumulator -= interval;
        if (particle_system) particle_system->add_particle(getWorldPosition());
    }
}
bool emitter_node::is_marked_for_removal() const
{
    return removal_mark;
}
