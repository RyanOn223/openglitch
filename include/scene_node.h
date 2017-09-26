    #ifndef SCENE_NODE_H
#define SCENE_NODE_H
#include "include.h"
//inherit classes for drawing and transforming from SFML, as well as
//mark non-copyable
class scene_node : public sf::Drawable, public sf::Transformable,
				   private sf::NonCopyable
{
	private:
		typedef std::unique_ptr<scene_node> scn_ptr;
	public:
		scene_node();
		void attach_child(scn_ptr child);
		scn_ptr detach_child(const scene_node& node);
	private:
		std::vector<scn_ptr> children;
		scene_node* parent;
	private:
		//draw function, called by the window due to the wonders of SFML
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		//draws only the current node
		virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const = 0;

		/* from the book, usage of draw():
		 * sf::RenderWindow window(...);
		 * SceneNode::Ptr node(...);
		 * window.draw(*node); // note: no node->draw(window) here!
		 */
		void draw_children(sf::RenderTarget& target, sf::RenderStates states) const;

};
#endif // SCENE_NODE_H
