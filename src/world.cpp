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
    zoom_level = 1;
    load_textures();
    build_scene();
    set_zoom(.2f);
    world_view.setCenter(spawn_position);
    sf::Vector2i win_center(wwindow.getSize() / 2u);
    sf::Mouse::setPosition(win_center, wwindow);
    wwindow.setMouseCursorVisible(false);
    add_enemies();
}
void world::load_textures()
{
    textures.load(textures::player,         "src/gfx/player.png");
    textures.load(textures::floor,          "src/gfx/floor.png");
    textures.load(textures::cursor,         "src/gfx/cursor.png");
    textures.load(textures::small_mutant,   "src/gfx/small_mutant.png");
    textures.load(textures::bullet,         "src/gfx/bullet.png");
    fonts.load(fonts::pixel,                "src/pixel.ttf");
}
void world::build_scene()
{
    for (std::size_t k = 0; k < layer_count; ++k)
    {
        //create layers, add a reference to them to the scene_layers, and then
        //move them to the scene graph
        cmd_category::ID category = (k == air_layer) ? cmd_category::air_layer : cmd_category::scene;
        scn_ptr layer(new scene_node(category));
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


    //here we create and attach the player and the cursor
    std::unique_ptr<monster> t(new monster(monster::type::player, textures, fonts, 100));
    the_player = t.get();
    the_player->setPosition(spawn_position);
    scene_layers[mon_layer]->attach_child(std::move(t));

    std::unique_ptr<cursor> cur(new cursor(textures));
    the_cursor = cur.get();
    the_cursor->setPosition(spawn_position);
    scene_layers[hud_layer]->attach_child(std::move(cur));

}
void world::draw()
{
    wwindow.setView(world_view);
    wwindow.draw(scene_graph);
}
void world::update(sf::Time delta)
{
    spawn_enemies();
    //before any commands are applied, reset the player speed to 0
    the_player->set_velocity(0.f, 0.f);
    //rotate towards the software cursor aka reticle
    rotate_player();
    //push all commands onto the queue
    while (!world_cmd_queue.is_empty())
    {
        scene_graph.on_command(world_cmd_queue.pop(), delta);
    }

    //fixes circular distances and world bounds
    adjust_player_v();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {scene_graph.print(); std::cout << "____\n";}
    update_cursor();
    world_view.setCenter(the_player->getPosition());
    scene_graph.update(delta, world_cmd_queue);
}
command_queue& world::get_cmd_queue()
{
    return world_cmd_queue;
}
void world::adjust_player_v()
{
    //currently, this just attempts to account for circular distances
    sf::Vector2f v = the_player->get_velocity();
    //if (v.x != 0) v.x = (2.f/3.f) * (v.x);
    //if (v.y != 0) v.y = (2.f/3.f) * (v.y);
    if (v.x != 0.f && v.y != 0.f)
    {
        v.x = v.x / std::sqrt(2.f);
        v.y = v.y / std::sqrt(2.f);
    }
    the_player->set_velocity(v);
}
void world::update_cursor()
{
    //this mess first calulates the new cursor position based on mouse movement for the last frame, as well as resetting the
    //hardward cursor to the center of the screen
    //then it checks for the cursor being out of bounds of the view
    //then it move the cursor to it's position
    //then it matches the velocity of the cursor to the velocity of the player
    //so the cursor moves when we do

    sf::Vector2i win_center(wwindow.getSize() / 2u);
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(wwindow);
    sf::Vector2i delta_mouse = win_center - mouse_pos;
    //printf("center: %d, %d\nmouse: %d, %d\n", win_center.x, win_center.y, mouse_pos.x, mouse_pos.y);
    //printf("center: %f, %f\nmouse: %f, %f\ncenter - pos: %f, %f\n", f_center.x, f_center.y, f_pos.x, f_pos.y, f_center.x - f_pos.x, f_center.y - f_pos.y);
    sf::Mouse::setPosition(win_center, wwindow);
    sf::Vector2f cur_pos = the_cursor->getPosition();
    sf::Vector2f view_center = world_view.getCenter();
    sf::Vector2f view_size = world_view.getSize();
    if (cur_pos.x <= view_center.x - (view_size.x / 2) && delta_mouse.x > 0) delta_mouse.x = 0;
    if (cur_pos.x >= view_center.x + (view_size.x / 2) && delta_mouse.x < 0) delta_mouse.x = 0;
    if (cur_pos.y <= view_center.y - (view_size.y / 2) && delta_mouse.y > 0) delta_mouse.y = 0;
    if (cur_pos.y >= view_center.y + (view_size.y / 2) && delta_mouse.y < 0) delta_mouse.y = 0;
    //this accounts for zoom level in reticle calculations for sensitivity
    the_cursor->move(-delta_mouse.x / (SENSITIVITY / zoom_level), -delta_mouse.y / (SENSITIVITY / zoom_level));
    the_cursor->set_velocity(the_player->get_velocity());
}
void world::rotate_player()
{
    sf::Vector2f diff = the_cursor->getPosition() - the_player->getPosition();
    float angle = atan2(diff.y, diff.x) * 180.f / PI;
    command rot;
    rot.ccategory = cmd_category::the_player;
    rot.action = monster::monster_rotator(angle);
    world_cmd_queue.push(rot);
}
world::~world()
{
}
void world::set_zoom(float f)
{
    if ((zoom_level <= UPPER_ZOOM_LIMIT && f > 1.f) ||
        (zoom_level >= LOWER_ZOOM_LIMIT && f < 1.f))
    {
        zoom_level *= f;
        world_view.zoom(f);
    }
}
float world::get_zoom()
{
    return zoom_level;
}
void world::spawn_enemies()
{
    while (!enemy_spawn_points.empty())
    {
        spawn_point spawn = enemy_spawn_points.back();
        std::unique_ptr<monster> enemy(new monster(spawn.stype, textures, fonts, 100));
        enemy->setPosition(spawn.x, spawn.y);
        scene_layers[mon_layer]->attach_child(std::move(enemy));
        enemy_spawn_points.pop_back();
    }
}
void world::add_enemies()
{
    add_enemy(monster::type::small_mutant, 50, 50);
}
void world::add_enemy(monster::type mtype, float x, float y)
{
    spawn_point new_p(mtype, the_player->getPosition().x + x, the_player->getPosition().y + y);
    enemy_spawn_points.push_back(new_p);
}
world::spawn_point::spawn_point(monster::type fstype, float fx, float fy)
{
    stype = fstype;
    x = fx;
    y = fy;
}