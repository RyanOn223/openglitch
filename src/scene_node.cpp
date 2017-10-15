#include "../include/scene_node.h"
typedef std::unique_ptr<scene_node> scn_ptr;
scene_node::scene_node()
{
	parent = nullptr;
	category = 1;
	draw_this = true;
}
scene_node::scene_node(unsigned int cat)
{
    parent = nullptr;
    category = cat;
    draw_this = true;
}
void scene_node::attach_child(scn_ptr child)
{
	child->parent = this;
	children.push_back(std::move(child));
}
scn_ptr scene_node::detach_child(const scene_node& node)
{
	//

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


}
void scene_node::draw_children(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const scn_ptr& child : children)
	{
		child->draw(target, states);
	}
}
void scene_node::update(sf::Time delta, command_queue& cmds)
{
    update_current(delta, cmds);
    update_children(delta, cmds);
}
void scene_node::update_current(sf::Time delta, command_queue& cmds)
{

}
void scene_node::update_children(sf::Time delta, command_queue& cmds)
{
    for (scn_ptr& child : children)
    {
        child->update(delta, cmds);
    }
}
sf::Vector2f scene_node::get_abs_position() const
{
    //this will be used with other sfml functions, so it uses uppercase
    return getWorldTransform() * sf::Vector2f();
}
sf::Transform scene_node::getWorldTransform() const
{
    //walk up from the parent to the root scene node applying transforms to get the global transform
    sf::Transform t = sf::Transform::Identity;
    for (const scene_node* node = this; node != nullptr; node = node->parent)
    {
        t = node->getTransform() * t;
    }
    return t;
}
void scene_node::draw_current(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (draw_this)
    {

    }
}
unsigned int scene_node::get_category() const
{
    return category;
}
void scene_node::on_command(const command& cmd, sf::Time delta)
{
    //check and perform this action upon this node
    if (cmd.ccategory & get_category())
    {
        cmd.action(*this, delta);
    }
    //forward command to children
    for (scn_ptr& child : children)
    {
        child->on_command(cmd, delta);
    }
}
void scene_node::print()
{
    std::cout << "this node: " << get_category();
    for (scn_ptr& child : children)
    {
        std::cout << "\n\tchild node: " << child->get_category() << std::endl;
        child->print();
    }
    std::cout << "\n";
}
sf::FloatRect scene_node::getBoundingRect() const
{
    return sf::FloatRect(0,0,0,0);
}
bool scene_node::collision(const scene_node& lhs, const scene_node& rhs)
{
    //our very simple collision test using sf::FloatRect::intersects() which probably uses SAT
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}
void scene_node::check_node_collision(scene_node& node, std::set<scn_pair>& collision_pairs)
{
    if (this != &node &&
        collision(*this, node) &&
        !is_dead() &&
        !node.is_dead())
    {
        //using minmax ensures that a-b and b-a will always be in the same order
        collision_pairs.insert(std::minmax(this, &node));
    }
    for (scn_ptr& child : children)
    {
        child->check_node_collision(node, collision_pairs);
    }
}
void scene_node::check_scene_collision(scene_node& scene_graph, std::set<scn_pair>& collision_pairs)
{
    check_node_collision(scene_graph, collision_pairs);

    for (scn_ptr& child : scene_graph.children)
    {
        check_scene_collision(*child, collision_pairs);
    }
}
bool scene_node::is_dead() const
{
    return false;
}
bool scene_node::is_marked_for_removal() const
{
    return is_dead();
}
void scene_node::remove_wrecks()
{
    auto wreck_field_begin = std::remove_if(children.begin(), children.end(),
                                    std::mem_fn(&scene_node::is_marked_for_removal));
    children.erase(wreck_field_begin, children.end());

    std::for_each(children.begin(), children.end(), std::mem_fn(&scene_node::remove_wrecks));
}
sf::Vector2f scene_node::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}
