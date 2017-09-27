#ifndef WORLD_H
#define WORLD_H

#include "include.h"
#include "texture_manager.h"
#include "resource_manager.h"
#include "scene_node.h"
#include "entity.h"
#include "monster.h"
#include "sprite_node.h"
#include "player.h"

class world : public sf::NonCopyable
{
    public:
        explicit world(sf::RenderWindow& window);
        virtual ~world();
        void update(sf::Time delta);
        void draw();
    private:
        void load_textures();
        void build_scene();
    private:
        enum scn_layer
        {
            bg_layer,
            mon_layer,
            walls_layer,
            hud_layer,
            layer_count,
        };
    private:
         sf::RenderWindow& wwindow;
         sf::View world_view;
         texture_manager textures;
         scene_node* scene_graph;
         std::array<scene_node*, layer_count> scene_layers;
         sf::FloatRect world_bounds;
         sf::Vector2f spawn_position;
         float default_zoom;
         player* the_player;
         float player_speed;
};

#endif // WORLD_H
