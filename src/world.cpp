#include "world.h"
typedef std::unique_ptr<scene_node> scn_ptr;
world::world(sf::RenderWindow& mwindow) :
    wwindow(mwindow),
    world_view(wwindow.getDefaultView()),
    world_bounds(0.f, 0.f, 2000, 2000),
    scene_graph(),
    spawn_position(world_bounds.width / 2.f, world_bounds.height / 2.f),
    the_player(nullptr)
{
    load_textures();
    build_scene();
    world_view.zoom(.2f);
    world_view.setCenter(spawn_position);
    //player_speed = 20;
}
void world::load_textures()
{
    textures.load(textures::player, "src/gfx/player.png");
    textures.load(textures::floor, "src/gfx/floor.png");
}
void world::build_scene()
{
    for (std::size_t k = 0; k < layer_count; ++k)
    {
        //create layers, add a reference to them to the scene_layers, and then
        //move them to the scene graph
        scn_ptr layer(new scene_node());
        scene_layers[k] = layer.get();
        scene_graph.attach_child(std::move(layer));
    }
    //load our one floor texture, set it to repeat mode, and set
    //the texture size to the whole bounds. this will be temporary
    sf::Texture& texture = textures.get(textures::floor);
    sf::IntRect texture_rect(world_bounds);
    texture.setRepeated(true);
    //create a new sprite node with this texture and settings
    //set its position to the origin
    //attach it to the "floor" scene layer
    std::unique_ptr<sprite_node> floor_sprite(new sprite_node(texture, texture_rect));
    floor_sprite->setPosition(world_bounds.left, world_bounds.top);
    scene_layers[bg_layer]->attach_child(std::move(floor_sprite));


    //here we create and attach the player
    std::unique_ptr<monster> t(new monster(monster::type::player, textures));
    the_player = t.get();
    the_player->setPosition(spawn_position);
    scene_layers[mon_layer]->attach_child(std::move(t));
    std::cout << "player type: " << the_player->get_category() << std::endl;
    scene_graph.print();
}
void world::draw()
{
    wwindow.setView(world_view);
    wwindow.draw(scene_graph);
}
void world::update(sf::Time delta)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) scene_graph.print();
    //all this stuff is pretty needed
    //send all commands to the scene graph
    while (!world_cmd_queue.is_empty())
    {
        scene_graph.on_command(world_cmd_queue.pop(), delta);
    }
    //std::cout << the_player->get_category();
    world_view.setCenter(the_player->getPosition());
    scene_graph.update(delta);
}
command_queue& world::get_cmd_queue()
{
    return world_cmd_queue;
}
//player& world::get_player()
//{
    //return *the_player;
//}
world::~world()
{
    //delete scene_graph;
    //delete the_player;
}
