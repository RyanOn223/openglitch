#include "../include/scene_node.h"
typedef std::unique_ptr<scene_node> scn_ptr;
scene_node::scene_node()
{
	parent = nullptr;
}
void scene_node::attach_child(scn_ptr child)
{
	child->parent = this;
	children.push_back(std::move(child));
}
scn_ptr scene_node::detach_child(const scene_node& node)
{
	//right here, right now, I understand this lambda function
	//I'm sure that will change

	auto found = std::find_if(	children.begin(), 		//check from beginning
								children.end(),   		//to end of children
								[&]				  		//capture all local vars
								(scn_ptr& p) -> bool 	//the function parameters to pass to the lambda
														//the function definition
								{
									return p.get() == &node;
								}
							 );
	assert(found != children.end());
	//now move the found node out of the container and into a var to pass back
	scn_ptr result = std::move(*found);
	//set its parent to null
	result->parent = nullptr;
	//erase it from the collection
	children.erase(found);
	return result;
}
void scene_node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//this function has to deal with with two different transforms,
	//the transform of this node (stored in terms relative to the parent)
	//and the transform of it's parent (stored in absolute terms)

	//this applies the absolute transform of the parent to the relative
	//transform on this node, resulting in "states" being the absolute transform of this node.
	//note that getTransform() is provided by sf::Transformable
	states.transform *= getTransform();
	//now draw this node with the now-modified states
	draw_current(target, states);
	//now draw the children
	draw_children(target, states);

	//note the recursion here

}
void scene_node::draw_children(sf::RenderTarget& target, sf::RenderStates states) const
{
	//use a c++11 for_each statement just for funsies
	for (const scn_ptr& child : children)
	{
		child->draw(target, states);
	}
}
