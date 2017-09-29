#ifndef SCENE_NODE_H
#define SCENE_NODE_H
#include "include.h"
#include "command.h"
//inherit classes for drawing and transforming from SFML, as well as
//mark non-copyable
class scene_node : public sf::Drawable, public sf::Transformable,
				   private sf::NonCopyable
{
	private:
		typedef std::unique_ptr<scene_node> scn_ptr;
	public:
        //constructor, just assings parent to null;
		scene_node();
		void    attach_child(scn_ptr child);
		scn_ptr detach_child(const scene_node& node);
		void    update(sf::Time delta);
		sf::Vector2f get_abs_position() const;
		//keep this word virtual in mind
		virtual unsigned int get_category() const;
        void on_command(const command& cmd, sf::Time delta);
        void print();
	private:
		std::vector<scn_ptr> children;
		scene_node* parent;
	private:
		//draw function, called by the window due to the wonders of SFML, which is why it can be private
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		//draws only the current node
		virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void update_current(sf::Time delta);
		void         update_children(sf::Time delta);

		void draw_children(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::Transform getWorldTransform() const;

        /* from the book, usage of draw():
		 * sf::RenderWindow window(...);
		 * SceneNode::Ptr node(...);
		 * window.draw(*node); // note: no node->draw(window) here!
		 */
};
#endif // SCENE_NODE_H
