#include "particle_node.h"

std::vector<particle_data> ptable = init_particle_data();


particle_node::particle_node(particle::type type, const texture_manager& textures) :
        scene_node(),
        ptype(type),
        texture(textures.get(textures::particles)),
        vertex_array(sf::Quads),
        particles(),
        needs_vertex_update(true)
{
}
void particle_node::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (needs_vertex_update)
    {
        compute_vertices();
        needs_vertex_update = false;
    }
    states.texture = &texture;
    target.draw(vertex_array, states);
}
void particle_node::compute_vertices() const
{
    sf::Vector2f size(texture.getSize());
    sf::Vector2f half = size / 2.f;
    vertex_array.clear();

    for (const particle& each_p : particles)
    {
        sf::Vector2f pos = each_p.position;
        sf::Color c = each_p.color;
        float ratio = each_p.lifetime.asSeconds() / ptable[ptype].lifetime.asSeconds();
        c.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));
        add_vertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, c);
        add_vertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, c);
        add_vertex(pos.x + half.x, pos.y + half.y, size.x, size.y, c);
        add_vertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, c);
    }
}
particle_node::~particle_node()
{
    //dtor
}

unsigned int particle_node::get_category() const
{
    return cmd_category::particle_sys;
}
particle::type particle_node::get_particle_type() const
{
    return ptype;
}
void particle_node::add_particle(sf::Vector2f pos)
{
    //ptype = particle::type::smoke;
    particle newp;
    newp.position = pos;
    newp.color = ptable[ptype].color;
    newp.lifetime = ptable[ptype].lifetime;
    particles.push_back(newp);
}
void particle_node::update_current(sf::Time delta, command_queue& cmds)
{
    while (!particles.empty() &&
            particles.front().lifetime <= sf::Time::Zero) particles.pop_front();
    for (particle& each_p : particles)
    {
        each_p.lifetime -= delta;
    }
    needs_vertex_update = true;
}
void particle_node::add_vertex(float worldX, float worldY, float tcoordX, float tcoordY, const sf::Color& color) const
{
    sf::Vertex vertex;
    vertex.position = sf::Vector2f(worldX, worldY);
    vertex.texCoords = sf::Vector2f(tcoordX, tcoordY);
    vertex.color = color;
    vertex_array.append(vertex);
}
