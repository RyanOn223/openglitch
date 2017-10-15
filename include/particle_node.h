#ifndef PARTICLE_NODE_H
#define PARTICLE_NODE_H
#include "include.h"
#include "scene_node.h"
#include "datatables.h"
class particle_node : public scene_node
{
    public:
        virtual void update_current(sf::Time delta, command_queue& cmds);
        virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const;
    public:
        explicit particle_node(particle::type type, const texture_manager& textures);
        void add_particle(sf::Vector2f pos);
        particle::type get_particle_type() const;
        virtual unsigned int get_category() const;
        virtual ~particle_node();

    protected:

    private:
        std::deque<particle> particles;
        const sf::Texture& texture;
        const particle::type ptype;
        mutable sf::VertexArray vertex_array;
        mutable bool needs_vertex_update;
        void compute_vertices() const;
        void add_vertex(float worldX, float worldY, float tcoordX, float tcoordY, const sf::Color& color) const;
};

#endif // PARTICLE_NODE_H
