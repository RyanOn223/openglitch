#ifndef SCENE_NODE_H
#define SCENE_NODE_H
#include "include.h"
#include "command_queue.h"

//inherit classes for drawing and transforming from SFML, as well as
//mark non-copyable
class scene_node : public sf::Drawable, public sf::Transformable,
				   private sf::NonCopyable
{
	private:
		typedef std::unique_ptr<scene_node> scn_ptr;
	public:
        typedef std::pair<scene_node*, scene_node*> scn_pair;
        //constructor, just assings parent to null;
		scene_node();
		scene_node(unsigned int category);
		void    attach_child(scn_ptr child);
		scn_ptr detach_child(const scene_node& node);
		void    update(sf::Time delta, command_queue& cmds);
		sf::Vector2f get_abs_position() const;
		//keep this word virtual in mind
		virtual unsigned int get_category() const;
        void on_command(const command& cmd, sf::Time delta);
        void print();
        sf::Transform getWorldTransform() const;
        void check_node_collision(scene_node& node, std::set<scn_pair>& collision_pairs);
        void check_scene_collision(scene_node& scene_graph, std::set<scn_pair>& collision_pairs);
        virtual bool is_dead() const;
        //virtual void destroy();
        virtual bool is_marked_for_removal() const;
        void remove_wrecks();
	private:
		std::vector<scn_ptr> children;
		scene_node* parent;
	private:
        unsigned int category;
		//draw function, called by the window due to the wonders of SFML, which is why it can be private
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		//draws only the current node
		virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void update_current(sf::Time delta, command_queue& cmds);
		void         update_children(sf::Time delta, command_queue& cmds);

		void draw_children(sf::RenderTarget& target, sf::RenderStates states) const;

        bool collision(const scene_node& lhs, const scene_node& rhs);
        virtual sf::FloatRect getBoundingRect() const;
        /* from the book, usage of draw():
		 * sf::RenderWindow window(...);
		 * SceneNode::Ptr node(...);
		 * window.draw(*node); // note: no node->draw(window) here!
		 */
};
#endif // SCENE_NODE_H
